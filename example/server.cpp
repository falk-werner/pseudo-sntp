#include "pseudo_sntp/sntp_server.hpp"

#include <getopt.h>

#include <cstdlib>
#include <csignal>

#include <string>
#include <iostream>

using pseudo_sntp::sntp_server;
using pseudo_sntp::sntp_packet;

namespace
{

struct context
{
    std::string local_interface;
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
    -h, --help                  - print this message
Examples:
    sudo sntp-client
    sudo sntp-client -i localhost
)";
}

void parse_args(int argc, char * argv[], context & ctx)
{
    ctx.local_interface = "";
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
            {"help"     , no_argument      , nullptr, 0},
            {nullptr    , 0                , nullptr, 0}
        };

        int c = getopt_long(argc, argv, "i:h", long_opts, nullptr);
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
            default:
                ctx.exit_code = EXIT_FAILURE;
                ctx.show_help = true;
                finished = true;
        }
    }
}

int run(std::string const & local_interface)
{
    int exit_code = EXIT_SUCCESS;

    try
    {
        sntp_server server(local_interface);
        g_server = &server;

        signal(SIGINT, &on_shutdown_requested);

        server.set_packet_handler([](sntp_packet const& request, sntp_packet & reply) {
            request.dump();
            reply.dump();
        });

        while (!g_shutdown_requested)
        {
            server.service();
        }
    }
    catch (std::exception const& ex)
    {
        std::cerr << "error: " << ex.what() << std::endl;
        exit_code = EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "error: unknown error" << std::endl;
        exit_code = EXIT_FAILURE;
    }
    
    g_server = nullptr;
    return exit_code;
}

}

int main(int argc, char * argv[])
{
    context ctx;
    parse_args(argc, argv, ctx);

    if (!ctx.show_help)
    {
        ctx.exit_code = run(ctx.local_interface);
    }
    else
    {
        print_usage();
    }

    return ctx.exit_code;
}
