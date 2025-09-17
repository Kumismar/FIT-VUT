#pragma once

#include <simlib.h>

class Car : public Process
{
public:
    void Behavior() override;
};