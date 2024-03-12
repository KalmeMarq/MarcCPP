#pragma once

#include <numeric>

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