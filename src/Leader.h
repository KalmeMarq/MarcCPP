#pragma once

#include <vector>
#include <array>
#include <string>

namespace kmmarc
{
    class Leader
    {
    public:
        uint16_t getRecordLength();
        char getRecordStatus();
        char getTypeOfRecord();
        std::array<char, 2> &getImplDefined1();
        char getCharCodingScheme();
        uint8_t getIndicatorCount();
        uint8_t getSubfieldLength();
        int getBaseAddressOfData();
        std::array<char, 3> &getImplDefined2();
        std::array<char, 4> &getEntryMap();

        std::string marshal();
        void unmarshal(const std::string &leader);
    private:
        uint16_t m_recordLength;
        char m_recordStatus;
        char m_typeOfRecord;
        std::array<char, 2> m_implDefined1;
        char m_charCodingScheme;
        uint8_t m_indicatorCount;
        uint8_t m_subfieldLength;
        int m_baseAddressOfData;
        std::array<char, 3> m_implDefined2;
        std::array<char, 4> m_entryMap;
    };
}