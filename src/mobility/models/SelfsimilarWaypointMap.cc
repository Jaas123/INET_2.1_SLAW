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

#include "SelfsimilarWaypointMap.h"

//namespace inet{

//The input is the map name and the clustering radius
SelfsimilarWaypointMap::SelfsimilarWaypointMap(std::string& name,double radius) {
    mapName = name;
    clusteringRadius = radius;
    areaVector = new std::vector<Area>;
    weightVector = new std::vector<double>;
    if(!loadAreaVector()) {
        std::cout << "Load selfsimilar waypoint map from "
                  << mapName << std::endl;
        std::list<Coord> waypointList;
        if(loadMap(waypointList)) {
            computeConfinedAreas(waypointList);
            computeAreaWeights();
            saveAreaVector();
        }
        else std::cerr << "Map not loaded\n";
    }
}

SelfsimilarWaypointMap::~SelfsimilarWaypointMap() {
    if(areaVector)
        delete areaVector;
    if(weightVector)
        delete weightVector;
}

bool SelfsimilarWaypointMap::loadMap(std::list<Coord>& waypointList) {
    std::string filename(mapName + ".map");
    std::ifstream waypointFile(filename, std::ifstream::in);
    bool success = false;
    if(waypointFile.is_open()) {
        Coord waypoint;
        while (!waypointFile.eof()) {
            waypointFile >> waypoint.x >> waypoint.y;
            waypointList.push_back(waypoint);
        }
        waypointList.pop_back();
        numberOfWaypoints = waypointList.size();
        std::cout << numberOfWaypoints << " waypoints have been read from "
        << filename << std::endl;
        waypointFile.close();
        success = true;
    }
    else std::cerr << "Erroneous filename, perhaps you added an extension\n";
    return success;
}

void SelfsimilarWaypointMap::computeConfinedAreas(std::list<Coord>& waypointList) {
    std::cout << "Computing confined areas... it may take some minutes" << std::endl;
    unsigned numOfWaypoints = 0;
    Area confinedArea; //confined area
    while(!waypointList.empty()) {
        Coord popularWaypoint = getPopularWaypoint(waypointList);
        auto it = waypointList.begin();
        while(it != waypointList.end()) {
            if(popularWaypoint.distance(*it) <= clusteringRadius) {
                confinedArea.push_back(*it);
                numOfWaypoints++;
                waypointList.erase(it++);
            }
            else it++;
        }
        areaVector->push_back(confinedArea);
        std::cout << confinedArea.size() << " waypoints have been clustered\n";
        confinedArea.clear();
    }
    std::cout << numOfWaypoints << " waypoints have been clustered in "
              << areaVector->size() << " confined areas" << std::endl;
}

Coord
SelfsimilarWaypointMap::getPopularWaypoint(std::list<Coord>& waypointList) {
    Coord popularWaypoint(0.0, 0.0);
    uint16_t maximum = 0;
    for(Coord& w : waypointList) {
        uint16_t surroundingWaypoints = 0;
        for(Coord& v : waypointList)
            if((w.distance(v) < clusteringRadius) && (w != v))
                surroundingWaypoints++;
        if(surroundingWaypoints >= maximum) {
            popularWaypoint = w;
            maximum = surroundingWaypoints;
        }
    }
    return popularWaypoint;
}

bool SelfsimilarWaypointMap::saveAreaVector() {
    bool result = false;
    std::string filename = mapName+"_cl"+ boost::lexical_cast<std::string>(int(clusteringRadius)) +".dat";
    std::ofstream ofs(filename.c_str(), std::ofstream::out);
    if(ofs.is_open()) {
        for(auto& area : *areaVector) {
            for(Coord& waypoint : area)
                ofs << waypoint.x << ' ' << waypoint.y << ' ';
            ofs << '\n';
        }
        ofs.close();
        result = true;
    }
    return result;
}

bool SelfsimilarWaypointMap::loadAreaVector() {
    bool result = false;
    std::string filename = mapName+"_cl"+ boost::lexical_cast<std::string>(int(clusteringRadius)) +".dat";
    std::ifstream ifs(filename.c_str(), std::ifstream::in);
    if(ifs.good()) {
        uint16_t waypointCounter = 0;
        Coord waypoint;
        std::vector<Coord> waypointCluster;
        std::string line;
        std::stringstream ss;
        while(std::getline(ifs, line)) { //Gets a line while they exist
            ss << line;
            while(ss >> waypoint.x >> waypoint.y) {
                waypointCounter++;
                waypointCluster.push_back(waypoint);
            }
            areaVector->push_back(waypointCluster);
            waypointCluster.clear();
            line.clear();
            ss.clear();
        }
        numberOfWaypoints = waypointCounter;
        computeAreaWeights();
        result = true;
        std::cout << areaVector->size()
                  << " confined areas have been read" << std::endl;
    }
    return result;
}

void SelfsimilarWaypointMap::computeAreaWeights() {
    double weight = 0.0;
    weightVector->push_back(weight);
    for(Area& area : *areaVector) { //EAM :: auto cast explícito
        weight += (double)area.size()/numberOfWaypoints;
        weightVector->push_back(weight);
    }
}

const Area* SelfsimilarWaypointMap::getConfinedArea(unsigned index) {
    return static_cast<const Area*>(&areaVector->at(index));
}

const std::vector<double>* SelfsimilarWaypointMap::getAreaWeights() {
    return static_cast<const std::vector<double>*>(weightVector);
}

int SelfsimilarWaypointMap::getAreaSize(unsigned index) {
    int size = -1;
    if (areaVector && (index < areaVector->size()))
        size = unsigned(areaVector->at(index).size());
    return size;
}

int SelfsimilarWaypointMap::getNumberOfAreas() {
    int size = -1;
    if (areaVector)
        size = int(areaVector->size());
//    printf("[***] Numero de áreas :: %d\n", size);
    return size;
}

Coord SelfsimilarWaypointMap::getWaypoint(unsigned indexArea, unsigned index) {
    Coord waypoint(-1.0, -1.0);
    if (indexArea < areaVector->size())
        if (index < areaVector->at(indexArea).size())
            waypoint = areaVector->at(indexArea).at(index);
    return waypoint;
}
//}
