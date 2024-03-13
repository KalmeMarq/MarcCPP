#pragma once

#include <vector>
#include <string>
#include "Record.h"
#include "DataField.h"

namespace kmmarc
{
	constexpr auto RT = 0x1D;
	constexpr auto FT = 0x1E;
    constexpr auto US = 0x1F;

    enum Encoding
    {
        Default,
        Iso85591,
        Utf8
    };

    struct WriteResult
    {
        int recordCount;
        int controlFieldCount;
        int dataFieldCount;
    };

    DataField readDataField(std::string tag, std::string data);

    std::vector<Record> read_mrc(const char* path, Encoding encoding = Encoding::Default);
    std::vector<Record> read_mrk(const char* path);
    std::vector<Record> read_mrk8(const char* path);
    std::vector<Record> read_json(const char* path);
    std::vector<Record> read_xml(const char* path);

    void write_mrc(const char* path, std::vector<Record>& records, Encoding encoding = Encoding::Default, bool is_encoding_forced = false);
    void write_mrk(const char* path, std::vector<Record>& records);
    void write_mrk8(const char* path, std::vector<Record>& records);
    void write_json(const char* path, std::vector<Record>& records);
    void write_xml(const char* path, std::vector<Record>& records);

    std::string getTagCodeFormatted(uint16_t tag);

    // From https://stackoverflow.com/a/39884120
    std::string iso_8859_1_to_utf8(std::string& str);

    // From https://stackoverflow.com/a/23690194
    std::string utf8_to_iso_8859_1(const std::string& in_str);
}