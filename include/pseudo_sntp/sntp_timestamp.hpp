#ifndef PSEUDO_SNTP_SNTP_TIMESTAMP_HPP
#define PSEUDO_SNTP_SNTP_TIMESTAMP_HPP

#include <cinttypes>
#include <string>

namespace pseudo_sntp
{

struct sntp_timestamp
{
    sntp_timestamp();
    std::string to_str() const;

    uint32_t seconds;
    uint32_t fraction;
};

}

#endif
