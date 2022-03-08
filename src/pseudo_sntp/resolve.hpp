#ifndef PSEUDO_SNTP_RESOLVE_HPP
#define PSEUDO_SNTP_RESOLVE_HPP

#include <cinttypes>
#include <string>

namespace pseudo_sntp
{

uint32_t resolve_ipv4(std::string const& hostname);

}

#endif
