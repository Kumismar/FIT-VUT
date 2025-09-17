#pragma once

#include <simlib.h>

class Quality;

class Frost : public Process 
{
private:
    Quality* _quality;

public:
    Frost(Quality* quality);

    void Behavior() override;

};