#pragma once

#include <simlib.h>

class Quality : public Process 
{
private:
    int _quality = 100;

public:
    void Behavior() override;

    int getQuality();

    void setQuality(int value);
};
