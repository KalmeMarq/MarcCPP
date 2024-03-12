#include <fstream>
#include <sstream>
#include "Leader.h"

namespace kmmarc
{
    uint16_t Leader::getRecordLength()
    {
        return m_recordLength;
    }

    char Leader::getRecordStatus()
    {
        return m_recordStatus;
    }

    char Leader::getTypeOfRecord()
    {
        return m_typeOfRecord;
    }

    std::array<char, 2>& Leader::getImplDefined1()
    {
        return m_implDefined1;
    }

    char Leader::getCharCodingScheme()
    {
        return m_charCodingScheme;
    }

    uint8_t Leader::getIndicatorCount()
    {
        return m_indicatorCount;
    }

    uint8_t Leader::getSubfieldLength()
    {
        return m_subfieldLength;
    }

    int Leader::getBaseAddressOfData()
    {
        return m_baseAddressOfData;
    }

    std::array<char, 3>& Leader::getImplDefined2()
    {
        return m_implDefined2;
    }

    std::array<char, 4>& Leader::getEntryMap()
    {
        return m_entryMap;
    }

    std::string Leader::marshal()
    {
        std::string result;
        {
            std::stringstream ss;
            char data[6] = { 0 };
            std::snprintf(data, 6, "%05d", m_recordLength);
            ss << data;
            result.append(ss.str());
        }
        result.append(std::string(1, m_recordStatus));
        result.append(std::string(1, m_typeOfRecord));
        result.append(std::string(1, m_implDefined1[0]));
        result.append(std::string(1, m_implDefined1[1]));
        result.append(std::string(1, m_charCodingScheme));
        result.append(std::to_string(m_indicatorCount));
        result.append(std::to_string(m_subfieldLength));
        {
            std::stringstream ss;
            char data[6] = { 0 };
            std::snprintf(data, 6, "%05d", m_baseAddressOfData);
            ss << data;
            result.append(ss.str());
        }
        result.append(std::string(1, m_implDefined2[0]));
        result.append(std::string(1, m_implDefined2[1]));
        result.append(std::string(1, m_implDefined2[2]));
        result.append(std::string(1, m_entryMap[0]));
        result.append(std::string(1, m_entryMap[1]));
        result.append(std::string(1, m_entryMap[2]));
        result.append(std::string(1, m_entryMap[3]));
        return result;
    }

    void Leader::unmarshal(const std::string& leader)
    {
        m_recordLength = std::stoi(leader.substr(0, 5));
        m_recordStatus = leader.at(5);
        m_typeOfRecord = leader.at(6);

        m_implDefined1[0] = leader.at(7);
        m_implDefined1[1] = leader.at(8);

        m_charCodingScheme = leader.at(9);
        m_indicatorCount = std::stoi(leader.substr(10, 1));
        m_subfieldLength = std::stoi(leader.substr(11, 1));
        m_baseAddressOfData = std::stoi(leader.substr(12, 5));

        m_implDefined2[0] = leader.at(17);
        m_implDefined2[1] = leader.at(18);
        m_implDefined2[2] = leader.at(19);

        m_entryMap[0] = leader.at(20);
        m_entryMap[1] = leader.at(21);
        m_entryMap[2] = leader.at(22);
        m_entryMap[3] = leader.at(23);
    }
}