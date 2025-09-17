#include "headers/harvest.h"
#include "headers/facilities.h"
#include <simlib.h>

const int oneMonth = 24 * 60 * 30;

void Harvest::Behavior()
{
    // Pokud prileteli mimozemstani, sklizen vubec nenastane
    Seize(aliensCame);
    Release(aliensCame);
    // Po mesici je zbytek urody zahozen
    Wait(oneMonth);
    hectaresLeft(hectares.Free());
    Enter(hectares, hectares.Free());

    double flour = (qualityNum / 100.0) * Uniform(0.7, 0.8) * grainWarehouse.Free();
    flourStat(flour);
}