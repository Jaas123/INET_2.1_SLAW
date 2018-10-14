//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "SlawBase.h"

//namespace inet{

SlawBase::SlawBase() {
    map = 0;//EAM :: nullptr;
    speed = 0;//EAM :: nullptr;
    pausetime = 0;//EAM :: nullptr;

}
void SlawBase::computeConfinedAreas(AreaSet& areaVector) {
    //printf("Mi mapa [Areas] :: %d \n",map->getNumberOfAreas());
//    if(map->getAreaWeights() == NULL){
//        printf("Es un objeto nulo, porqué ??? \n");
//    }
    const std::vector<double>* weights = map->getAreaWeights();
    double rnd;
    unsigned areaID = 0;
    uint8_t confinedAreas = intuniform(3, 5);
    while (areaVector.size() < confinedAreas) {
        rnd = uniform(0.0, 1.0);
        while (!((*weights)[areaID] < rnd && rnd <= (*weights)[areaID+1]))
            areaID++;
        auto it = std::find(areaVector.begin(), areaVector.end(), areaID);
        if (it == areaVector.end())
            areaVector.push_back(areaID);
        areaID = 0;
    }
}

Coord SlawBase::computeHome(const AreaSet& C_k) {
    unsigned areaIndex = intuniform(0, C_k.size() - 1);
    unsigned waypointIndex = intuniform(0, map->getAreaSize(C_k[areaIndex])-1);
    Coord home = map->getWaypoint(C_k[areaIndex], waypointIndex);
    return home;
}

void SlawBase::computeTripRandomness(Trip& trip, const AreaSet& C_k,
                                     Coord& home) {
    AreaSet::const_iterator areaIt;
    std::vector<Coord> temp;
    unsigned rndAreaId;
    do {
        rndAreaId = intuniform(0, map->getNumberOfAreas()-1);
        areaIt = std::find(C_k.begin(), C_k.end(), rndAreaId);
    } while ( areaIt != C_k.end() );

    unsigned numberOfRandomWaypoints = ceil(0.1 * map->getAreaSize(rndAreaId));
    while(trip.size() < numberOfRandomWaypoints) {
        unsigned rndWPIndex = intuniform(0, map->getAreaSize(rndAreaId)-1);
        Coord waypoint = map->getWaypoint(rndAreaId, rndWPIndex);
        if(waypoint == home)
            error("Slaw: WTF! how home waypoint %lf, %lf came here!!!",home.x, home.y); // EAM :: Como mejorar?
        if(std::find(trip.begin(), trip.end(), waypoint) == trip.end())
            trip.push_back(waypoint);
    }
}

void
SlawBase::computeRutine(Trip& trip, const AreaSet& rutineAreas,
                        Coord& home) {
    AreaSet selectedAreas;
    std::vector<Coord> temp;
    unsigned numberOfRutineAreas = unsigned(rutineAreas.size());
    //index of discarded area
    unsigned rndAreaIndex = intuniform(0, numberOfRutineAreas - 1);
    unsigned tripSize = intuniform(120, 150);//minus the home waypoint

    //Discard one area from the set C_k of confined areas of a walker k
    for (unsigned areaIndex = 0; areaIndex < numberOfRutineAreas; areaIndex++)
        if (rndAreaIndex != areaIndex)
            selectedAreas.push_back(rutineAreas.at(areaIndex));

    for (auto& area : selectedAreas)
        for (auto& waypoint : *(map->getConfinedArea((unsigned)area)))
            if (waypoint != home)
                temp.push_back((Coord)waypoint); //EAM :: Retomar

    unsigned rutineTripSize = tripSize - trip.size();
    if (temp.size() < rutineTripSize)
        for (auto& waypoint : temp)
            trip.push_back((Coord)waypoint); //EAM :: Retomar
    else {
        randomizeVector(temp);
        for (unsigned i = 0; i < rutineTripSize; i++)
            trip.push_back(temp[i]);
    }
}


SlawBase::~SlawBase() {
    // TODO Auto-generated destructor stub
}
//}
