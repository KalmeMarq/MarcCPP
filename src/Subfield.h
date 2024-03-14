#pragma once

#include <string>

namespace kmmarc
{
    class Subfield
    {
    public:
        Subfield() {};
        Subfield(char code, std::string data = "") : m_code(code), m_data(data) {};

        void setCode(char code);
        void setData(std::string data);
        char getCode();
        std::string getData();
    private:
        char m_code;
        std::string m_data;
    };
}