#include "calculations.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>

using namespace std;

bool intersect1d (double l1, double r1, double l2, double r2) {
	if (l1 > r1) {
        swap (l1, r1);
    }
	if (l2 > r2) {
        swap (l2, r2);
    }
	return max(l1, l2) <= min(r1, r2) + EPSILON;
}

int vec(const pt& a, const pt& b, const pt& c) {
	double s = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
	return abs(s) < EPSILON ? 0 : s > 0 ? +1 : -1;
}
 
bool intersect(const seg& a, const seg& b) {
	return intersect1d (a.p.x, a.q.x, b.p.x, b.q.x)
		&& intersect1d (a.p.y, a.q.y, b.p.y, b.q.y)
		&& vec (a.p, a.q, b.p) * vec (a.p, a.q, b.q) <= 0
		&& vec (b.p, b.q, a.p) * vec (b.p, b.q, a.q) <= 0;
}

bool operator<(const seg& a, const seg& b) {
    double x = max(min (a.p.x, a.q.x), min(b.p.x, b.q.x));
    return a.get_y(x) < b.get_y(x) - EPSILON;
}

bool operator==(const seg& a, const seg& b) {
    return a.p == b.p && a.q == b.q && a.id == b.id;
}

bool operator!=(const seg& a, const seg& b) {
    return !(a == b);
}

multiset<seg> s;
vector <set<seg>::iterator> where;

multiset<seg>::iterator prev(multiset<seg>::iterator it) {
	return it == s.begin() ? s.end() : --it;
}

multiset<seg>::iterator next(multiset<seg>::iterator it) {
	return ++it;
}

pair<int, int> IsPolygonIntersected (const vector<seg>& a, bool& is_e_btw_points) {
    multiset<seg>& s1 = s;
    vector <multiset<seg>::iterator>& where1 = where;
	int n = (int)a.size();
	vector<event> e;
	for (int i = 0; i < n; ++i) {
		e.push_back(event (min (a[i].p.x, a[i].q.x), +1, i));
		e.push_back(event (max (a[i].p.x, a[i].q.x), -1, i));
	}
	sort(e.begin(), e.end());
 
	s.clear();
	where.resize(a.size());
	for (size_t i = 0; i < e.size(); ++i) {
		int id = e[i].id;
		if (e[i].tp == +1) {
            multiset<seg>::iterator nxt = s.lower_bound(a[id]);
            multiset<seg>::iterator prv = prev(nxt);
			if (nxt != s.end() && intersect(*nxt, a[id])) {
                if (nxt->id == n - 1 || id == n - 1) {
                    is_e_btw_points = true;                                         
                } else if (abs(nxt->id - id) != 1 && abs(nxt->id - id) != n - 2) {//проверяем, что не соседние ребра
                    return make_pair (nxt->id, id);
                }
            }
			if (prv != s.end() && intersect(*prv, a[id])) {
                if (prv->id == n - 1|| id == n - 1) {
                    is_e_btw_points = true;
                } else if (abs(prv->id - id) != 1 && abs(prv->id - id) != n - 2) {//проверяем, что не соседние ребра
                    return make_pair (prv->id, id);
                }
            }
            where[id] = s.insert(nxt, a[id]);
		}
		else {
            multiset<seg>::iterator nxt = next(where[id]);
            multiset<seg>::iterator prv = prev(where[id]);
            if (nxt != s.end() && prv != s.end() && intersect(*nxt, *prv)) {
                if (nxt->id == n - 1|| prv->id == n - 1) {
                    is_e_btw_points = true;
                } else if(abs(nxt->id - prv->id) != 1 && abs(nxt->id - prv->id) != n - 2) {//проверяем, что не соседние ребра
                    return make_pair (prv->id, nxt->id);
                }
            }
			s.erase (where[id]);
		}
	}
	return make_pair (-1, -1);
}

bool IsPointInsideBoundingBox(geo_objects::Point p, geo_objects::Point p_min, geo_objects::Point p_max) {
        if (p.x < p_min.x || p.x > p_max.x || p.y < p_min.y || p.y > p_max.y ) {
            return false;
        }
        return true;
    }

bool IsPointInPolygon(geo_objects::Point p, geo_objects::Polygon &mp) {
    bool isInside = false;
    int count = 0;

    double minX = DBL_MAX;
    for (int i = 0; i < mp.size(); i++, mp.advance(1)) {
        minX = min(minX, mp.v()->x);
    }

    geo_objects::Point p0(p.x, p.y);
    geo_objects::Point p1(p.x, p.y);
    geo_objects::Point p2(minX - 10, p.y);// Принимаем наименьшее значение X в качестве конечной точки луча

    // Обходим каждую сторону
    for (int i = 0; i < mp.size(); i++, mp.advance(1)) {
        geo_objects::Point c1(mp.v()->x, mp.v()->y);
        geo_objects::Point c2(mp.cw()->x, mp.cw()->y);

        if (geo_objects::IsPointOnLine(p0, c1, c2)) {
            return true;
        }
        if (fabs (c2.y - c1.y) < EPSILON) { // Параллельно, не пересекаются
            continue;
        }
        if (geo_objects::IsPointOnLine(c1, p1, p2)) {
            if (c1.y > c2.y) {// Гарантируем только верхнюю конечную точку +1
                count++;
            }
        } else if (geo_objects::IsPointOnLine(c2, p1, p2)) {
            if (c2.y > c1.y) {// Гарантируем только верхнюю конечную точку +1
                count++;
            }
        } else if (geo_objects::IsIntersect(c1, c2, p1, p2)) {// Параллельная ситуация исключена
            count++;
        }
    }

    if (count % 2 == 1) {
        isInside = true;
    }
 
    return isInside;
}

struct ang {
	double a, b;
};

bool operator <(const ang & p, const ang & q) {
	if (p.b == 0 && q.b == 0)
		return p.a < q.a;
	return p.a * 1ll * q.b < p.b * 1ll * q.a;
}

long long sq(pt & a, pt & b, pt & c) {
	return a.x * 1ll * (b.y - c.y) + b.x * 1ll * (c.y-a.y) + c.x * 1ll * (a.y-b.y);
}

bool IsPointInPolygonBinarySearch(std::vector<geo_objects::Vertex> p, pair<geo_objects::Point, int> zero_pt, geo_objects::Point q) {
    pt zero = zero_pt.first;
	int zero_id = zero_pt.second;
    int n = static_cast<int>(p.size()) - 1;

    rotate(p.begin(), p.begin()+zero_id, p.end());
	p.erase(p.begin());

	vector<ang> a(n);
	for (int i = 0; i < n; ++i) {
		a[i].a = p[i].y - zero.y;
		a[i].b = p[i].x - zero.x;
		if (a[i].a == 0) {
            a[i].b = a[i].b < 0 ? -1 : 1;
        }
	}

	bool in = false;
	if (q.x >= zero.x) {
		if (q.x == zero.x && q.y == zero.y) {
            in = true;
        } else {
			ang my = { q.y-zero.y, q.x-zero.x };
			if (my.a == 0) {
                my.b = my.b < 0 ? -1 : 1;
            }
			vector<ang>::iterator it = upper_bound (a.begin(), a.end(), my);
			if (it == a.end() && my.a == a[n-1].a && my.b == a[n-1].b) {
                it = a.end()-1;
            }
			if (it != a.end() && it != a.begin()) {
				int p1 = int (it - a.begin());
				if (sq (p[p1], p[p1-1], q) <= 0) {
                    in = true;
                }
			}
		}
    }
	return (in ? true : false);
}
