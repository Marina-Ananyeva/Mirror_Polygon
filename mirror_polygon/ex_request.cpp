#define _USE_MATH_DEFINES

#include "ex_request.h"

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

/*
    std::vector<svg::Point> vertexes(size + 1);
    for (int i = 0; i < size + 1; ++i) {
        geo_objects::Point p_v = mp.point();
        svg::Point p(p_v.x, p_v.y);
        vertexes[i] = p;
        mp.advance(1);
    }
*/

double ComputeAngleBeamFromTwoPoints(geo_objects::Point a, geo_objects::Point b) {
    //double cos = (a.x * b.x + a.y * b.y) / (sqrt(pow(a.x, 2) + pow(a.y, 2)) * sqrt(pow(b.x, 2) + pow(b.y, 2)));
    //double angle = acos(cos) * 180.0 / M_PI;
    double angle = polarAnglePoints(a, b);
    //cout << fixed << setprecision(3) << angle << '\n';
    return angle;
}