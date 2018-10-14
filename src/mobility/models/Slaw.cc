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

#include "Slaw.h"
#include <boost/ptr_container/ptr_list.hpp>
//namespace inet {

Define_Module(Slaw);

Slaw::Slaw() {
    //EAM :: printf("Iniciando modelo SLAW \n");
    map = 0;//EAM :: nullptr;
    speed = 0; //EAM :: nullptr;
    pausetime = 0; //EAM ::  nullptr;
    a = 0;
    isInitialized = false;

}


Slaw::~Slaw() {
    if(map) {
        delete map;
        std::cout << "Destroying map: OK" << '\n';
    }
    if(speed) {
        delete speed;
        std::cout << "Destroying speed model: OK" << '\n';
    }
    if(pausetime) {
        delete pausetime;
        std::cout << "Destroying pausetime model: OK" << '\n';
    }
    std::cout << "SLAW module was destroyed" << '\n';
}

Coord Slaw::LATP(Coord& currentPosition, Trip& walkerTrip) {
//        Trip_& walkerTrip_;
//        Trip_::const_iterator nextWaypointIt_ = walkerTrip_.begin();
//        walkerTrip_.erase(nextWaypointIt_);

        Trip::const_iterator nextWaypointIt = walkerTrip.begin();
        std::vector<double> distanceVector, cdf;
        double probabilityFactor = 0.0, cumulativeProbability = 0.0;
        //Compute distance to each one of all unvisited waypoints, which are stored
        //in the trip walker
        for (Coord& waypoint : walkerTrip) { //EAM :: Cast explícito, "auto"
            double distance = currentPosition.distance(waypoint);
            if (distance <= 0)
                error("Invalid distance between waypoints %lf, %lf and %lf, %lf",
                       currentPosition.x, currentPosition.y,
                       waypoint.x, waypoint.y);


            probabilityFactor += pow(1/distance, a);
            distanceVector.push_back(1/distance);
        }
        //Compute probability to move to all unvisited waypoints.
        for (double& distance : distanceVector) { //EAM :: Cast explícito, "auto"
            cumulativeProbability += pow(distance, a) / probabilityFactor;
            cdf.push_back(cumulativeProbability);
        }
        //Choose the next waypoint according to the probability vector
        double rnd = uniform(0, 1);
        auto it = std::upper_bound(cdf.begin(), cdf.end(), rnd);
        std::advance(nextWaypointIt, std::distance(cdf.begin(), it));

        Coord nextWaypoint = *nextWaypointIt;
        if(nextWaypointIt == walkerTrip.end())
            error("SLAW: Trying to delete an invalid pointer");
        //walkerTrip.erase(nextWaypointIt);
        //[Candidato] :: std::_List_iterator<Coord> erase(std::_List_iterator<Coord>)
        //EAM walkerTrip.erase(nextWaypointIt); //EAM ???
        walkerTrip.remove(*nextWaypointIt);
        return nextWaypoint;
}

void Slaw::initialize() {

        initializeMap();
        initializePauseTimeModel();
        initializeSpeedModel();
        a = par("planningDegree");
        isInitialized = true;
        std::cout << "Slaw has been initialized\n";
}

void Slaw::initializeMap() {
        std::string mapName = (par("mapName").stringValue());
        double clusteringRadius = par("clusteringRadius");
        map = new SelfsimilarWaypointMap(mapName, clusteringRadius);
}

void Slaw::initializePauseTimeModel() {
        PauseTimeModel pauseModel = static_cast<PauseTimeModel>(uint8_t(par("pauseTimeModel")));
        double par1 = par("pausetimeA");
        double par2 = par("pausetimeB");
        double parH = par("pausetimeH");
        //printf("RNG valor (PauseTimeModel) :: %d\n",getRNG(0));
        EV << getRNG(0)->info();
        pausetime = new PauseTime(getRNG(0), pauseModel, par1, par2, parH);
}

void Slaw::initializeSpeedModel() {
        SpeedModel model = static_cast<SpeedModel>(uint8_t(par("speedModel")));
        double par1 = par("speedA");
        double par2 = par("speedB");
        //cout << "[++] RNG valor (SpeedModel) :: " << getRNG(0)->info());
        EV << getRNG(0)->info();
        speed = new Speed(getRNG(0), model, par1, par2);
}

void Slaw::initializeMobilityState(Trip& trip, AreaSet& C_k, Coord& home) {
        computeConfinedAreas(C_k);
        home = computeHome(C_k);
        computeTrip(trip, C_k, home);
}

void Slaw::computeTrip(Trip& walkerTrip, const AreaSet& C_k, Coord& home) {
        computeTripRandomness(walkerTrip, C_k, home);
        computeRutine(walkerTrip, C_k, home);
}

double Slaw::computePauseTime() {
        return pausetime->computePausetime();
}

double Slaw::computeArrivalTime(const Coord& w, const Coord& v) {
    double distance = w.distance(v);
    return distance/speed->computeSpeed(distance);
}


//}
