#pragma once

#include <simlib.h>

class Quality;

class Pests : public Process 
{
private:
    Quality* _quality;

public:
    Pests(Quality* quality);

    void Behavior() override;
};