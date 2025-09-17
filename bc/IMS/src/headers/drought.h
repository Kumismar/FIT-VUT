#pragma once

#include <simlib.h>

class Quality;

class Drought : public Process 
{
private:
    Quality* _quality;

public:
    Drought(Quality* quality);

    void Behavior() override;
};