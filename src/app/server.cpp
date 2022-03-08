#include "pseudo_sntp/sntp_server.hpp"
#include <cstdlib>
#include <csignal>
#include <iostream>

using pseudo_sntp::sntp_server;
using pseudo_sntp::sntp_packet;

namespace
{

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

}

int main(int argc, char * argv[])
{
    try
    {
        sntp_server server;
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
    }
    catch (...)
    {
        std::cerr << "error: unknown error" << std::endl;
    }
    
    g_server = nullptr;
    return EXIT_SUCCESS;
}