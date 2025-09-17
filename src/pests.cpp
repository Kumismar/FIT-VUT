#include <simlib.h>
#include <cmath>
#include "headers/pests.h"
#include "headers/quality.h"

Pests::Pests(Quality* quality) 
{
    this->_quality = quality;
}

void Pests::Behavior() 
{
    const int probab = (int)(Uniform(0, 100));
    const int percentToLose = (int)(Normal(13, 3));
    if (probab < 33) {
        const int quality = this->_quality->getQuality();
        const int toChange = std::round(quality / 100.0 * percentToLose);
        this->_quality->setQuality(quality - toChange);
    }
}