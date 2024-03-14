#pragma once

#include <vector>
#include <cstddef>
#include "Leader.h"
#include "ControlField.h"
#include "DataField.h"

namespace kmmarc
{
    class Record
    {
    public:
        Record() {};
        Record(Leader recordLeader, std::vector<ControlField> controlfields = {}, std::vector<DataField> datafields = {}) : leader(recordLeader), controlFields(controlfields), dataFields(datafields) {};

        Leader leader;
        std::vector<ControlField> controlFields;
        std::vector<DataField> dataFields;
    };
}