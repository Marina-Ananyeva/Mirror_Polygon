#pragma once

#include <vector>

namespace in_reader{
bool CheckPolygonCoordinatesUniqueValidity(const std::vector<std::pair<double, double>>&);

bool CheckPolygonCoordinatesOrderValidity(const std::vector<std::pair<double, double>>&);

bool CheckPolygonCoordinatesNotIntersectingValidity(const std::vector<std::pair<double, double>>&);

bool CheckPointInsidePolygonValidity(double, double);
} //namespace in_reader