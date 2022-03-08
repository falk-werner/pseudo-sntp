#include "pseudo_sntp/sntp_packet.hpp"

#include <iostream>

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
        for (size_t i = 0; i < 4; i++) {
            uint8_t v = value & 0xff;
            value >>= 8;

            add_u8(v);
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
: leap(0)
, version(4)
, mode(3)
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
        << "leap                : " << (int) leap << std::endl
        << "version             : " << (int) version << std::endl
        << "mode                : " << (int) mode << std::endl
        << "stratum             : " << (int) stratum << std::endl
        << "poll intevall       : " << (int) poll_intervall << std::endl
        << "precision           : " << (int)  precision << std::endl
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