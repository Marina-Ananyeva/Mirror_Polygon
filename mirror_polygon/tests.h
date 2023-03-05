#pragma once

#include "geometric_objects.h"
#include "validation.h"
#include "calculations.h"
#include "ex_request.h"
#include "visualization.h"

#include <assert.h>

void TestCheckPolygonCoordinatesNotIntersecting();

void TestCheckIsPolygonConvex();

void TestCheckPointInsidePolygon();

void TestComputeAngleBeamFromTwoPoints();

void TestBeamsVisualization();

void Tests();
