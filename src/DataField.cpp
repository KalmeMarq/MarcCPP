#include "DataField.h"

namespace kmmarc
{
    void DataField::setInd1(char ind1)
    {
        m_ind1 = ind1;
    }

    void DataField::setInd2(char ind2)
    {
        m_ind2 = ind2;
    }

    char DataField::getInd1()
    {
        return m_ind1;
    }

    char DataField::getInd2()
    {
        return m_ind2;
    }
}