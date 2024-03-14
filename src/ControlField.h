#pragma once

#include <string>
#include "VariableField.h"

namespace kmmarc
{
    class ControlField : public VariableField
    {
    public:
        ControlField() {};
        ControlField(uint16_t tag, std::string data) : VariableField(tag), m_data(data) {};

        std::string getData();
        void setData(std::string data);
    private:
        std::string m_data;
    };
}