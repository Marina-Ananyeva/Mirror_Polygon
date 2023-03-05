#pragma once

#include "geometric_objects.h"
#include "constants.h"

#include <vector>
#include <iterator>
#include <set>
#include <algorithm>

using pt = geo_objects::Point;

struct seg {
	pt p, q;
	int id;

    seg() = default;
    seg(pt _p, pt _q, int _id): p(_p), q(_q), id(_id) {
    }
 
	double get_y (double x) const {
		if (std::abs (p.x - q.x) < EPSILON) {
            return p.y;
        }
        if (std::abs(q.x - p.x) < EPSILON) {
            return std::max(p.y, p.x);
        }
        return p.y + (q.y - p.y) * (x - p.x) / (q.x - p.x);
	}
};

inline bool intersect1d(double, double, double, double);

inline int vec(const pt &, const pt &, const pt &);

bool intersect(const seg &, const seg &);

bool operator<(const seg &, const seg &);

struct event {
	double x;
	int tp, id;
 
	event() { }
	event (double x, int tp, int id)
		: x(x), tp(tp), id(id)
	{ }
 
	bool operator< (const event & e) const {
		if (abs (x - e.x) > EPSILON) {
            return x < e.x;
        }
		return tp > e.tp;
	}
};

inline std::set<seg>::iterator prev(std::set<seg>::iterator);

inline std::set<seg>::iterator next(std::set<seg>::iterator);

std::pair<int, int> solve(const std::vector<seg>&);

bool IsPointInsideBoundingBox(geo_objects::Point, geo_objects::Point, geo_objects::Point);

// Точка находится внутри многоугольника
bool IsPointInPolygon(geo_objects::Point, geo_objects::Polygon&);

//bool IsPointInPolygonBinarySearch(geo_objects::Point, geo_objects::Polygon&);
bool IsPointInPolygonBinarySearch();