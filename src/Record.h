#pragma once

#include <vector>
#include "Leader.h"
#include "ControlField.h"
#include "DataField.h"

namespace kmmarc
{
    class Record
    {
    public:
        Record();
        Record(Leader leader);

        Leader leader;
        std::vector<ControlField> controlFields;
        std::vector<DataField> dataFields;
    };
}