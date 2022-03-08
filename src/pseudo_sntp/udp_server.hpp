#ifndef PSEUDO_SNTP_UDP_SERVER_HPP
#define PSEUDO_SNTP_UDP_SERVER_HPP

#include "pseudo_sntp/endpoint.hpp"
#include <inttypes.h>
#include <functional>

namespace pseudo_sntp
{

using upd_handler = std::function<void(endpoint const& from, uint8_t const * data, size_t size)>;

class udp_server
{
    udp_server(udp_server const&) = delete;
    udp_server& operator=(udp_server const&) = delete;
public:
    udp_server(endpoint const& endpoint);
    ~udp_server();
    void set_handler(upd_handler handler);
    int get_fd() const;
    void service();
    void interrupt();
private:
    upd_handler handler;
    int control_fds[2];
    int fd;
};

}

#endif
