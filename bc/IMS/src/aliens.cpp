#include <simlib.h>
#include "headers/aliens.h"
#include "headers/quality.h"
#include "headers/facilities.h"

Aliens::Aliens(Quality* quality)
{
    this->_quality = quality;
}

void Aliens::Behavior() 
{
    // Pokud prileti mimozemstane, vsechna psenice se znici
    const int probab = (int)(Uniform(0, 1000));
    if (probab < 5) {
        this->_quality->setQuality(0);
        // Priznak, ze sklizen vubec nenastane, protoze nemaji psenici
        Seize(aliensCame);
        aliensStat(1);
        hectaresLeft(hectares.Capacity());
    }
}