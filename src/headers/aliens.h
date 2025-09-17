#pragma once

#include <simlib.h>

class Quality;

class Aliens : public Process 
{
private:
    Quality* _quality;

public:
    Aliens(Quality* quality);

    void Behavior() override;
};