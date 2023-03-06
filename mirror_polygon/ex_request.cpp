#define _USE_MATH_DEFINES

#include "ex_request.h"

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

namespace request {
double RequestHandler::ComputeAngleBeamFromOnePoint(Polygon & mp, Point a) {
    double angle = 0;
    Point near_normal = near_edge_a.second;
    angle = polarAnglePoints(a, near_normal);
    beam1.push_back(a);
    beam1.push_back(near_normal);
    return angle;
}

double RequestHandler::ComputeAngleBeamFromTwoPoints(Polygon &mp, Point a, Point b) {
    double angle = 0;
    if (!is_e_btw_points) {
        angle = polarAnglePoints(a, b);
        beam2.push_back(a);
        beam2.push_back(b);
    } else {
        double x = (near_edge_ab.second.first.x + near_edge_ab.second.second.x) / 2;
        double y = (near_edge_ab.second.first.y + near_edge_ab.second.second.y) / 2;
        Point centre = Point(x, y);
        angle = polarAnglePoints(a, centre);
        beam2.push_back(a);
        beam2.push_back(centre);
        beam2.push_back(b);
    }
    return angle;
}

bool RequestHandler::ExecuteRequest(ostream& os, Polygon &mp, Point a, Point b) {
    angle1 = ComputeAngleBeamFromOnePoint(mp, a);
    angle2 = ComputeAngleBeamFromTwoPoints(mp, a, b);
    bool is_success_write = OutputResult(os, angle1, angle2);
    return is_success_write;
}
}//namespace request