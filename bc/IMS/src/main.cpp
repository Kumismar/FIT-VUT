#include <simlib.h>
#include <time.h>
#include "headers/quality.h"
#include "headers/facilities.h"
#include "headers/harvester.h"
#include "headers/harvest.h"
#include "headers/day_cycle.h"

const int tenMonths = 60 * 24 * 300;
const int oneDay = 60 * 24;
const int startTime = 0;
const int endTime = 60 * 24 * 365;

Facility underGround("");
Facility afterWinter("");
Facility growth("");
Facility day("");
Facility aliensCame("");

Store hectares("", 200);
Store cars("", 2);
Store grainWarehouse("", 1000);

Stat flourStat("Mnozstvi mouky");
Stat qualityStat("Kvalita psenice");
Stat harvesterRepair("Oprava kombajnu");
Stat grainDeployment("Vykecavani ridice u mlyna");
Stat hectaresLeft("Nesklizene hektary pole");
Stat aliensStat("Invaze mimozemstanu");

int qualityNum = 100;

int main() 
{
    RandomSeed(time(nullptr));
    SetOutput("output.log");

    for (int i = 0; i < 10000; i++) {
        Init(startTime, endTime);

        underGround.Clear();
        afterWinter.Clear();
        growth.Clear();
        day.Clear(); 
        aliensCame.Clear();

        hectares.Clear();
        cars.Clear();
        grainWarehouse.Clear();

        // Nejdrive spocitame, kolik zbylo psenice a pak na ni delame sklizen
        Quality* quality = new Quality();
        quality->Activate();

        (new DayCycle)->Activate(tenMonths + oneDay);
        (new Harvest)->Activate(tenMonths + oneDay);
        (new Harvester)->Activate(tenMonths + oneDay);

        Run();
    }
    flourStat.Output();
    qualityStat.Output();
    harvesterRepair.Output();
    grainDeployment.Output();
    hectaresLeft.Output();
    aliensStat.Output();
    return 0;
}