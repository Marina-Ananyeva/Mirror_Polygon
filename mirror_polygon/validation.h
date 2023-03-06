#pragma once

#include "calculations.h"
#include "geometric_objects.h"
#include "ex_request.h"
#include "log_duration.h"

#include <stdexcept>
#include <string>
#include <vector>

namespace in_reader{
using namespace geo_objects;
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

void PolygonPrepare(Polygon &, Point, Point, std::vector<seg>&, request::RequestHandler&);

void CheckPolygonCoordinatesUnique(std::vector<seg> v);

void CheckPolygonCoordinatesNotIntersecting(const std::vector<Point>&, bool&);

void CheckPointInsidePolygon(Polygon&, Point, Point, request::RequestHandler&);

void PolygonValidation(Polygon &, Point, Point, request::RequestHandler&);
} //namespace in_reader