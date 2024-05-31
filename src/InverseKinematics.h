#ifndef INVERSEKINEMATICS_H
#define INVERSEKINEMATICS_H

#include <iostream>
#include <eigen3/Eigen/Dense>
#include <cmath>
#include <fstream>

using namespace std;
using namespace Eigen;

class Ankle_Parallel_Mechanism{
public:
    Ankle_Parallel_Mechanism();
    ~Ankle_Parallel_Mechanism();
    void get_angle(double ankle_q1, double ankle_q2, double ankle_ql, double ankle_qr);
    void get_file_data();
private:
    double N1, N2, N3, N4, N5, L1, L2, L3, L4;
    double theta_l, theta_r;
};

class Poi_Move{
public:
    Poi_Move();
    ~Poi_Move();
    double poi_move(double poi_start, double poi_end, double speed, double time_step);
private:

};

#endif//INVERSEKINEMATICS_H