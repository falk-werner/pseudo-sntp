#ifndef PSEUDO_SNTP_SNTP_CLIENT_HPP
#define PSEUDO_SNTP_SNTP_CLIENT_HPP

#include "pseudo_sntp/sntp_packet.hpp"
#include <string>

namespace pseudo_sntp
{

class sntp_client
{
    sntp_client(sntp_client const &) = delete;
    sntp_client& operator=(sntp_client const &) = delete;
public:
    sntp_client(std::string const & hostname);
    ~sntp_client();
    void poll(sntp_packet& packet);
private:
    class detail;
    detail * d;
};

}

#endif
