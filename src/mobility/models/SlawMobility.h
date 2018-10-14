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

#ifndef __INET_SLAWMOBILITY_H_
#define __INET_SLAWMOBILITY_H_

#include <omnetpp.h>

#include <cstdint>
#include "Slaw.h"
#include "INETDefs.h"
#include "LineSegmentsMobilityBase.h"

//namespace inet{

/**
 * TODO - Generated class
 */
class INET_API SlawMobility : public LineSegmentsMobilityBase
{
protected:
    /** @brief Boolean value used to switch between these states {pause, motion} */
    bool nextMoveIsWait;
    /** @brief Number of trips a walker has performed. */
    unsigned numberOfTrip;
    /** @brief Pointer to the SLAW module, it is used to access the selfsimilar
     *  waypoint map and it computes the next destination using LATP. */
    Slaw* slaw;
    /** @brief Name of the module executing Slaw. It is defined in a ned file. Its
     *  default value is merely "slaw". */
    std::string slawModuleName;
    /** @brief Vector storing the sets of confined areas. */
    AreaSet areas;
    /** @brief List storing the set of waypoints integrating a trip. */
    Trip trip;
    /** @brief Home waypoint. All SLAW trips start and end in this waypoint. */
    Coord home;
    /** @brief Pause time a node has spent in a waypoint.*/
protected:
    /** Number of states used to initialize every module derived from
     *  MobilityBase. This module is initialized in the state 0. It is very
     *  important place it below the slaw model in he ned file in order
     *  to avoid the error 139: segmentation fault. */
    virtual int numInitStages() const { return 3; }
    /** @brief Initializes mobility model parameters.*/
    virtual void initialize(int stage);
    /** @brief Overridden from MobilityBase.*/
    virtual void setInitialPosition();
    /** @brief Overridden from LineSegmentsMobilityBase.*/
    virtual void setTargetPosition();
    /** @brief Overridden from LineSegmentsMobilityBase.*/
    virtual void move();
public:
    /** @brief Default constructor. It initializes the motion status and the
     *  value of the slaw pointer. */
    SlawMobility();
    /** @brief Saves the trip the module SLAW assigned to the walker. Used
     *  for debugging. */
    virtual void saveTrip();
    /** @brief Returns the number of trips a walker has performed. */
    virtual unsigned getNumberOfTrips() { return numberOfTrip; }
    /** @brief Returns the home waypoint. */
    virtual Coord getHome() {return home;}
    /** @brief Returns true if the walker is in motion, otherwise returns false. */
    virtual bool getState() {return !nextMoveIsWait;}
    /** @brief Returns a const pointer to the mobility areas of a node. */
    virtual const AreaSet* getAreaSet(){return static_cast<const AreaSet*>(&areas);}
    /** @brief Returns a const pointer to the data structure storing the walker trip. */
    virtual const Trip* getTrip(){return static_cast<const Trip*>(&trip);}
};

//}
#endif
