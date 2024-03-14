#pragma once

#include <numeric>
#include <cstdint>

namespace kmmarc
{
    class VariableField
    {
    public:
        VariableField() {};
        VariableField(uint16_t tag) : m_tag(tag) {};

        uint16_t getTag();
        void setTag(uint16_t);
    private:
        uint16_t m_tag;
    };
}