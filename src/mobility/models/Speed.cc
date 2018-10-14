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

#include "Speed.h"

//namespace inet{

Speed::Speed(cRNG* generator, SpeedModel m, double a, double b) {
   rng = generator;
   model = m;
   speedParameters.first = a;
   speedParameters.second = b;
}

double Speed::rheeModel(double flightLength) {
    double speed;
    if(flightLength < 500.0)
        speed = flightLength/(30.55*pow(flightLength, 1-0.89));
    else
        speed = flightLength/(0.76*pow(flightLength, 1-0.28));
    return speed;
}

void Speed::setSpeedModel(SpeedModel m) {
    model = m;
}

void Speed::setConstantModel(double a) {
    speedParameters.first = a;
    speedParameters.second = 0.0;
}

void Speed::setUniformModel(double a, double b) {
    speedParameters.first = a;
    speedParameters.second = b;
}

void Speed::setNormalModel(double mean, double stddev) {
    speedParameters.first = mean;
    speedParameters.second = stddev;
}

double Speed::computeSpeed(double flightLength) {
    double speed;
    switch(model) {
        case SpeedModel::CONSTANT: speed = speedParameters.first; break;
        case SpeedModel::UNIFORM:
            //EAM :: printf("Valor 1 [v4.2]:: %f \n",speedParameters.first);
            //EAM :: printf("Valor 2 [v4.2]:: %f \n",speedParameters.second);
            //EAM :: printf("RNG [v4.2]:: %lu \n",(unsigned long)rng);
            //By default this is generator 0, but you can specify which one to used.
            speed = uniform(speedParameters.first,speedParameters.second);
            //EAM :: speed = rheeModel(flightLength); //EAM :: Prueba
            break;
        case SpeedModel::NORMAL:
            //EAM :: speed = rheeModel(flightLength);
            //EAM :: printf("Valor 1[v4.2] :: %f \n",speedParameters.first);
            //EAM :: printf("Valor 2[v4.2] :: %f \n",speedParameters.second);
            //EAM :: printf("RNG [v4.2]:: %lu \n",(unsigned long)rng);
            //By default this is generator 0, but you can specify which one to used.
            speed = truncnormal(speedParameters.first,speedParameters.second);
            break;
        default:
            speed = rheeModel(flightLength);
            break;
    }
    return speed;
}

SpeedModel Speed::getSpeedModel() {
    return model;
}
//}
