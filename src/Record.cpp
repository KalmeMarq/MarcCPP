#include "Record.h"

namespace kmmarc
{
    Record::Record()
    {
    }

    Record::Record(Leader leader)
    {
        this->leader = leader;
    }
}