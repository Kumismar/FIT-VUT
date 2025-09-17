#include <simlib.h>
#include <cmath>
#include "headers/heavy_rains.h"
#include "headers/quality.h"

HeavyRains::HeavyRains(Quality* quality) 
{
    this->_quality = quality;
}

void HeavyRains::Behavior() 
{
    const int probab = (int)(Uniform(0, 100));
    const int percentToLose = (int)(Normal(13, 3));
    if (probab < 33) {
        const int quality = this->_quality->getQuality();
        const int toChange = std::round(quality / 100.0 * percentToLose);
        this->_quality->setQuality(quality - toChange);
    }
}