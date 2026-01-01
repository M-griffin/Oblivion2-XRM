#ifndef UUID_HPP
#define UUID_HPP

#include <random>
#include <sstream>
#include <iomanip>

class Uuid {
public:
    explicit Uuid() = default;

    ~Uuid() = default;

    std::string createUuidString() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);

        uint32_t data[4];
        for (int i = 0; i < 4; ++i) {
            data[i] = dis(gen);
        }

        std::stringstream ss;
        ss << std::hex << std::setfill('0');

        // 8-4-4-4-12 format
        ss << std::setw(8) << (data[0])
                << '-'
                << std::setw(4) << ((data[1] >> 16) & 0xFFFF)
                << '-'
                << std::setw(4) << (((data[1] & 0xFFFF) & 0x0FFF) | 0x4000) // version 4
                << '-'
                << std::setw(4) << (((data[2] >> 16) & 0x0FFF) | 0x8000) // variant
                << '-'
                << std::setw(4) << (data[2] & 0xFFFF)
                << std::setw(8) << (data[3]);

        return ss.str();
    }
};

#endif
