#include "validation.h"

using namespace std;

namespace in_reader{
bool CheckPolygonCoordinatesUniqueValidity(const vector<pair<double, double>>& coordinates) {
    return true;
}

bool CheckPolygonCoordinatesOrderValidity(const vector<pair<double, double>>& coordinates) {
    return true;
}

bool CheckPolygonCoordinatesNotIntersectingValidity(const vector<pair<double, double>>& coordinates) {
    return true;
}

bool CheckPointInsidePolygonValidity(double, double) {
    return true;
}
} //namespace in_reader