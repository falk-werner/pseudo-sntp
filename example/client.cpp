#include "pseudo_sntp/sntp_client.hpp"

#include <getopt.h>

#include <cstddef>

#include <iostream>
#include <string>

using pseudo_sntp::sntp_client;
using pseudo_sntp::sntp_packet;

namespace
{

struct context
{
    std::string hostname;
    int timeout;
    bool show_help;
    int exit_code;
};

void print_usage()
{
    std::cout << R"(sntp-client, (C) 2022 by Falk Werner <github.com/falk-werner>
sntp demo client

Usage:
    sntp-client [-t <millis>] <hostname>

Options:
    -t, --timeout <millis> - timeout to wait for reply in milli seconds (Default: 5000)
    -h, --help             - print this message

Arguments:
    hostname - IP or hostname of (S)NTP server

Examples:
    sntp-client ntp.org
    sntp-client -t 1000 localhost
)";
}

void parse_args(int argc, char* argv[], context &ctx)
{
    ctx.hostname = "";
    ctx.timeout = 5000;
    ctx.show_help = false;
    ctx.exit_code = EXIT_SUCCESS;

    opterr = 0;
    optind = 0;

    bool finished = false;
    while (!finished)
    {
        static option const long_opts[] =
        {
            {"timeout", required_argument, nullptr, 0},
            {"help"   , no_argument      , nullptr, 0},
            {nullptr  , 0                , nullptr, 0}
        };

        int c = getopt_long(argc, argv, "t:h", long_opts, nullptr);
        switch (c)
        {
            case -1:
                finished = true;
                break;
            case 't':
                try
                {
                    ctx.timeout = std::stoi(optarg);
                }
                catch (...)
                {
                    std::cerr << "error: invalid timeout" << std::endl;
                    ctx.exit_code = EXIT_FAILURE;
                    ctx.show_help = true;
                    finished = true;
                }
                break;
            case 'h':
                ctx.show_help = true;
                finished = true;
                break;
            default:
                std::cerr << "error: invalid option" << std::endl;
                ctx.exit_code = EXIT_FAILURE;
                ctx.show_help = true;
                finished = true;
                break;
        }
    }

    if (optind < argc)
    {
        ctx.hostname = argv[optind];
    }
    else
    {
        std::cerr << "error: missing hostname" << std::endl;
        ctx.show_help = true;
        ctx.exit_code = EXIT_FAILURE;
    }
}

void run(context& ctx)
{
    try
    {
        sntp_client client(ctx.hostname);
        client.set_timeout(ctx.timeout);
        sntp_packet reply;
        client.poll(reply);
        reply.dump();
    }
    catch (std::exception const &ex)
    {
        std::cerr << "error: " << ex.what() << std::endl;
        ctx.exit_code = EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "error: unknown error" << std::endl;
        ctx.exit_code = EXIT_FAILURE;
    }
}

}

int main(int argc, char* argv[])
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