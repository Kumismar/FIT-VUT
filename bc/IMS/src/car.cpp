#include <simlib.h>
#include "headers/car.h"
#include "headers/facilities.h" 

const double carTravel = 20.0;
const double carTravelDisperse = 1.0;
const double zrniDeploymentMin = 10.0;
const double zrniDeploymentMax = 20.0;

const int grainLoadTons = 5;

void Car::Behavior()
{
    // Cesta, vylozeni, cesta zpet
    Wait(Normal(carTravel, carTravelDisperse));
    // 10 minut vyklada zrni, ale muze se zakecat
    double deploymentTime = 10 + Exponential(3.0);
    Wait(deploymentTime);
    grainDeployment(deploymentTime);
    Leave(grainWarehouse, grainLoadTons);
    Wait(Normal(carTravel, carTravelDisperse));
    Leave(cars, 1);
}