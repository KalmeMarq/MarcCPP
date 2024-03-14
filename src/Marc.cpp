#include <iostream>
#include <fstream>
#include <sstream>
#include "Marc.h"
#include "Subfield.h"
#include "DataField.h"
#include "pugixml.hpp"
#include "nlohmann/json.hpp"

namespace kmmarc
{
    DataField readDataField(Encoding encoding, std::string tag, std::string data)
    {
        DataField dataField;
        dataField.setTag((uint16_t) std::stoi(tag));
        dataField.setInd1(data.at(0));
        dataField.setInd2(data.at(1));

        const char *raw_data = data.c_str();

        int offset = 0;
        while (offset < data.length()) {
            char readByte = raw_data[offset++];

            if (readByte == US) {
                if (offset >= data.length()) break;
                char code = raw_data[offset++];

                if (code == FT)
                {
                    continue;
                }

                int bck_offset = offset;
                while (offset < data.length() && raw_data[offset] != US && raw_data[offset] != FT)
                {
                    offset++;
                }

                Subfield subfield;
                subfield.setCode(code);
                std::string subfieldData = data.substr(bck_offset, offset - bck_offset);
                subfield.setData(encoding == Encoding::Utf8 ? utf8_to_iso_8859_1(subfieldData) : subfieldData);
                dataField.subfields.push_back(subfield);
            }
            else if (readByte == FT)
            {
                continue;
            }
        }

        return dataField;
    }

    std::vector<Record> read_mrc(const char *path, Encoding encoding)
    {
        std::vector<Record> records;
        
        std::ifstream input;
        input.open(path, std::ios::binary);

        std::vector<char> leaderBuffer(24, 0);

        while (input.read(leaderBuffer.data(), leaderBuffer.size()))
        {
            std::string leaderStr(leaderBuffer.begin(), leaderBuffer.end());
            kmmarc::Leader leader;
            leader.unmarshal(leaderStr);

            kmmarc::Record record(leader);

            Encoding recordEncoding = leader.getCharCodingScheme() == 'a' ? Encoding::Utf8 : Encoding::Iso85591;

            std::vector<char> recordBuffer(leader.getRecordLength() - 24, 0);
            input.read(recordBuffer.data(), recordBuffer.size());

            int directoryLength = leader.getBaseAddressOfData() - (24 + 1);
            int size = directoryLength / 12;

            std::string directoryStr(recordBuffer.begin(), recordBuffer.begin() + directoryLength);

            std::vector<std::string> tags;
            std::vector<int> lengths;
            std::vector<int> starts;

            for (size_t i = 0; i < size; ++i)
            {
                tags.push_back(directoryStr.substr(i * 12, 3));
                lengths.push_back(std::stoi(directoryStr.substr(i * 12 + 3, 4)));
                starts.push_back(std::stoi(directoryStr.substr(i * 12 + 7, 5)));
            }

            std::string contentStr(recordBuffer.begin() + directoryLength, recordBuffer.end());

            for (size_t i = 0; i < size; ++i)
            {
                int tagN = std::stoi(tags[i]);

                if (tagN < 10) {
                    kmmarc::ControlField controlField;
                    controlField.setTag((uint16_t) tagN);
                    std::string data = contentStr.substr(starts[i] + 1, lengths[i] - 1);
                    controlField.setData(recordEncoding == Encoding::Utf8 ? utf8_to_iso_8859_1(data) : data);
                    record.controlFields.push_back(controlField);
                }
                else
                {
                    kmmarc::DataField dataField = kmmarc::readDataField(recordEncoding, tags[i], contentStr.substr(starts[i] + 1, lengths[i]));
                    record.dataFields.push_back(dataField);
                }
            }

            if (contentStr.at(contentStr.size() - 1) != kmmarc::RT) {
                std::cerr << "Expected record terminator";
                break;
            }

            records.push_back(record);
        }

        input.close();

        return records;
    }

    std::vector<Record> read_mrk(const char *path)
    {
        std::vector<Record> records;

        std::ifstream input;
        input.open(path);

        if (input.is_open())
        {
            std::string line;
            size_t lineIdx = 0;

            Record record;

            while (std::getline(input, line))
            {
                if (line.size() == 0)
                {
                    records.push_back(record);
                    record = Record();
                    continue;
                }

                std::string n = line.substr(1, 3);

                if (n == "LDR")
                {
                    record.leader.unmarshal(line.substr(5, line.size() - 5));
                }
                else
                {
                    int tagN = std::stoi(n);

                    if (tagN < 10)
                    {
                        ControlField field;
                        field.setTag((uint16_t) tagN);
                        field.setData(line.substr(5, line.size() - 5));

                        record.controlFields.push_back(field);
                    }
                    else
                    {
                        DataField field;
                        field.setTag((uint16_t) tagN);
                        field.setInd1(line.at(5) == '/' ? ' ' : line.at(5));
                        field.setInd2(line.at(6) == '/' ? ' ' : line.at(6));

                        size_t df_size = line.size() - 7;

                        for (size_t i = 7; i < df_size; ++i)
                        {
                            if (line.at(i) == '$')
                            {
                                char code = line.at(i + 1);
                                i += 2;

                                size_t bck_i = i;
                                while (i < df_size && line.at(i) != '$')
                                {
                                    ++i;
                                }

                                Subfield subfield(code, line.substr(bck_i, i - bck_i));
                                field.subfields.push_back(subfield);
                            }
                        }

                        record.dataFields.push_back(field);
                    }
                }

                ++lineIdx;
            }
            
            records.push_back(record);
        }

        input.close();

        return records;
    }

    std::vector<Record> read_mrk8(const char *path)
    {
        std::vector<Record> records;

        std::ifstream input;
        input.open(path);

        if (input.is_open())
        {
            std::string line;
            size_t lineIdx = 0;

            Record record;

            while (std::getline(input, line))
            {
                if (line.size() == 0)
                {
                    records.push_back(record);
                    record = Record();
                    continue;
                }

                std::string n = line.substr(1, 3);

                if (n == "LDR")
                {
                    record.leader.unmarshal(line.substr(5, line.size() - 5));
                }
                else
                {
                    int tagN = std::stoi(n);

                    if (tagN < 10)
                    {
                        ControlField field;
                        field.setTag((uint16_t) tagN);
                        field.setData(line.substr(5, line.size() - 5));

                        record.controlFields.push_back(field);
                    }
                    else
                    {
                        DataField field;
                        field.setTag((uint16_t) tagN);
                        field.setInd1(line.at(5) == '/' ? ' ' : line.at(5));
                        field.setInd2(line.at(6) == '/' ? ' ' : line.at(6));

                        size_t df_size = line.size() - 7;

                        for (size_t i = 7; i < df_size; ++i)
                        {
                            if (line.at(i) == '$')
                            {
                                char code = line.at(i + 1);
                                i += 2;

                                size_t bck_i = i;
                                while (i < df_size && line.at(i) != '$')
                                {
                                    ++i;
                                }

                                Subfield subfield(code, line.substr(bck_i, i - bck_i));
                                field.subfields.push_back(subfield);
                            }
                        }

                        record.dataFields.push_back(field);
                    }
                }

                ++lineIdx;
            }
            
            records.push_back(record);
        }

        input.close();

        return records;
    }

    std::vector<Record> read_json(const char *path)
    {
        std::vector<Record> records;

        std::ifstream input(path);
        nlohmann::json data = nlohmann::json::parse(input);

        for (size_t i = 0; i < data.size(); ++i)
        {
            Record record;
            nlohmann::json record_obj = data[i];
            std::string leader_str = record_obj["leader"].get<std::string>();
            record.leader.unmarshal(leader_str);

            nlohmann::json field_obj = record_obj["fields"];

            for (size_t j = 0; j < field_obj.size(); ++j)
            {
                nlohmann::json f_obj = field_obj[j];
                
                for (auto &el : f_obj.items())
                {
                    uint16_t tag = (uint16_t) std::stoi(el.key());

                    if (tag < 10)
                    {
                        ControlField field;
                        field.setTag(tag);
                        std::string data_str = std::string(el.value().get<std::string>());
                        field.setData(data_str);
                        record.controlFields.push_back(field);
                    }
                    else
                    {
                        DataField field;
                        field.setTag(tag);

                        nlohmann::json field_obj = el.value();
                        field.setInd1(field_obj["ind1"].get<std::string>().at(0));
                        field.setInd2(field_obj["ind2"].get<std::string>().at(0));

                        nlohmann::json subfield_arr = field_obj["subfields"];

                        for (size_t k = 0; k < subfield_arr.size(); ++k)
                        {
                            nlohmann::json f1_obj = subfield_arr[j];
                
                            for (auto &el1 : f1_obj.items())
                            {
                                char code = el1.key().at(0);
                                Subfield subfield;
                                subfield.setCode(code);
                                std::string data_str = std::string(el1.value().get<std::string>());
                                subfield.setData(data_str);
                                field.subfields.push_back(subfield);
                            }
                        }

                        record.dataFields.push_back(field);
                    }
                }
            }

            records.push_back(record);
        }

        input.close();

        return records;
    }
    
    std::vector<Record> read_xml(const char *path)
    {
        std::vector<Record> records;

        std::string contents;
        {
            std::ifstream input;
            input.open(path);
            std::string s;
            while (std::getline(input, s))
            {
                contents.append(s);
            }
            input.close();
        }

        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_string(contents.c_str());

        pugi::xml_node collection_node = doc.child("collection");
        
        for (pugi::xml_node record_node : collection_node.children("record"))
        {
            Record record;
            pugi::xml_node leader_node = record_node.child("leader");
            std::string leader_str = std::string(leader_node.text().get());
            record.leader.unmarshal(leader_str);

            for (pugi::xml_node controlfield_node : record_node.children("controlfield"))
            {
                ControlField field;
                field.setTag((uint16_t) std::stoi(controlfield_node.attribute("tag").as_string()));
                std::string data_str = std::string(controlfield_node.text().get());
                field.setData(utf8_to_iso_8859_1(data_str));

                record.controlFields.push_back(field);
            }

            for (pugi::xml_node datafield_node : record_node.children("datafield"))
            {
                DataField field;
                field.setTag((uint16_t) std::stoi(datafield_node.attribute("tag").as_string()));
                field.setInd1(datafield_node.attribute("ind1").as_string()[0]);
                field.setInd2(datafield_node.attribute("ind2").as_string()[0]);
            
                for (pugi::xml_node subfield_node : datafield_node.children("subfield"))
                {
                    Subfield subfield;
                    subfield.setCode(subfield_node.attribute("code").as_string()[0]);
                    std::string data_str = std::string(subfield_node.text().get());
                    subfield.setData(utf8_to_iso_8859_1(data_str));
                    field.subfields.push_back(subfield);
                }

                record.dataFields.push_back(field);
            }

            records.push_back(record);
        }

        return records;
    }

    void write_mrc(const char *path, std::vector<Record> &records, Encoding encoding, bool is_encoding_forced)
    {
        std::ofstream output;
        output.open(path, std::ios::binary);

        if (output.is_open())
        {
            for (Record &record : records)
            {
                std::string leader_str = record.leader.marshal();
                Encoding recordEncoding = encoding;
                Encoding storedEncoding = leader_str.at(9) == 'a' ? Encoding::Utf8 : Encoding::Iso85591;

                if (encoding == Encoding::Default)
                {
                    if (leader_str.at(9) == 'a')
                    {
                        recordEncoding = Encoding::Utf8;
                    }
                    else
                    {
                        recordEncoding = Encoding::Iso85591;
                    }
                }
                else if (!is_encoding_forced)
                {
                    leader_str[9] = Encoding::Utf8 ? 'a' : ' ';
                }

                std::ostringstream dir_buf;
		        std::ostringstream data_buf;

                size_t previous = 0;
                for (ControlField &field : record.controlFields)
                {
                    std::string fieldData = field.getData();
                    data_buf << (recordEncoding == Encoding::Utf8 ? iso_8859_1_to_utf8(fieldData) : storedEncoding == Encoding::Utf8 ? utf8_to_iso_8859_1(fieldData) : fieldData) << (char)FT;
                    size_t a = data_buf.tellp();
                    dir_buf << getTagCodeFormatted(field.getTag());
                    {
                        std::stringstream ss;
                        char data[6] = { 0 };
                        std::snprintf(data, 6, "%04zd", a - previous);
                        ss << data;
                        dir_buf << ss.str();
                    }
                    {
                        std::stringstream ss;
                        char data[6] = { 0 };
                        std::snprintf(data, 6, "%05zd", previous);
                        ss << data;
                        dir_buf << ss.str();
                    }
                    previous = a;
                }

                for (DataField &field : record.dataFields)
                {
                    data_buf << field.getInd1();
			        data_buf << field.getInd2();

                    for (Subfield &subfield : field.subfields)
                    {
                        data_buf << (char)US;
                        data_buf << subfield.getCode();
                        std::string subfieldData = subfield.getData();
                        data_buf << (recordEncoding == Encoding::Utf8 ? iso_8859_1_to_utf8(subfieldData) : storedEncoding == Encoding::Utf8 ? utf8_to_iso_8859_1(subfieldData) : subfieldData);
                    }

                    data_buf << (char)FT;

                    size_t a = data_buf.tellp();
                    dir_buf << getTagCodeFormatted(field.getTag());
                    {
                        std::stringstream ss;
                        char data[6] = { 0 };
                        std::snprintf(data, 6, "%04zd", a - previous);
                        ss << data;
                        dir_buf << ss.str();
                    }
                    {
                        std::stringstream ss;
                        char data[6] = { 0 };
                        std::snprintf(data, 6, "%05zd", previous);
                        ss << data;
                        dir_buf << ss.str();
                    }
                    previous = a;
                }

                data_buf << (char)RT;

                data_buf.seekp(0, std::ios::end);
                int data_buf_size = data_buf.tellp();
                data_buf.seekp(0, std::ios::beg);
                
                dir_buf.seekp(0, std::ios::end);
                int dir_buf_size = dir_buf.tellp();
                dir_buf.seekp(0, std::ios::beg);

                int recordLength = data_buf_size + dir_buf_size + 24 + 1;
                {
                    std::stringstream ss;
                    char data[6] = { 0 };
                    std::snprintf(data, 6, "%05d", recordLength);
                    ss << data;
                    std::string recordLengthStr = ss.str();
                    leader_str[0] = recordLengthStr[0];
                    leader_str[1] = recordLengthStr[1];
                    leader_str[2] = recordLengthStr[2];
                    leader_str[3] = recordLengthStr[3];
                    leader_str[4] = recordLengthStr[4];
                }

                output << leader_str;
                output << dir_buf.str();
                output << (char) FT;
                output << data_buf.str();
            }

            output.close();
        }
    }

    void write_mrk(const char *path, std::vector<Record> &records)
    {
        std::ofstream output;
        output.open(path);

        int i = 0;
        for (Record &record : records)
        {
            output << "=LDR " << record.leader.marshal() << "\n";
            for (size_t i = 0; i < record.controlFields.size(); ++i)
            {
                ControlField c = record.controlFields[i];
                output << "=" << getTagCodeFormatted(c.getTag()) << " " << c.getData() << "\n";
            }

            for (size_t i = 0; i < record.dataFields.size(); ++i)
            {
                DataField c = record.dataFields[i];

                output << "=" << getTagCodeFormatted(c.getTag()) << " ";
                output << ((c.getInd1() == ' ') ? '\\' : c.getInd1());
                output << ((c.getInd2() == ' ') ? '\\' : c.getInd2());

                for (size_t j = 0; j < c.subfields.size(); ++j)
                {
                    Subfield s = c.subfields[j];
                    output << "$" << s.getCode() << s.getData();
                }
                output << std::endl;
            }

            if (i + 1 < records.size()) output << "\n";
        }

        output.close();
    }

    void write_mrk8(const char *path, std::vector<Record> &records)
    {
        std::ofstream out;
        std::stringstream output;
        out.open(path);

        int i = 0;
        for (Record &record : records)
        {
            output << "=LDR " << record.leader.marshal() << "\n";
            for (size_t i = 0; i < record.controlFields.size(); ++i)
            {
                ControlField c = record.controlFields[i];
                std::string d = c.getData();
                output << "=" << getTagCodeFormatted(c.getTag()) << " " << iso_8859_1_to_utf8(d) << "\n";
            }

            for (size_t i = 0; i < record.dataFields.size(); ++i)
            {
                DataField c = record.dataFields[i];

                output << "=" << getTagCodeFormatted(c.getTag()) << " ";
                output << ((c.getInd1() == ' ') ? '\\' : c.getInd1());
                output << ((c.getInd2() == ' ') ? '\\' : c.getInd2());

                for (size_t j = 0; j < c.subfields.size(); ++j)
                {
                    Subfield s = c.subfields[j];
                    std::string d = s.getData();
                    output << "$" << s.getCode() << iso_8859_1_to_utf8(d);
                }
                output << std::endl;
            }

            if (i + 1 < records.size()) output << "\n";
        }

        out << output.str();
        out.close();
    }

    void write_json(const char *path, std::vector<Record> &records)
    {
        nlohmann::json arr;

        size_t i = 0;
        for (Record &record : records)
        {
            nlohmann::json record_obj;
            record_obj["leader"] = record.leader.marshal();
            
            nlohmann::json fields_arr;

            size_t ci = 0;
            for (ControlField &field : record.controlFields)
            {
                nlohmann::json field_obj;
                field_obj[getTagCodeFormatted(field.getTag())] = field.getData();

                fields_arr[ci] = field_obj;
                ++ci;
            }

            for (DataField &field : record.dataFields)
            {
                nlohmann::json field_obj;

                nlohmann::json obj;
                obj["ind1"] = std::string(1, field.getInd1());
                obj["ind2"] = std::string(1, field.getInd2());
                
                nlohmann::json obj_arr;
                
                size_t si = 0;
                for (Subfield &subfield : field.subfields)
                {
                    nlohmann::json subfield_obj;
                    obj_arr[si] = subfield_obj;
                    ++si;
                }

                obj["subfields"] = obj_arr;
                field_obj[getTagCodeFormatted(field.getTag())] = obj;            
                fields_arr[ci] = field_obj;
                ++ci;
            }

            record_obj["fields"] = fields_arr;

            arr[i] = record_obj;
            ++i;
        }

        std::ofstream output;
        output.open(path);
        output << arr.dump(2);
        output.close();
    }

    void write_xml(const char *path, std::vector<Record> &records)
    {
        pugi::xml_document doc;
        pugi::xml_node collection_node = doc.append_child("collection");

        for (Record &record : records)
        {
            pugi::xml_node record_node = collection_node.append_child("record");
            pugi::xml_node leader_node = record_node.append_child("leader");
            leader_node.text().set(record.leader.marshal().c_str());

            for (ControlField &field : record.controlFields)
            {
                pugi::xml_node field_node = record_node.append_child("controlfield");
                pugi::xml_attribute tag_attr = field_node.append_attribute("tag");
                tag_attr.set_value(getTagCodeFormatted(field.getTag()).c_str());

                std::string data = field.getData();
                field_node.text().set(iso_8859_1_to_utf8(data).c_str());
            }

            for (DataField &field : record.dataFields)
            {
                pugi::xml_node field_node = record_node.append_child("datafield");
                pugi::xml_attribute tag_attr = field_node.append_attribute("tag");
                tag_attr.set_value(getTagCodeFormatted(field.getTag()).c_str());

                char indEnc[] = { field.getInd1(), 0 };
                pugi::xml_attribute ind1_attr = field_node.append_attribute("ind1");
                ind1_attr.set_value(indEnc);
                pugi::xml_attribute ind2_attr = field_node.append_attribute("ind2");
                indEnc[0] = field.getInd2();
                ind2_attr.set_value(indEnc);

                for (Subfield &subfield : field.subfields)
                {
                    pugi::xml_node subfield_node = field_node.append_child("subfield");
                    pugi::xml_attribute code_attr = subfield_node.append_attribute("code");
                    char codeEnc[] = { subfield.getCode(), 0 };
                    code_attr.set_value(codeEnc);

                    std::string data = subfield.getData();
                    subfield_node.text().set(iso_8859_1_to_utf8(data).c_str());
                }
            }
        }

        std::ofstream output;
        output.open(path);
        output << "<!xml encoding=\"UTF-8\">\n";
        doc.print(output);
        output.close();
    }

    std::string getTagCodeFormatted(uint16_t tag)
    {
        std::string result;
        {
            std::stringstream ss;
            char data[6] = { 0 };
            std::snprintf(data, 6, "%03d", tag);
            ss << data;
            result.append(ss.str());
        }
        return result;
    }

    // From https://stackoverflow.com/a/39884120
    std::string iso_8859_1_to_utf8(std::string &str)
    {
        std::string strOut;
        for (std::string::iterator it = str.begin(); it != str.end(); ++it)
        {
            uint8_t ch = *it;
            if (ch < 0x80) {
                strOut.push_back(ch);
            }
            else {
                strOut.push_back(0xc0 | ch >> 6);
                strOut.push_back(0x80 | (ch & 0x3f));
            }
        }
        return strOut;
    }

    // From https://stackoverflow.com/a/23690194
    std::string utf8_to_iso_8859_1(const std::string &in_str)
    {
        const char *in = in_str.c_str();
        std::string out;
        if (in == NULL) {
            return out;
        }

        unsigned int codepoint;
        while (*in != 0)
        {
            unsigned char ch = static_cast<unsigned char>(*in);
            if (ch <= 0x7f)
                codepoint = ch;
            else if (ch <= 0xbf)
                codepoint = (codepoint << 6) | (ch & 0x3f);
            else if (ch <= 0xdf)
                codepoint = ch & 0x1f;
            else if (ch <= 0xef)
                codepoint = ch & 0x0f;
            else
                codepoint = ch & 0x07;
            ++in;
            if (((*in & 0xc0) != 0x80) && (codepoint <= 0x10ffff))
            {
                if (codepoint <= 255)
                {
                    out.append(1, static_cast<char>(codepoint));
                }
                else
                {
                    // do whatever you want for out-of-bounds characters
                }
            }
        }
        return out;
    }
}
