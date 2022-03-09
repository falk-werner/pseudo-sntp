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
    static sntp_timestamp now();
    static sntp_timestamp zero();
    static sntp_timestamp from_iso8601(std::string const &value);

    uint32_t seconds;
    uint32_t fraction;
};

}

#endif
