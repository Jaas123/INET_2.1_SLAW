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

#ifndef SLAWBASE_H_
#define SLAWBASE_H_

#include <algorithm>
#include <cmath>
#include <fstream>
#include <boost/ptr_container/ptr_list.hpp>
#include <omnetpp.h>

#include "SelfsimilarWaypointMap.h"
#include "Speed.h"
#include "PauseTime.h"


#include "INETDefs.h"

//namespace inet{

/** @brief List to store the waypoints integrating the trip of a walker. A list
 *  is employed to store represents the trip due to the fact that it is
 *  necessary to remove a waypoint once LATP used it as next destination. */
typedef std::list<Coord> Trip;
typedef boost::ptr_list<Coord> Trip_;

/** @brief Vector used to stored the confined areas of a walker. According to
 *  the authors' SLAW, this is denoted by C_k. */
typedef std::vector<unsigned int> AreaSet;


class INET_API SlawBase : public cSimpleModule {
protected:
        SelfsimilarWaypointMap* map;
        Speed* speed;
        PauseTime* pausetime;
        /** @brief Adds the 10 percent of waypoints of a random-elected
         *  confined area to the trip of a walker */
        virtual void computeTripRandomness(Trip&, const AreaSet&, Coord&);
        /** @brief Adds waypoints to the trip of a walker until reaching a number
         *  between 120 and 150 */
        virtual void computeRutine(Trip&, const AreaSet&, Coord&);
        /** @brief Randomly, assigns a waypoint from a confined area. The area
         * vector is passed as formal parameter to do the assignment. */
        virtual Coord computeHome(const AreaSet&);
        /** @brief Computes the confined areas of a walker. The areas are
         *  determined by their weight, which is computed by dividing their
         *  number of waypoints into the number of waypoints integrating the map */
        virtual void computeConfinedAreas(AreaSet& areaVector);
public:
        SlawBase();
    /** @brief Default destructor. */
        template<typename T>
        std::vector<T>& randomizeVector(std::vector<T>& v);
        virtual ~SlawBase();
};

template<typename T>
std::vector<T>& SlawBase::randomizeVector(std::vector<T>& v) {
    for(unsigned i = 0; i < v.size(); i++) {
        unsigned rnd = intuniform(0, unsigned(v.size())-1);
        std::swap(v[i],v[rnd]);
    }
    return v;
}
//}
#endif /* SLAWBASE_H_ */
