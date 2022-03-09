#include "pseudo_sntp/sntp_timestamp.hpp"
#include "pseudo_sntp/sntp_defines.hpp"

#include <time.h>
#include <stdexcept>
#include <iostream>
namespace pseudo_sntp
{

sntp_timestamp::sntp_timestamp()
: seconds(0)
, fraction(0)
{

}

sntp_timestamp sntp_timestamp::now()
{
    sntp_timestamp timestamp;
    timestamp.seconds = ((uint32_t)::time(nullptr)) + SNTP_UNIX_CORRECTION;
    return timestamp;
}

sntp_timestamp sntp_timestamp::zero()
{
    sntp_timestamp timestamp;
    return timestamp;
}

sntp_timestamp sntp_timestamp::from_iso8601(std::string const &value)
{
    struct tm t;
    char * result = strptime(value.c_str(), "%Y-%m-%dT%TZ", &t);
    if ((nullptr != result) && ('\0' == *result))
    {
        sntp_timestamp timestamp;
        timestamp.seconds = timegm(&t) + SNTP_UNIX_CORRECTION;
        return timestamp;
    }
    else
    {
        throw std::runtime_error("failed to parse time");
    }
}


std::string sntp_timestamp::to_str() const
{
    if ((0 == seconds) && (0 == fraction))
    {
        return "<null>";
    }

    if (seconds < SNTP_UNIX_CORRECTION)
    {
        return "<invalid>";
    }

    time_t value = seconds - SNTP_UNIX_CORRECTION;

    struct tm timep;
    if (NULL != gmtime_r(&value, &timep))
    {
        char buffer[100];
        strftime(buffer, 100, "%FT%TZ", &timep);
        return buffer;
    }

    throw std::runtime_error("invalid time");
}


}