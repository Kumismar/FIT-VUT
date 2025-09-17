#pragma once

#include <simlib.h>

class DayCycle : public Process 
{
public:
    void Behavior() override;
};

class Night : public Process 
{
public:
    void Behavior() override;
};