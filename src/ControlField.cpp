#include "ControlField.h"

namespace kmmarc
{
    std::string ControlField::getData()
    {
        return m_data;
    }

    void ControlField::setData(std::string data)
    {
        m_data = data;
    }
}