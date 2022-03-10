#include "pseudo_sntp/sntp_client.hpp"
#include "pseudo_sntp/sntp_defines.hpp"
#include "pseudo_sntp/udp_client.hpp"
#include "pseudo_sntp/endpoint.hpp"
#include "pseudo_sntp/sntp_packet.hpp"

namespace pseudo_sntp
{

constexpr size_t const BUFFER_SIZE = 512;

class sntp_client::detail
{
public:
    detail(std::string const & hostname)
    : server(hostname, SNTP_PORT)
    {

    }

    endpoint server;
    udp_client client;
};

sntp_client::sntp_client(std::string const & hostname)
: d(new detail(hostname))
{


}

sntp_client::~sntp_client()
{
    delete(d);
}

void sntp_client::set_timeout(int millis)
{
    d->client.set_timeout(millis);
}

void sntp_client::poll(sntp_packet& packet)
{
    uint8_t data[BUFFER_SIZE] = {0};

    sntp_packet request;
    size_t size = request.serialize(data, BUFFER_SIZE);
    d->client.send_to(data, size, d->server);

    endpoint peer;
    size_t len = d->client.recv_from(data, BUFFER_SIZE, peer);
    sntp_packet reply(data, len);

    packet = reply;
}

}