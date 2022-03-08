#include "pseudo_sntp/udp_server.hpp"
#include "pseudo_sntp/endpoint.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>

#include <cstring>

#include <stdexcept>

namespace pseudo_sntp
{

udp_server::udp_server(endpoint const& local_endpoint)
: handler([](endpoint const&, uint8_t const*, size_t){})
{
    fd = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > fd)
    {
        throw std::runtime_error("failed to create UDP socket");
    }

    struct sockaddr_in address;
    ::memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = local_endpoint.ip;
    address.sin_port = htons(local_endpoint.port);

    int rc = ::bind(fd, reinterpret_cast<struct sockaddr*>(&address), sizeof(struct sockaddr_in));
    if (0 != rc)
    {
        ::close(fd);
        throw std::runtime_error("failed to bind");
    }

    rc = ::socketpair(AF_LOCAL, SOCK_DGRAM, 0, control_fds);
    if (0 != rc)
    {
        ::close(fd);
        throw std::runtime_error("failed to create control fds (socketpair)");
    }
}

udp_server::~udp_server()
{
    ::close(fd);
    ::close(control_fds[0]);
    ::close(control_fds[1]);
}

void udp_server::set_handler(upd_handler handler_)
{
    handler = handler_;
}

int udp_server::get_fd() const
{
    return fd;
}

void udp_server::service()
{
    pollfd fds[2];
    fds[0].fd = control_fds[0];
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    fds[1].fd = fd;
    fds[1].events = POLLIN;
    fds[1].revents = 0;

    int rc = poll(fds, 2, -1);
    if (0 < rc)
    {
        if (0 != (fds[0].revents & POLLIN))
        {
            char c;
            ::recv(0, &c, 1, 0);
        }

        if (0 != (fds[1].revents & POLLIN))
        {
            constexpr size_t const BUFFER_SIZE = 512;
            uint8_t data[BUFFER_SIZE];
            struct sockaddr_in address;
            address.sin_family = AF_INET;
            socklen_t address_length = sizeof(struct sockaddr_in);
            ssize_t len = recvfrom(fd, data, BUFFER_SIZE, 0, reinterpret_cast<struct sockaddr*>(&address), &address_length);
            if (0 < len)
            {
                endpoint from(address.sin_addr.s_addr, ntohs(address.sin_port));
                handler(from, data, len);
            }
        }
    }
}

void udp_server::interrupt()
{
   char c = '\0';
    ::send(control_fds[1], &c, 1, 0);
}

}