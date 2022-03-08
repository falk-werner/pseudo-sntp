#include "pseudo_sntp/sntp_client.hpp"

#include <cstddef>

#include <iostream>
#include <string>

using pseudo_sntp::sntp_client;
using pseudo_sntp::sntp_packet;

namespace
{

void print_usage()
{
    std::cout 
        << "sntp-client, (C) 2022 by Falk Werner <github.com/falk-werner>" << std::endl
        << "sntp demo client" << std::endl
        << std::endl
        << "Usage:" << std::endl
        << "    sntp-client <hostname>" << std::endl
        << std::endl
        << "Arguments:" << std::endl
        << "    hostname - IP or hostname of (S)NTP server" << std::endl
        << std::endl
        << "Example:" << std::endl
        << "    sntp-client ntp.org" << std::endl
        ; 
}

}

int main(int argc, char* argv[])
{
    int exit_code = EXIT_SUCCESS;

    if ((argc > 1) && (std::string("-h") != argv[1]) && (std::string("--help") != argv[1]))
    {
        try
        {
            char const * hostname = argv[1];
            sntp_client client(hostname);
            sntp_packet reply;
            client.poll(reply);
            reply.dump();
        }
        catch (std::exception const &ex)
        {
            std::cerr << "error: " << ex.what() << std::endl;
            exit_code = EXIT_FAILURE;
        }
        catch (...)
        {
            std::cerr << "error: unknown error" << std::endl;
            exit_code = EXIT_FAILURE;
        }

    }
    else
    {
        print_usage();
    }
    
    return exit_code;
}