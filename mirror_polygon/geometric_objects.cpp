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

//Операция скалярного умножения реализована в виде дружественной функции классу Point, 
//а не члена класса, поскольку ее первый операнд не относится к типу Point. Оператор определен следующим образом:
Point operator*(double s, Point p) {
    return Point(s * p.x, s * p.y);
}

//Операция-функция operator[] возвращает координату х текущей точки, если в обращении 
//в качестве индекса координаты было указано значение О, или координату у при указании значения индекса 1:
double Point::operator[](int i) {
    return (i == 0) ? x : y;
}

//Операции отношения == и != используются для определения эквивалентности двух точек:
int Point::operator==(Point p) {
    return (x == p.x) && (y == p.y);
}

int Point::operator!=(Point p) {
    return !(*this == p);
}

//Операции < и > реализуют лексикографический порядок отношений
int Point::operator<(Point p) {
    return ((x < p.x) || ((x == p.x) && (y < p.y)));
}

int Point::operator>(Point p) {
    return ((x > p.x) || ((x == p.x) && (y > p.y)));
}

//Функция orientation возвращает значение 1, если обрабатываемые три точки ориентированы положительно, -1, если они ориентированы отрицательно, или 0, если они коллинеарны.

int orientation(Point p0, Point p1, Point p2) {
    Point a = p1 - p0;
    Point b = p2 - p0;
     double sa = a.x * b.y - b.x * a.y;
    if (sa > 0.0) {
        return 1;
    }
    if (sa < 0.0) {
        return -1;
    }
    return 0;
}

enum {LEFT, RIGHT, BEYOND, BEHIND, BETWEEN, ORIGIN, DESTINATION};
//    СЛЕВА, СПРАВА, ВПЕРЕДИ, ПОЗАДИ, МЕЖДУ, НАЧАЛО, КОНЕЦ

int Point::classify(Point p0, Point p1) {
    Point p2 = *this;
    Point a = p1 - p0;
    Point b = p2 - p0;
    double sa = a.x * b.y - b.x * a.y;
    if (sa > 0.0) {
        return LEFT;
    }
    if (sa < 0.0) {
        return RIGHT;
    }
    if ((a.x * b.x < 0.0) || (a.y * b.y < 0.0)) {
        return BEHIND;
    }
    if (b.length() - a.length() < EPSILON) {
        return BEYOND;
    }
    if (p0 == p2) {
        return ORIGIN;
    }
    if (p1 == p2) {
        return DESTINATION;
    }
    return BETWEEN;
}

int Point::classify(Edge e) {
    return classify(e.org, e.dest);
}

double Point::polarAngle() {
    if ((x == 0.0) && (y == 0.0)) {
        return -1.0;
    }
    if (x == 0.0) {
        return ((y > 0.0) ? 90 : 270);
    }

    double theta = atan(y/x);                     // в радианах
    theta *= 360 / (2 * 3.1415926);               // перевод в градусы
    if (x > 0.0)                                  // 1 и 4 квадранты
        return ((y >= 0.0) ? theta : 360 + theta);
    else                                          // 2 и З квадранты
        return (180 + theta);
}

double Point::length() {
    return sqrt(x * x + y * y);
}

double Point::distance(Edge e) {
    Edge ab = e;
    ab.flip().rot();                            // поворот ab на 90 градусов
                                                // против часовой стрелки
    Point n(ab.dest -  ab.org);
                                                // n = вектор, перпендикулярный ребру е
    n = (1.0 / n.length()) * n;
                                                // нормализация вектора n
    Edge f(*this, *this + n);
                                                // ребро f = n позиционируется 
                                                // на текущей точке  
    double t;                                   // t = расстоянию со знаком
    f.intersect(e, t);                          // вдоль вектора f до точки,
                                                // в которой ребро f пересекает ребро е
    return t;
}

//------------------Edge--------------------------
Edge::Edge(Point _org, Point _dest): org (_org), dest (_dest) {
}

Edge::Edge(): org (Point (0.0, 0.0)), dest (Point (1.0, 0.0)) {
}

//Поворот ребра на 90 градусов по часовой стрелке
Edge &Edge::rot() {
    Point m = 0.5 * (org + dest);
    Point v = dest - org;
    Point n(v.y, - v.x);
    Point x = 0.5 * n;
    org = m - x;
    dest = m + x;
    return *this;
}

Edge &Edge::flip() {
    return rot().rot();
}

Point Edge::point(double t) {
    return Point(org + t * (dest - org));
}

double Edge::dotProduct(Point p, Point q) {
  return (p.x * q.x + p.y * q.y);
}

enum { COLLINEAR, PARALLEL, SKEW, SKEW_CROSS, SKEW_NO_CROSS };
int Edge::intersect(Edge e, double t) {
    Point a = org;
    Point b = dest;
    Point c = e.org;
    Point d = e.dest;
    Point n = Point((d - c).y, (c - d).x);
    double denom = dotProduct(n, b - a);
    if (denom ==0.0) {
        int aclass = org.classify(e);
        if ((aclass==LEFT) || (aclass==RIGHT)) {
            return PARALLEL;
        }
        else return COLLINEAR;
    }
    double num = dotProduct(n, a-c);
    t = -num / denom;
    return SKEW;
}

int Edge::cross(Edge e, double t) {
    double s;
    int crossType = e.intersect(*this, s);
    if ((crossType == COLLINEAR) || (crossType == PARALLEL)) {
        return crossType;
    }

    if ((s < 0.0) || (s > 1.0)) {
        return SKEW_NO_CROSS;
    }

    intersect(e, t);
    if ((0.0 <= t) && (t <= 1.0)) {
        return SKEW_CROSS;
    }
    else
    return SKEW_NO_CROSS;
}

//Компонентная функция isVertical возвращает значение TRUE (истина) только в том случае, 
//если текущее ребро вертикально:
bool Edge::isVertical() {
    return (org.x == dest.x);
}

//Компонентная функция slope возвращает величину наклона текущего ребра или значение DBL_MAX, 
//если текущее ребро вертикально:
double Edge::slope() {
    if (org.x != dest.x) {
        return (dest.y - org.y) / (dest.x - org.x);
    }
    return MAX_DOUBLE;
}

//Для компонентной функции у задается значение х и она возвращает значение у, соответствующее точке (х, у) 
//на текущей бесконечной прямой линии. Функция действует только в том случае, если текущее ребро не вертикально.
double Edge::у(double x) {
    return slope() * (x - org.x) + org.y;
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

void Node::splice(Node *b) {
    Node *a = this;
    Node *an = a->_next;
    Node *bn = b->_next;
    a->_next = bn;
    b->_next = an;
    an->_prev = b;
    bn->_prev = a;
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

void Vertex::splice(Vertex *b) {
    Node::splice (b);
}

Vertex *Vertex::split(Vertex *b) {
    Vertex *bр = b->ccw()->insert(new Vertex(b->point()));  // занесение bр перед вершиной b
    insert(new Vertex(point()));                            // занесение ар после текущей вершины
    splice(bр);
    return bр;
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

Vertex *Polygon::v() {
    return _v;
}

int Polygon::size() {
    return _size;
}

Point Polygon::point() {
    return _v->point();
}

Edge Polygon::edge() {
    return Edge (point(), _v->cw()->point());
}

Vertex *Polygon::cw() {
    return _v->cw();
}

Vertex *Polygon::ccw() {
    return _v->ccw();
}

Vertex *Polygon::neighbor(int rotation) {
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

Polygon *Polygon::split(Vertex *b) {
    Vertex *bp = _v->split(b);
    resize();
    return new Polygon(bp);
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
bool pointlnConvexPolygon(Point s , Polygon &p) {
    if (p. size() == 1) {
        return (s == p.point());
    }
    if (p. size () == 2) {
        int c = s .classify (p. edge ());
        return ( (c==BETWEEN) || (c==ORIGIN) || (c==DESTINATION) );
    }
    Vertex *org = p.v();
    for (int i = 0; i < p.size(); i++, p.advance(CLOCKWISE)) {
        if (s.classify (p.edge()) == LEFT) {
            p.setV(org);
            return false;
        } 
    }
  return true;
}

Vertex *leastVertex(Polygon &p, int (*cmp)(Point*,Point*)) {
    Vertex *bestV = p.v();
    p.advance(CLOCKWISE);
    for (int  i=1; i < p.size(); p.advance(CLOCKWISE), i++) {
        if ((*cmp)(p.v(),bestV) < 0) {
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