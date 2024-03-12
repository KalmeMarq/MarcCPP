#pragma once

#include <vector>
#include "VariableField.h"
#include "Subfield.h"

namespace kmmarc
{
    class DataField : public VariableField
    {
    public:
        void setInd1(char ind1);
        void setInd2(char ind2);
        char getInd1();
        char getInd2();

        std::vector<Subfield> subfields;
    private:
        char m_ind1;
        char m_ind2;
    };
}