#pragma once

#include <string>
#include "VariableField.h"

namespace kmmarc
{
    class ControlField : public VariableField
    {
    public:
        std::string getData();
        void setData(std::string data);
    private:
        std::string m_data;
    };
}