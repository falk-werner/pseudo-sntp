#include "pseudo_sntp/sntp_server.hpp"

#include <getopt.h>

#include <cstdlib>
#include <csignal>

#include <string>
#include <iostream>

using pseudo_sntp::sntp_server;
using pseudo_sntp::sntp_packet;
using pseudo_sntp::sntp_timestamp;

namespace
{

struct context
{
    std::string local_interface;
    std::string time;
    int stratum;
    bool verbose;
    bool show_help;
    int exit_code;
};

static sntp_server * g_server = nullptr;
static bool g_shutdown_requested = false;

void on_shutdown_requested(int)
{
    if (NULL != g_server)
    {
        g_shutdown_requested = true;
        g_server->interrupt();        
    }
}

void print_usage()
{
    std::cout << R"(sntp-server, (C) 2022 Falk Werner <github.com/falk-werner>
Dummy SNTP server for testing purposes" << std::endl

Usage:
    sntp-server [-i <interface>]

Note:
    This app needs special privileges to run in order to bind to NTP port (123 UDP).

Options:
    -i, --interface <interface> - bind to specific interface (Default: any)
    -s, --stratum   <stratum>   - override stratum (0..15)
    -t, --time      <time>      - fake time (Default: use system time)
    -v, --verbose               - print SNTP replies
    -h, --help                  - print this message
Examples:
    sudo sntp-client
    sudo sntp-client -i localhost
    sudo sntp-client -i localhost -stratum 1
    sudo sntp-client -i localhost -time 2020-12-31T01:23:45Z    
)";
}

void parse_args(int argc, char * argv[], context & ctx)
{
    ctx.local_interface = "";
    ctx.time = "";
    ctx.stratum = -1;
    ctx.verbose = false;
    ctx.show_help = false;
    ctx.exit_code = EXIT_SUCCESS;

    opterr = 0;
    optind = 0;

    bool finished = false;
    while (!finished)
    {
        static option const long_opts[] =
        {
            {"interface", required_argument, nullptr, 0},
            {"time"     , required_argument, nullptr, 0},
            {"stratum"  , required_argument, nullptr, 0},
            {"verbose"  , no_argument      , nullptr, 0},
            {"help"     , no_argument      , nullptr, 0},
            {nullptr    , 0                , nullptr, 0}
        };

        int c = getopt_long(argc, argv, "i:t:s:vh", long_opts, nullptr);
        switch (c)
        {
            case -1:
                finished = true;
                break;
            case 'h':
                ctx.show_help = true;
                finished = true;
                break;
            case 'i':
                ctx.local_interface = optarg;
                break;
            case 't':
                try
                {
                    sntp_timestamp::from_iso8601(optarg);
                    ctx.time = optarg;
                    
                }
                catch (...)
                {
                    std::cerr << "error: invalid time" << std::endl;
                    ctx.exit_code = EXIT_FAILURE;
                    ctx.show_help = true;
                    finished = true;

                }
                break;
            case 's':
                try 
                {
                    int stratum = std::stoi(optarg);
                    if ((0 <= stratum) && (stratum < 16))
                    {
                        ctx.stratum = stratum;
                    }
                    else
                    {
                        throw std::runtime_error("invalid value");
                    }
                }
                catch (...)
                {
                    std::cerr << "error: invalid value for stratum" << std::endl;
                    ctx.exit_code = EXIT_FAILURE;
                    ctx.show_help = true;
                    finished = true;
                }
                break;
            case 'v':
                ctx.verbose = true;
                break; 
            default:
                std::cerr << "error: invalid option" << std::endl;
                ctx.exit_code = EXIT_FAILURE;
                ctx.show_help = true;
                finished = true;
                break;
        }
    }
}

void run(context &ctx)
{
    try
    {
        sntp_server server(ctx.local_interface);
        g_server = &server;

        signal(SIGINT, &on_shutdown_requested);

        server.set_packet_handler([&ctx](sntp_packet const&, sntp_packet & reply) {
            if (!ctx.time.empty())
            {
                reply.receive = pseudo_sntp::sntp_timestamp::from_iso8601(ctx.time);
                reply.transmit = reply.receive;
            }

            if (0 <= ctx.stratum)
            {
                reply.stratum = ctx.stratum;
            }

            if (ctx.verbose)
            {
                reply.dump();
                std::cout << std::endl;
            }
        });

        while (!g_shutdown_requested)
        {
            server.service();
        }
    }
    catch (std::exception const& ex)
    {
        std::cerr << "error: " << ex.what() << std::endl;
        ctx.exit_code = EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "error: unknown error" << std::endl;
        ctx.exit_code = EXIT_FAILURE;
    }
    
    g_server = nullptr;
}

}

int main(int argc, char * argv[])
{
    context ctx;
    parse_args(argc, argv, ctx);

    if (!ctx.show_help)
    {
        run(ctx);
    }
    else
    {
        print_usage();
    }

    return ctx.exit_code;
}
