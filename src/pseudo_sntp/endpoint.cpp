#include "pseudo_sntp/endpoint.hpp"
#include "pseudo_sntp/resolve.hpp"

namespace pseudo_sntp
{

endpoint::endpoint()
: ip(0)
, port(0)
{

}

endpoint::endpoint(std::string const& hostname, uint16_t port_)
: ip(resolve_ipv4(hostname))
, port(port_)
{

}

endpoint::endpoint(uint32_t ip_, uint16_t port_)
: ip(ip_)
, port(port_)
{

}

}