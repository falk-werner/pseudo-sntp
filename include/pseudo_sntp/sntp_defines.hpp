#ifndef PSEUDO_SNTP_SNTP_DEFINES_HPP
#define PSEUDO_SNTP_SNTP_DEFINES_HPP

#include <cinttypes>

namespace pseudo_sntp
{

constexpr uint16_t const SNTP_PORT = 123;

constexpr uint32_t const SNTP_UNIX_CORRECTION = 2208988800;

constexpr uint8_t const SNTP_LEAP_NO_WARNING = 0;
constexpr uint8_t const SNTP_LEAP_SECOND_MORE = 1;
constexpr uint8_t const SNTP_LEAP_SECOND_LESS = 2;
constexpr uint8_t const SNTP_LEAP_UNSYNCRHONIZED = 3;

constexpr uint8_t const SNTP_VERSION_4 = 4;

constexpr uint8_t const SNTP_MODE_SYMMETRIC_ACTIVE = 1;
constexpr uint8_t const SNTP_MODE_SYMMETRIC_PASSIVE = 2;
constexpr uint8_t const SNTP_MODE_CLIENT = 3;
constexpr uint8_t const SNTP_MODE_SERVER = 4;
constexpr uint8_t const SNTP_MODE_BROADCAST = 5;

constexpr uint8_t const SNTP_STRATUM_KISS_OF_DEATH = 0;
constexpr uint8_t const SNTP_STRATUM_PRIMARY = 1;
constexpr uint8_t const SNTP_STRATUM_SECONDARY_2 = 2;
constexpr uint8_t const SNTP_STRATUM_SECONDARY_3 = 3;
constexpr uint8_t const SNTP_STRATUM_SECONDARY_4 = 4;
constexpr uint8_t const SNTP_STRATUM_SECONDARY_5 = 5;
constexpr uint8_t const SNTP_STRATUM_SECONDARY_6 = 6;
constexpr uint8_t const SNTP_STRATUM_SECONDARY_7 = 7;
constexpr uint8_t const SNTP_STRATUM_SECONDARY_8 = 8;
constexpr uint8_t const SNTP_STRATUM_SECONDARY_9 = 9;
constexpr uint8_t const SNTP_STRATUM_SECONDARY_10 = 10;
constexpr uint8_t const SNTP_STRATUM_SECONDARY_11 = 11;
constexpr uint8_t const SNTP_STRATUM_SECONDARY_12 = 12;
constexpr uint8_t const SNTP_STRATUM_SECONDARY_13 = 13;
constexpr uint8_t const SNTP_STRATUM_SECONDARY_14 = 14;
constexpr uint8_t const SNTP_STRATUM_SECONDARY_15 = 15;

}

#endif
