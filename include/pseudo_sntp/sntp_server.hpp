#ifndef PSEUDO_SNTP_SNTP_SERVER_HPP
#define PSEUDO_SNTP_SNTP_SERVER_HPP

#include "pseudo_sntp/sntp_packet.hpp"

#include <string>
#include <functional>

namespace pseudo_sntp
{

using packet_handler = std::function<void(sntp_packet const&, sntp_packet &)>;

class sntp_server
{
    sntp_server(sntp_server const &other) = delete;
    sntp_server& operator=(sntp_server const& other) = delete;
public:
    sntp_server();
    sntp_server(std::string const & local_interface);
    ~sntp_server();
    void set_packet_handler(packet_handler handler);
    void service();
    void interrupt();
    int get_socket() const;
private:
    class detail;
    detail * d;
};

}

#endif
