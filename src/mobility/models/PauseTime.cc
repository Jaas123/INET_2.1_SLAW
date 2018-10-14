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

#include "PauseTime.h"

//namespace inet{


double PauseTime::pareto_bounded() {
    double alpha = boost::get<0>(parameters);
    double L = boost::get<1>(parameters);
    double H = boost::get<2>(parameters);
    //EAM :: printf("Valor :: %lu \n",(unsigned long)rng);
    //By default this is generator 0, but you can specify which one to used.
    double u = uniform( 0, 1);
    //EAM :: double u = 1; //EAM :: Corregir !!!
    double op1 = pow(H, alpha);
    double op2 = pow(L, alpha);
    //(-(UH^a - UL^a - H^a)/(H^a * L^a))^(-1/a) where U is random 0<=U<=1
    return pow(-(u*op1 - u*op2 - op1) / pow(H*L, alpha), -1.0/alpha);
}

void PauseTime::setPauseTimeModel(PauseTimeModel m) {
   model = m;
}

void PauseTime::setConstantModel(double value) {
    boost::get<0>(parameters) = value;
}

void PauseTime::setNormalModel(double mean, double stddev) {
    boost::get<0>(parameters) = mean;
    boost::get<1>(parameters) = stddev;
}

void PauseTime::setUniformModel(double a, double b) {
    boost::get<0>(parameters) = a;
    boost::get<1>(parameters) = b;
}

void PauseTime::setParetoBoundedModel(double alpha, double L, double H) {
    boost::get<0>(parameters) = alpha;
    boost::get<1>(parameters) = L;
    boost::get<1>(parameters) = H;
}

double PauseTime::computePausetime() {
    double pausetime;
    switch(model) {
        case PauseTimeModel::CONSTANT: pausetime = boost::get<0>(parameters);
            break;
        case PauseTimeModel::UNIFORM:
             //EAM :: pausetime = uniform(boost::get<0>(parameters),boost::get<1>(parameters),(unsigned long)rng);
            pausetime = uniform(boost::get<0>(parameters),boost::get<1>(parameters));
            //EAM :: pausetime = pareto_bounded();
            break;
        case PauseTimeModel::NORMAL:
            //EAM :: pausetime = truncnormal(boost::get<0>(parameters),boost::get<1>(parameters),(unsigned long)rng);
            pausetime = truncnormal(boost::get<0>(parameters),boost::get<1>(parameters));
            //EAM :: pausetime = pareto_bounded();
            break;
        default:
            pausetime = pareto_bounded();
            break;
    }
    return pausetime;
}
//}
