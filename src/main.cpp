#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Leader.h"
#include "Marc.h"

int main()
{
    std::vector<kmmarc::Record> records = kmmarc::read_xml("C:/Users/FilipeMarques/Documents/test1.xml");

    std::cout << "Records: " << records.size() << std::endl;

    kmmarc::write_mrc("C:/Users/FilipeMarques/Documents/test2.mrc", records);
    // kmmarc::write_mrc("C:/Users/FilipeMarques/Documents/test1-utf8.mrc", records, kmmarc::Encoding::Utf8);
    // kmmarc::write_mrk("C:/Users/FilipeMarques/Documents/test1.mrk", records);
    // kmmarc::write_mrk8("C:/Users/FilipeMarques/Documents/test2.mrk8", records);
    // kmmarc::write_json("C:/Users/FilipeMarques/Documents/test2.json", records);
    // kmmarc::write_xml("C:/Users/FilipeMarques/Documents/test2.xml", records);

    return 0;
}