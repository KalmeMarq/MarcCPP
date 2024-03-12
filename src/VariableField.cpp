#include "VariableField.h"

namespace kmmarc
{
    void VariableField::setTag(uint16_t tag)
    {
        m_tag = tag;
    }

    uint16_t VariableField::getTag()
    {
        return m_tag;
    }
}