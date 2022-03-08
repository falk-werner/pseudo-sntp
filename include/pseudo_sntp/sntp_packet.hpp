#ifndef PSEUDO_SNTP_SNTP_PACKET_HPP
#define PSEUDO_SNTP_SNTP_PACKET_HPP

#include "pseudo_sntp/sntp_timestamp.hpp"

#include <cinttypes>
#include <cstddef>

namespace pseudo_sntp
{

struct sntp_packet
{
    sntp_packet();
    sntp_packet(uint8_t const * data, size_t size);
    
    size_t serialize(uint8_t * buffer, size_t size) const;
    void dump() const;

    uint8_t leap;
    uint8_t version;
    uint8_t mode;
    uint8_t stratum;
    uint8_t poll_intervall;
    uint8_t precision;
    uint32_t root_delay;
    uint32_t root_dispersion;
    unsigned char reference_identifier[4];
    sntp_timestamp reference;
    sntp_timestamp originate;
    sntp_timestamp receive;
    sntp_timestamp transmit;

};

}

#endif
