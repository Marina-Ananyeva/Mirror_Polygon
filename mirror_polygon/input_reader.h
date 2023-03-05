#pragma once

#include "geometric_objects.h"
#include "log_duration.h"
#include "validation.h"

namespace in_reader{
void ReadFromConsole(std::istream&, geo_objects::Polygon&, geo_objects::Point&, geo_objects::Point&);

void ReadFromFile(std::istream&, geo_objects::Polygon&, geo_objects::Point&, geo_objects::Point&);

bool ReadQuery(std::istream&, geo_objects::Polygon&, geo_objects::Point&, geo_objects::Point&);
} //namespace in_reader
