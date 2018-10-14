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

#include "SlawMobility.h"
//namespace inet{


Define_Module(SlawMobility);

SlawMobility::SlawMobility()
{
   nextMoveIsWait = false;
   slaw = 0;//EAM ::nullptr;
   numberOfTrip = 0;
}

//TODO think about the use of signals to implement SLAW
void SlawMobility::initialize(int stage) {
    LineSegmentsMobilityBase::initialize(stage);
    if(stage == 0) {
        slawModuleName = par("slawModuleName").stringValue();
        slaw = (Slaw*) simulation.getSystemModule()->getSubmodule(slawModuleName.c_str());
        if(!slaw)
         error("No destination generator found: add module Slaw to the\
                network");
        slaw->initializeMobilityState(trip, areas, home);
    }
}

void SlawMobility::setInitialPosition() {
    lastPosition = home;
}

void SlawMobility::setTargetPosition()
{
   if(nextMoveIsWait)
      nextChange = simTime() + slaw->computePauseTime();
   else{
      if(trip.empty()) {
          slaw->computeTrip(trip, areas, home);
          targetPosition = home;
          EV << "Walker " /*EAM :: << getContainingNode(this)->getIndex()*/ << " has finished its trip\n";

          numberOfTrip++;
      }
      else targetPosition = slaw->LATP(lastPosition, trip);
      nextChange = simTime() +
                   slaw->computeArrivalTime(lastPosition, targetPosition);
   }
   nextMoveIsWait = !nextMoveIsWait;
}

void SlawMobility::move()
{
   LineSegmentsMobilityBase::move();
   raiseErrorIfOutside();
}

void SlawMobility::saveTrip() {
    std::ofstream ofs("trip.txt");
    if(ofs.is_open()) {
        for (auto it = trip.begin(); it != trip.end(); it++)
            ofs << it->x << " " << it->y << "\n";
        ofs.close();
    }
    else
        std::cerr << "Trip couldn't be written" << '\n';
}

//}
