#include "pseudo_sntp/sntp_server.hpp"
#include "pseudo_sntp/sntp_defines.hpp"
#include "pseudo_sntp/resolve.hpp"
#include "pseudo_sntp/udp_server.hpp"
#include "pseudo_sntp/udp_client.hpp"

namespace pseudo_sntp
{

constexpr size_t const BUFFER_SIZE = 512;

void default_packet_handler(sntp_packet const & , sntp_packet & )
{
}

class sntp_server::detail
{
public:
    detail(endpoint const& local_endpoint)
    : server(local_endpoint)
    , handler(default_packet_handler)
    {
        server.set_handler([this](endpoint const& from, uint8_t const * data, size_t size) {
            try 
            {
                udp_client conn(server.get_fd());
                sntp_packet request(data, size);
                sntp_packet reply;

                handler(request, reply);

                uint8_t buffer[BUFFER_SIZE];
                size_t reply_size = reply.serialize(buffer, 512);
                conn.send_to(buffer, reply_size, from);
            }
            catch (...)
            {
                // swallow
            }
        });
    }

    udp_server server;
    packet_handler handler;
};

sntp_server::sntp_server()
: d(new detail(endpoint(0, SNTP_PORT)))
{

}

sntp_server::sntp_server(std::string const & local_interface)
: d(new detail(endpoint(resolve_ipv4(local_interface), SNTP_PORT)))
{

}

sntp_server::~sntp_server()
{
    delete d;
}

void sntp_server::set_packet_handler(packet_handler handler)
{
    d->handler = handler;
}

void sntp_server::service()
{
    d->server.service();
}

void sntp_server::interrupt()
{
    d->server.interrupt();
}

int sntp_server::get_socket() const
{
    return d->server.get_fd();
}

}