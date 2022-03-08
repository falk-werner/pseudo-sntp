#ifndef PSEUDO_SNTP_ENDPOINT_HPP
#define PSEUDO_SNTP_ENDPOINT_HPP

#include <cinttypes>

#include <string>

namespace pseudo_sntp
{

struct endpoint
{
    endpoint();
    endpoint(std::string const& hostname, uint16_t port);
    endpoint(uint32_t ip, uint16_t port);

    uint32_t ip;
    uint16_t port;
};

}

#endif
