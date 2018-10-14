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

package inet.mobility.models;

//
// TODO auto-generated module
//
simple Slaw
{
    parameters:
    	string mapName; //= "/home/eandres/Downloads/omnetpp-4.2.2/inet/examples/mobility/maps/2000_1000x1000/2000_1000x1000_0.75_10";
    	double clusteringRadius @unit(m) = default(40m);
    	//CONSTANT = 1, UNIFORM = 2, NORMAL = 3, PARETO_BOUNDED = 4
    	int pauseTimeModel @enum(1, 2, 3, 4) = default(4);
    	double pausetimeA = default(1.36);
    	double pausetimeB @unit(s) = default(30s); 
    	double pausetimeH @unit(s) = default(9054s);
    	//CONSTANT = 1, UNIFORM = 2, NORMAL = 3, RHEE = 4
    	//EAM :: int speedModel @enum(1, 2, 3, 4) = default(3);
    	int speedModel @enum(1, 2, 3, 4) = default(3);
    	double speedA @unit(mps) = default(1.36mps);
    	double speedB @unit(mps) = default(0.19mps);
    	int planningDegree = default(3);
    	//EAM :: @class(Slaw);
}
