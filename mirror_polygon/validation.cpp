#include "validation.h"

#include <algorithm>
#include <set>
#include <unordered_set>

using namespace std;

namespace in_reader{
void CheckPolygonSize (int size) {
    if (size < 3) {
        throw ValidityError(" - the number of vertexes cannot be less than three"s);
    }
}

void PolygonPrepare(Polygon & mp, Point a, Point b, vector<seg>& v, request::RequestHandler& rh) {
    rh.p_min.SetPoint(MAX_DOUBLE, MAX_DOUBLE);
    rh.p_max.SetPoint(MIN_DOUBLE, MIN_DOUBLE);
    rh.zero_pt = make_pair(Point(MAX_DOUBLE, MAX_DOUBLE), -1);
    rh.is_convex = mp.IsPolygonConvex();
    rh.near_edge_a = make_pair(MAX_DOUBLE, Point(0.0, 0.0));
    rh.near_edge_ab = make_pair(MAX_DOUBLE, make_pair(Point(0.0, 0.0), Point(0.0, 0.0)));
    for (int i = 0; i < mp.size(); i++, mp.advance(1)) {
        Point p1 = mp.v()->point();
        Point p2 = mp.cw()->point();
        v[i] = seg(p1, p2, i);
        rh.vertexes.push_back(Vertex(p1));
        //поиск min max 
        if (p1.x > rh.p_max.x) {
            rh.p_max.x = p1.x;
        } 
        if (p1.x < rh.p_min.x) {
            rh.p_min.x = p1.x;
        }
        if (p1.y > rh.p_max.y) {
            rh.p_max.y = p1.y;
        }
        if (p1.y < rh.p_min.y) {
            rh.p_min.y = p1.y;
        }
        //Поиск вершины зеро
        if (p1 < rh.zero_pt.first) {
            rh.zero_pt = make_pair(p1, i);
        }
        //Поиск ближайших точек на ребре к начальной точке и к начальной и конечной точкам
        pair<bool, Point> normal_a = a.distance_normal(Edge(p1, p2));
        if (normal_a.first && normal_a.second != p1 && normal_a.second != p2) {
            long double dist = a.distance(normal_a.second);
            if (dist < rh.near_edge_a.first) {
                rh.near_edge_a = make_pair(dist, normal_a.second);
            }
            pair<bool, Point> normal_b = b.distance_normal(Edge(p1, p2));
            if (normal_b.first && normal_b.second != p1 && normal_b.second != p2) {
                dist -= b.distance(normal_b.second);
                if (abs(dist) < rh.near_edge_ab.first && normal_a.second != normal_b.second) {
                    rh.near_edge_ab = make_pair(abs(dist), make_pair(normal_a.second, normal_b.second));
                }
            }
        }
    }
}

void CheckPolygonCoordinatesUnique(vector<seg> v) {
    auto it = unique(v.begin(), v.end());
    if (it != v.end()) {
        throw ValidityError(" - coordinates are repeated"s);
    }
}

void CheckPolygonCoordinatesNotIntersecting(const vector<seg>& v, bool& is_e_btw_points) {
    pair<int, int> validation_res = IsPolygonIntersected(v, is_e_btw_points);
    if (validation_res != make_pair(-1, -1)) {
        throw ValidityError(" - polygon must not intersect"s);
    }
}

void CheckPointInsidePolygon(Polygon& mp, Point a, Point b, request::RequestHandler& rh) {
    bool is_inside = IsPointInsideBoundingBox(a, rh.p_min, rh.p_max);
    if (is_inside) {
        bool is_inside = IsPointInsideBoundingBox(b, rh.p_min, rh.p_max);
        if (is_inside) {
            if (!rh.is_convex) {
                is_inside = IsPointInPolygon(a, mp);
                if (is_inside) {
                    is_inside = IsPointInPolygon(b, mp);
                } else {
                    throw ValidityError(" - the starting point is not inside polygon"s);
                }
                if (!is_inside) {
                    throw ValidityError(" - the endpoint is not inside polygon"s);
                }
            } else {
                is_inside = IsPointInPolygonBinarySearch(rh.vertexes, rh.zero_pt, a);
                if (is_inside) {
                    is_inside = IsPointInPolygonBinarySearch(rh.vertexes, rh.zero_pt, b);
                } else {
                    throw ValidityError(" - the starting point is not inside polygon"s);
                }
                if (!is_inside) {
                    throw ValidityError(" - the endpoint is not inside polygon"s);
                }
            }
        } else {
            throw ValidityError(" - the endpoint is not inside polygon"s);
        }
    } else {
        throw ValidityError(" - the starting point is not inside polygon"s);
    }
}

void PolygonValidation(Polygon &mp, Point a, Point b, request::RequestHandler& rh) {
    int size = static_cast<int>(mp.size());
    vector<seg> v(size);
    PolygonPrepare(mp, a, b, v, rh);

    CheckPolygonCoordinatesUnique(v);

    v.push_back(seg(a, b, mp.size())); //добавляем отрезок между заданными точками (для проверки пересекается ли он с ребром)
    CheckPolygonCoordinatesNotIntersecting(v, rh.is_e_btw_points);

    CheckPointInsidePolygon(mp, a, b, rh);
}
} //namespace in_reader