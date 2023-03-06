#define _USE_MATH_DEFINES

#include "geometric_objects.h"

using namespace std;

namespace geo_objects {
//--------------------Point-------------------------------------------------------
Point::Point(double _х, double _y): x(_х), y(_y) {
}

Point& Point::SetPoint (double xx, double yy) {
        x = xx;
        y = yy;
        return *this;
}

//Векторное сложение и векторное вычитание выполняется с помощью операций-функций со знаками операций + и - соответственно:
Point Point::operator+(Point p) {
     return Point (x + p.x, y + p.y);
}

Point Point::operator-(Point p) {
    return Point (x - p.x, y - p.y);
}

Point operator*(double s, Point p) {
    return Point(s * p.x, s * p.y);
}

double Point::operator[](int i) {
    return (i == 0) ? x : y;
}

bool Point::operator==(Point p) const{
    return (x == p.x) && (y == p.y);
}

bool Point::operator!=(Point p) const{
    return !(*this == p);
}

bool Point::operator<(Point p) const{
    return ((x < p.x) || ((x - p.x < EPSILON) && (y < p.y)));
}

bool Point::operator>(Point p) const{
    return ((x > p.x) || ((x - p.x < EPSILON) && (y > p.y)));
}

double Point::distance(Point p) {
    double d = sqrt(pow((p.x - x), 2) + pow((p.y - y), 2));
    return d;
}

pair<bool, Point> Point::distance_normal(Edge e) {
    double x1 = e.org.x;
    double y1 = e.org.y;
    double x2 = e.dest.x;
    double y2 = e.dest.y;
    long double L = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    long double PR = (x - x1) * (x2 - x1) + (y - y1) * (y2 - y1);
    bool res = true;
    long double cf = PR / L;
    if(cf < 0) {
        cf = 0; res = false; 
    }
    if(cf > 1) { 
        cf = 1;
        res = false; 
    }
    long double xres = x1 + cf * (x2 - x1);
    long double yres = y1 + cf * (y2 - y1);
    return make_pair(res, Point(xres, yres));
}

double polarAnglePoints(Point a, Point b) {
    if (fabs(a.y - b.y) < EPSILON && fabs(a.x - b.x) < EPSILON) {
        return 0.0;
    }
    if (fabs(a.y - b.y) < EPSILON) {
        return ((a.x < b.x - EPSILON) ? 0.0 : 180.0);
    }
    if (fabs(a.x - b.x) < EPSILON) {
        return ((a.y < b.y - EPSILON) ? 90.0 : -90.0);
    }

    double theta = atan((b.y - a.y) / (b.x - a.x)) * 180.0 / M_PI;
    if (b.x < a.x - EPSILON) {
        return ((a.y < b.y - EPSILON) ? 180.0 + theta : -180.0 + theta);
    }
    return theta;
}

double distance(Point p1, Point p2) {
    double d = sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
    return d;
}

// Точка оценки находится на отрезке линии
bool IsPointOnLine(Point p0, Point p1, Point p2) {
    bool flag = false;
    double d1 = (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y);
    if ((abs(d1) < EPSILON) && ((p0.x - p1.x) * (p0.x - p2.x) <= 0) && ((p0.y - p1.y) * (p0.y - p2.y) <= 0)) {
        flag = true;
    }
    return flag;
}
 
// Оцениваем пересечение двух отрезков прямой
bool IsIntersect(Point p1, Point p2, Point p3, Point p4) {
    bool flag = false;
    double d = (p2.x - p1.x) * (p4.y - p3.y) - (p2.y - p1.y) * (p4.x - p3.x);
    if (d != 0) {
        double r = ((p1.y - p3.y) * (p4.x - p3.x) - (p1.x - p3.x) * (p4.y - p3.y)) / d;
        double s = ((p1.y - p3.y) * (p2.x - p1.x) - (p1.x - p3.x) * (p2.y - p1.y)) / d;
        if ((r >= 0) && (r <= 1) && (s >= 0) && (s <= 1)) {
            flag = true;
        }
    }
    return flag;
}

//------------------Edge--------------------------
Edge::Edge(Point _org, Point _dest): org (_org), dest (_dest) {
}

Edge::Edge(): org (Point (0.0, 0.0)), dest (Point (1.0, 0.0)) {
}

//--------------Node-------------------------
Node::Node(): _next (this) , _prev (this) {
}

Node *Node::next() {
    return _next;
}
Node *Node::prev() {
    return _prev;
}

Node *Node::insert(Node *b) {
    Node *c = _next;
    b->_next = c;
    b->_prev = this ;
    _next = b;
    c->_prev = b;
    return b;
}

Node *Node::remove() {
    _prev->_next = _next;
    _next->_prev = _prev;
    _next = _prev = this;
    return  this;
}

Node::~Node() {
}

//-----------------Vertex------------------------
Vertex::Vertex(double x, double у): Point (x, у){
}

Vertex::Vertex(Point p): Point (p) {
}

//Компонентные функции cw и ccw возвращают указатели на последователя и предшественника текущей вершины соответственно.

Vertex *Vertex::cw(void) {
    return (Vertex*)_next;
}

Vertex *Vertex::ccw(void) {
    return (Vertex*)_prev;
}

enum { CLOCKWISE, COUNTER_CLOCKWISE };

Vertex *Vertex::neighbor(int rotation) {
    return ((rotation == CLOCKWISE) ? cw() : ccw());
}

//Компонентная функция point возвращает точку на плоскости, в которой находится текущая вершина
Point Vertex::point() {
    return *((Point*)this);
}

Vertex *Vertex::insert(Vertex *v) {
    return (Vertex *) (Node::insert (v));
}

Vertex *Vertex::remove() {
    return (Vertex *) (Node::remove ());
}

//----------------------Polygon--------------------
Polygon::Polygon(): _v(NULL), _size(0) {
}

Polygon::Polygon(Polygon &p) {
    _size = p._size;
    if (_size == 0)
    _v = NULL;
    else {
        _v = new Vertex (p.point());
        for (int i = 1; i < _size; i++) {
            p.advance(CLOCKWISE);
            _v = _v->insert (new Vertex (p.point()));
        }
        p.advance (CLOCKWISE);
        _v = _v->cw();
    }
}

Polygon::Polygon(Vertex *v): _v(v) {
    resize();
}

void Polygon::resize() {
    if (_v == NULL) {
        _size = 0;
    } else {
        Vertex *v = _v->cw();
        for (_size = 1; v != _v; ++_size, v = v->cw());
    }
}

Vertex *Polygon::v() const{
    return _v;
}

int Polygon::size() const{
    return _size;
}

Point Polygon::point() const{
    return _v->point();
}

Edge Polygon::edge() const{
    return Edge (point(), _v->cw()->point());
}

Vertex *Polygon::cw() const{
    return _v->cw();
}

Vertex *Polygon::ccw() const{
    return _v->ccw();
}

Vertex *Polygon::neighbor(int rotation) const{
    return _v->neighbor(rotation);
}

Vertex *Polygon::advance(int rotation) {
    return _v = _v->neighbor(rotation);
}

Vertex *Polygon::setV(Vertex *v) {
    return _v = v;
}

Vertex *Polygon::insert(Point p) {
    if (_size++ == 0) {
        _v = new Vertex(p);
    } else {
        _v = _v->insert(new Vertex(p));
    }
    return _v;
}

void Polygon::remove() {
    Vertex *v = _v;
    _v = (--_size == 0) ? NULL : _v->ccw();
    delete v->remove();
}

//Возвращает 1 - если выпуклый и -1, если не выпуклый
int Polygon::IsPolygonConvex() {
    if (_size == 3) {
        return 1;
    }

    long double is_convex = 0.0;
    for (int i = 0; i < size(); i++, advance(CLOCKWISE)) {
        Vertex* a = ccw();
        Vertex *b = v();
        Vertex *c = cw();
        Point ab(b->x - c->x, b->y - c->y);
        Point bc(a->x - b->x, a->y - b->y);
        long double prev = is_convex;
        is_convex = ab.x * bc.y - ab.y * bc.x;
        if (is_convex > 0 && prev < 0 || is_convex < 0 && prev > 0) {
            return -1;
        }
    }
    return 1;
}

Polygon::~Polygon() {
    if (_v) {
        Vertex *w = _v->cw();
        while (_v != w) {
            delete w->remove();
            w = _v->cw();
        }
        delete _v;
    }
}

//-------------------------------------------------------------------

Vertex *leastVertex(Polygon &p, int (*cmp)(Point*,Point*)) {
    Vertex *bestV = p.v();
    p.advance(CLOCKWISE);
    for (int i = 1; i < p.size(); p.advance(CLOCKWISE), i++) {
        if ((*cmp)(p.v(), bestV) < 0) {
            bestV = p.v();
        }
    }
    p.setV(bestV);
    return bestV;
}

int leftToRightCmp(Point *a, Point *b) {
    if (*a < *b) {
        return -1;
    }
    if (*a > *b) {
        return 1;
    }
    return 0;
}

int rightToLeftCmp(Point *a, Point *b) {
    return leftToRightCmp(b, a);
}
} //namespace geo_objects