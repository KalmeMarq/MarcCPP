#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Leader.h"
#include "Marc.h"

int main()
{
    std::vector<kmmarc::Record> records = kmmarc::read_mrk("../tests/record_1_0.mrk");

    std::cout << "Records: " << records.size() << std::endl;

    // {
    //     std::ifstream input;
    //     input.open("C:/Users/FilipeMarques/TestCMake/tests/b.txt");
    //     std::string m;
    //     std::getline(input, m);
    //     input.close();

    //     std::ofstream output;
    //     output.open("C:/Users/FilipeMarques/TestCMake/tests/c.txt");
    //     output << kmmarc::utf8_to_iso_8859_1(m);
    //     output.close();
    // }

    // std::ofstream output;
    // output.open("C:/Users/FilipeMarques/TestCMake/tests/a.txt");
    // std::string a("é");
    // output << a;
    // output.close();

    // kmmarc::write_mrc("C:/Users/FilipeMarques/TestCMake/tests/record_1_1.mrc", records, kmmarc::Encoding::Iso85591);

    // kmmarc::write_mrc("C:/Users/FilipeMarques/Documents/test1-utf8.mrc", records, kmmarc::Encoding::Utf8);
    // kmmarc::write_mrk("C:/Users/FilipeMarques/TestCMake/tests/record_1_0.mrk", records);
    // kmmarc::write_mrk8("C:/Users/FilipeMarques/Documents/test2.mrk8", records);
    // kmmarc::write_json("C:/Users/FilipeMarques/Documents/test2.json", records);
    // kmmarc::write_xml("C:/Users/FilipeMarques/Documents/test2.xml", records);

    return 0;
}