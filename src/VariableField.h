#pragma once

#include <numeric>
#include <cstdint>

namespace kmmarc
{
    class VariableField
    {
    public:
        uint16_t getTag();
        void setTag(uint16_t);
    private:
        uint16_t m_tag;
    };
}