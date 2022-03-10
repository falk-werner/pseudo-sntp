#include "pseudo_sntp/sntp_packet.hpp"
#include "pseudo_sntp/sntp_defines.hpp"

#include <cmath>

#include <iostream>

namespace
{

char const * dump_stratum(uint8_t stratum)
{
    if (pseudo_sntp::SNTP_STRATUM_KISS_OF_DEATH == stratum)
    {
        return "Kiss o' Death";
    }
    else if (pseudo_sntp::SNTP_STRATUM_PRIMARY == stratum)
    {
        return "primary";
    }
    else if (stratum < 16)
    {
        return "secondary";
    }
    else
    {
        return "invalid";
    }
}

char const * dump_leap(uint8_t leap)
{
    switch (leap)
    {
        case pseudo_sntp::SNTP_LEAP_NO_WARNING:
            return "no warning";
        case pseudo_sntp::SNTP_LEAP_SECOND_MORE:
            return "last minute has 61 seconds";
        case pseudo_sntp::SNTP_LEAP_SECOND_LESS:
            return "last minute has 59 seconds";
        case pseudo_sntp::SNTP_LEAP_UNSYNCRHONIZED:
            return "clock not synchronized";
        default:
            return "invalid";
    }
}

char const * dump_mode(uint8_t mode)
{
    switch (mode)
    {
        case pseudo_sntp::SNTP_MODE_SYMMETRIC_ACTIVE:
            return "symmetric active";
        case pseudo_sntp::SNTP_MODE_SYMMETRIC_PASSIVE:
            return "symmetric passive";
        case pseudo_sntp::SNTP_MODE_CLIENT:
            return "client";
        case pseudo_sntp::SNTP_MODE_SERVER:
            return "server";
        case pseudo_sntp::SNTP_MODE_BROADCAST:
            return "broadcast";
        default:
            return "invalid";
    }
}

}

namespace pseudo_sntp
{

class writer
{
public:
    writer(uint8_t * data_, size_t capacity_)
    : data(data_)
    , capacity(capacity_)
    , size(0)
    {

    }

    void add_u8(uint8_t value)
    {
        if (size < capacity)
        {
            data[size] = value;
            size++;
        }
        else
        {
            throw std::runtime_error("capacitry exceeded");
        }
    }

    void add_u32(uint32_t value)
    {
        uint8_t buffer[4];
        for (size_t i = 0; i < 4; i++) {
            buffer[i] = value & 0xff;
            value >>= 8;
        }

        for (int i = 3; i >= 0; i--) {
            add_u8(buffer[i]);
        }
    }

    size_t get_size() const
    {
        return size;
    }

private:
    uint8_t * data;
    size_t capacity;
    size_t size;
};

class reader
{
public:
    reader(uint8_t const * buffer_, size_t size_)
    : buffer(buffer_)
    , size(size_)
    , offset(0)
    {

    }

    uint8_t read_u8()
    {
        if (offset < size)
        {
            uint8_t value = buffer[offset];
            offset++;
            return value;
        }
        else
        {
            throw std::runtime_error("size exceeded");
        }
    }

    uint32_t read_u32()
    {
        uint32_t value = 0;
        for(size_t i = 0; i < 4; i++)
        {
            value <<= 8;
            value |= read_u8();
        }

        return value;
    }

private:
    uint8_t const * buffer;
    size_t size;
    size_t offset;
};

sntp_packet::sntp_packet()
: leap(SNTP_LEAP_NO_WARNING)
, version(SNTP_VERSION_4)
, mode(SNTP_MODE_CLIENT)
, stratum(0)
, poll_intervall(0)
, precision(0)
, root_delay(0)
, root_dispersion(0)
, reference_identifier{0,0,0,0}
{

}

sntp_packet::sntp_packet(uint8_t const * data, size_t size)
{
    reader r(data, size);
    uint8_t value = r.read_u8();

    leap    = (value >> 6) & 0x3;
    version = (value >> 3) & 0x7;
    mode    = value & 0x7;
    stratum = r.read_u8();
    poll_intervall = r.read_u8();
    precision = r.read_u8();

    root_delay = r.read_u32();
    root_dispersion = r.read_u32();
    
    reference_identifier[0] = r.read_u8();
    reference_identifier[1] = r.read_u8();
    reference_identifier[2] = r.read_u8();
    reference_identifier[3] = r.read_u8();

    reference.seconds = r.read_u32();
    reference.fraction = r.read_u32();

    originate.seconds = r.read_u32();
    originate.fraction = r.read_u32();

    receive.seconds = r.read_u32();
    receive.fraction = r.read_u32();

    transmit.seconds = r.read_u32();
    transmit.fraction = r.read_u32();
}

size_t sntp_packet::serialize(uint8_t * buffer, size_t size) const
{
    writer w(buffer, size);
    w.add_u8( ((leap & 0x3) << 6) | ((version & 0x7) << 3) | (mode & 0x07));
    w.add_u8(stratum);
    w.add_u8(poll_intervall);
    w.add_u8(precision);
    w.add_u32(root_delay);
    w.add_u32(root_dispersion);
    w.add_u8(reference_identifier[0]);
    w.add_u8(reference_identifier[1]);
    w.add_u8(reference_identifier[2]);
    w.add_u8(reference_identifier[3]);
    
    w.add_u32(reference.seconds);
    w.add_u32(reference.fraction);

    w.add_u32(originate.seconds);
    w.add_u32(originate.fraction);

    w.add_u32(receive.seconds);
    w.add_u32(receive.fraction);

    w.add_u32(transmit.seconds);
    w.add_u32(transmit.fraction);

    return w.get_size();
}

void sntp_packet::dump() const
{
    std::cout
        << "leap                : " << dump_leap(leap) << " (" << (int) leap << ")" << std::endl
        << "version             : " << (int) version << std::endl
        << "mode                : " << dump_mode(mode) << " (" << (int) mode << ")" <<std::endl
        << "stratum             : " << dump_stratum(stratum) << " (" << (int) stratum << ")" << std::endl
        << "poll intevall       : " << pow(2.0, poll_intervall) << "s (" << (int) poll_intervall << ")" << std::endl
        << "precision           : " << pow(2.0, precision) << "s (" << (int)  precision << ")" << std::endl
        << "root delay          : " << root_delay << std::endl
        << "root dispersion     : " << root_dispersion << std::endl
        << "reference identifier: " 
            << (int) reference_identifier[0] << "."
            << (int) reference_identifier[1] << "." 
            << (int) reference_identifier[2] << "." 
            << (int) reference_identifier[3] << std::endl
        << "reference timestamp : " << reference.to_str() << std::endl
        << "originate timestamp : " << originate.to_str() << std::endl
        << "receive timestamp   : " << receive.to_str() << std::endl
        << "transmit timestamp  : " << transmit.to_str() << std::endl
        ;
}

}