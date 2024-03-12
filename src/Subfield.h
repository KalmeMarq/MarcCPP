#pragma once

#include <string>

namespace kmmarc
{
    class Subfield
    {
    public:
        void setCode(char code);
        void setData(std::string data);
        char getCode();
        std::string getData();
    private:
        char m_code;
        std::string m_data;
    };
}