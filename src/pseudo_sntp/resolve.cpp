#include "pseudo_sntp/resolve.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <cstring>
#include <stdexcept>

namespace pseudo_sntp
{

uint32_t resolve_ipv4(std::string const& hostname)
{
    if (hostname.empty()) {return 0; }
    
    struct addrinfo hints;
    memset(reinterpret_cast<void*>(&hints), 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    struct addrinfo * addresses;
    int rc = ::getaddrinfo(hostname.c_str(), nullptr, &hints, &addresses);
    if (0 != rc)
    {
        throw std::runtime_error(std::string("failed to resolve ") + hostname + ": " + gai_strerror(rc));
    }

    struct sockaddr_in* address = reinterpret_cast<struct sockaddr_in*>(reinterpret_cast<void*>(addresses[0].ai_addr));
    int result = address->sin_addr.s_addr;

    freeaddrinfo(addresses);

    return result;
}

}