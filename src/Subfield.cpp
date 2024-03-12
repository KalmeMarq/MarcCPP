#include "Subfield.h"

namespace kmmarc
{
    void Subfield::setCode(char code)
    {
        m_code = code;
    }

    void Subfield::setData(std::string data)
    {
        m_data = data;
    }

    char Subfield::getCode()
    {
        return m_code;
    }

    std::string Subfield::getData()
    {
        return m_data;
    }
}