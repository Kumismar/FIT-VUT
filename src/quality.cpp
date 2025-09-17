#include <simlib.h>
#include "headers/quality.h"
#include "headers/frost.h"
#include "headers/drought.h"
#include "headers/heavy_rains.h"
#include "headers/pests.h"
#include "headers/facilities.h"
#include "headers/aliens.h"

const int fiveMonths = 60 * 24 * 150;

void Quality::Behavior() 
{
    // Faktory/poruchy definujici ztratu psenice za dany rok
    (new Aliens(this))->Activate();
    Wait(1);
    if (this->_quality == 0) {
        qualityNum = 0;
        qualityStat(0);
        flourStat(0);
        this->Cancel();
    }

    // Zimni poruchy
    Seize(underGround);
    (new Frost(this))->Activate();
    Wait(fiveMonths);
    Release(underGround);

    // Jarni/letni poruchy
    Seize(growth);
    (new Drought(this))->Activate();
    (new HeavyRains(this))->Activate();
    (new Pests(this))->Activate();
    Wait(fiveMonths);
    Release(growth);
    qualityStat(this->_quality);
    qualityNum = this->_quality;
}


int Quality::getQuality() 
{
    return this->_quality;
}

void Quality::setQuality(int value) 
{
    this->_quality = value;
}