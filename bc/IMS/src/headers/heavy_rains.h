#pragma once

#include <simlib.h>

class Quality;

class HeavyRains : public Process 
{
private:
    Quality* _quality;

public:
    HeavyRains(Quality* quality);

    void Behavior() override;
};