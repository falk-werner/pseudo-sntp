#ifndef PSEUDO_SNTP_UDP_CLIENT_HPP
#define PSEUDP_SNTP_UDP_CLIENT_HPP

#include "pseudo_sntp/endpoint.hpp"

#include <cinttypes>
#include <cstddef>

namespace pseudo_sntp
{

class udp_client
{
    udp_client(udp_client const &) = delete;
    udp_client& operator=(udp_client const &) = delete;
public:
    udp_client();
    explicit udp_client(int fd);
    ~udp_client();
    void set_timeout(int millis);
    void send_to(uint8_t const * data, size_t size, endpoint const& peer);
    size_t recv_from(uint8_t * buffer, size_t size, endpoint & peer);

private:
    bool dont_close;
    int fd;
};

}

#endif
