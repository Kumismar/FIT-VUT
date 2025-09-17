#pragma once

#include <simlib.h>
#include "facilities.h"

class Harvest : public Process 
{
public:
    void Behavior() override;
};