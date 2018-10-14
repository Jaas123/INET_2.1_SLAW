//
// Copyright (C) 2005 Georg Lutz, Institut fuer Telematik, University of Karlsruhe
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//


package inet.mobility.models;


//
// Random Waypoint mobility model.
//
// The node moves in line segments. For each line segment, a random destination
// position (distributed uniformly over the constraint area) and a random speed
// is chosen. You can define a speed as a variate from which a new value
// will be drawn for each line segment; it is customary to specify it as
// uniform(minSpeed, maxSpeed). When the node reaches the target position,
// it waits for the time waitTime which can also be defined as a variate.
// After this time the the algorithm calculates a new random position, etc.
//
// This model was written by Georg Lutz (GeorgLutz AT gmx DOT de) for his
// diploma thesis "Effizientes Modell fuer Funkverbindungen in 4G-Netzen fuer
// OMNeT++" (Efficient model for radio links in 4G networks for OMNeT++)
// at Institut fuer Telematik, Universitaet Karlsruhe (Institute for
// Telematics, University Karlsruhe, Germany), 2005-06-21. Slightly modified
// by Andras Varga 2005.06.22.
//
// @author Georg Lutz
//
simple RandomWPMobility extends MovingMobilityBase
{
    parameters:
        double initialX @unit(m) = default(uniform(this.constraintAreaMinX, this.constraintAreaMaxX));
        double initialY @unit(m) = default(uniform(this.constraintAreaMinY, this.constraintAreaMaxY));
        double initialZ @unit(m) = default(uniform(this.constraintAreaMinZ, this.constraintAreaMaxZ));
        bool initFromDisplayString = default(true);
//        volatile double speed @unit(mps) = default(mps); // use uniform(minSpeed, maxSpeed) or another distribution
//        volatile double waitTime @unit(s) = default(0s); // wait time between reaching a target and choosing a new one
//        @class(RandomWPMobility);

		string mapName = "./maps/2000_1000x1000/2000_1000x1000_0.75_10";
    	double clusteringRadius @unit(m) = default(40m);
    	//CONSTANT = 1, UNIFORM = 2, NORMAL = 3, PARETO_BOUNDED = 4
    	int pauseTimeModel @enum(1, 2, 3, 4) = default(4);
    	double pausetimeA = default(1.36);
    	double pausetimeB @unit(s) = default(30s); 
    	double pausetimeH @unit(s) = default(9054s);
    	//CONSTANT = 1, UNIFORM = 2, NORMAL = 3, RHEE = 4
    	int speedModel @enum(1, 2, 3, 4) = default(3);
    	double speedA @unit(mps) = default(1.36mps);
    	double speedB @unit(mps) = default(0.19mps);
    	int planningDegree = default(3);
    	
    	volatile double speed @unit(mps) = default(mps); // use uniform(minSpeed, maxSpeed) or another distribution
    	volatile double waitTime @unit(s) = default(0s); // wait time between reaching a target and choosing a new one
    	@class(RandomWPMobility);

}
