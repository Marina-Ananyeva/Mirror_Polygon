#pragma once

#include "calculations.h"
#include "geometric_objects.h"
#include "log_duration.h"

#include <stdexcept>
#include <string>
#include <vector>

namespace in_reader{
class ValidityError : public std::logic_error {
public:
    ValidityError(const std::string& error_text) : std::logic_error(MakeErrorMessage(error_text)) {
    }
    std::string MakeErrorMessage(const std::string& error_text) {
        using namespace std::literals;
        return "\nВходные данные некорректны"s + error_text;
    }
};

void CheckPolygonSize(int);

void CheckPolygonCoordinatesUnique(const std::vector<geo_objects::Point>&);

void CheckPolygonCoordinatesNotIntersecting(const std::vector<geo_objects::Point>&);

void CheckPointInsidePolygon(geo_objects::Point, geo_objects::Point);
} //namespace in_reader