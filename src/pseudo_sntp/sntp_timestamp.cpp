#include "pseudo_sntp/sntp_timestamp.hpp"
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

std::string sntp_timestamp::to_str() const
{
    if ((0 == seconds) && (0 == fraction))
    {
        return "<null>";
    }

    constexpr uint32_t correction = 2208988800;
    if (seconds < correction)
    {
        return "<invalid>";
    }

    time_t value = seconds - correction;

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