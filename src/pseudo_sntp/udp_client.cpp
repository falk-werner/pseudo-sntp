#include "pseudo_sntp/udp_client.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>

#include <stdexcept>


namespace pseudo_sntp
{

udp_client::udp_client()
: dont_close(false)
{
    fd = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > fd)
    {
        throw std::runtime_error("failed to create socket");
    }

    struct sockaddr_in address;
    memset(reinterpret_cast<void*>(&address), 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = 0;

    int rc = ::bind(fd, reinterpret_cast<struct sockaddr*>(&address), sizeof(struct sockaddr_in));
    if (0 != rc)
    {
        ::close(fd);
        throw std::runtime_error("failed to bind");
    }
}

udp_client::udp_client(int fd_)
: dont_close(true)
, fd(fd_)
{

}

udp_client::~udp_client()
{
    if (!dont_close)
    {
        ::close(fd);
    }
}

void udp_client::send_to(uint8_t const * data, size_t size, endpoint const& peer)
{
    struct sockaddr_in address;
    memset(reinterpret_cast<void*>(&address), 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = peer.ip;
    address.sin_port = htons(peer.port);

    ::sendto(fd, data, size, 0, reinterpret_cast<struct sockaddr *>(&address), sizeof(struct sockaddr_in));
}

size_t udp_client::recv_from(uint8_t * buffer, size_t size, endpoint & peer)
{
    struct sockaddr_in address;
    memset(reinterpret_cast<void*>(&address), 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    socklen_t address_length = sizeof(struct sockaddr_in);

    ssize_t len = ::recvfrom(fd, buffer, size, 0, reinterpret_cast<struct sockaddr *>(&address), &address_length);
    if (0 > len)
    {
        throw std::runtime_error("failed to recv");
    }

    if (address_length != sizeof(struct sockaddr_in))
    {
        throw std::runtime_error("invalid socket length");
    }

    peer.ip = address.sin_addr.s_addr;
    peer.port = ntohs(address.sin_port);

    return len;
}


}