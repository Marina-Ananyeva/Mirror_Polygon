#pragma once

#include "geometric_objects.h"
#include "ex_request.h"
#include "log_duration.h"
#include "validation.h"

namespace in_reader{
using namespace geo_objects;
void ReadFromConsole(std::istream&, Polygon&, Point&, Point&, request::RequestHandler&);

void ReadFromFile(std::istream&, Polygon&, Point&, Point&, request::RequestHandler&);

bool ReadQuery(std::istream&, Polygon&, Point&, Point&, request::RequestHandler&);
} //namespace in_reader
