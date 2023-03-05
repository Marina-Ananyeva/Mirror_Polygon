#pragma once

#include "constants.h"

#include <cmath>
#include <stdint.h>

namespace geo_objects {
class Edge;
//--------------------Point-------------------------------------------------------
class Point {
public:
    double x = 0.0;
    double y = 0.0;

    Point() = default;
    Point(double _x, double _y);

    Point &SetPoint(double, double);

    Point operator+(Point);
    Point operator-(Point);
    friend Point operator*(double, Point);
    
    // возвращает координату х, если в качестве индекса
    // координаты указано значение О, или координату у при индексе 1
    double operator[](int);
    
    // одинаковы ли точки ?
    int operator==(Point) const;
    int operator!=(Point) const;
    
    // лексикографический порядок отношений, точка а < точки b,
    // если либо а.х < b.х, либо a.х = b.x и а.у < b.у.  
    int operator<(Point) const;
    int operator>(Point) const;

    //Разделение плоскости на семь областей направленным отрезком прямой линии
    // Возвращается значение типа перечисления, указывающее на положение
    // точки относительно отрезка
    enum {LEFT, RIGHT, BEYOND, BEHIND, BETWEEN, ORIGIN, DESTINATION};
    //       СЛЕВА, СПРАВА, ВПЕРЕДИ, ПОЗАДИ, МЕЖДУ,   НАЧАЛО, КОНЕЦ
    int classify(Point, Point);
    int classify(Edge);  // ребро вместо пары точек
    
    // Угол точки в полярной системе координат
    // возвращает -1, если точка = (0, 0)
    double polarAngle(); 
  
    double length();
  
    double distance(Point);

    double distance(Edge);

    struct PointHasher {
        std::size_t operator()(const Point& p) const {
            uint64_t hash = (size_t)(p.x) * 37 + (size_t)(p.y) * 37 * 37;
            return static_cast<size_t>(hash);
        }
    };
};

//Функция orientation возвращает значение 1, если обрабатываемые три точки ориентированы положительно,
// -1, если они ориентированы отрицательно, или 0, если они коллинеарны.
int orientation(Point, Point, Point);

double polarAnglePoints(Point, Point);

double distance(Point, Point);

// Точка оценки находится на отрезке линии
bool IsPointOnLine(Point, Point, Point);

// Оцениваем пересечение двух отрезков прямой
bool IsIntersect(Point, Point, Point, Point);
//------------------Edge--------------------------
//Ребро, любые линии
class Edge {
public:
    Point org;                          //концевая точка начала
    Point dest;                         //концевая точка конца
    
    Edge (Point _org, Point _dest);
    Edge ();
    Edge &rot();
    Edge &flip();
    Point point(double);
    int intersect(Edge, double);
    double dotProduct(Point, Point);
    int cross(Edge, double);
    bool isVertical();
    double slope();
    double у(double);
};

//--------------Node-------------------------
class Node {
protected:
    Node *_next;            // связь к последующему узлу
    Node *_prev;            // связь к предшествующему узлу
public:
    Node ();
    Node *next();
    Node *prev();
    Node *insert(Node*);    // вставить узел после текущего
    Node *remove();     // удалить узел из списка, возвратить его указатель
    void splice(Node*);
    virtual ~Node ();
};

//-----------------Vertex------------------------
class Vertex: public Node, public Point {
public:
    Vertex(double x, double y);
    Vertex(Point);
    Vertex *cw();
    Vertex *ccw();
    Vertex *neighbor(int rotation);
    Point point();
    Vertex *insert(Vertex*);
    Vertex *remove();
    void splice(Vertex*);
    Vertex *split(Vertex*);
    friend class Polygon;
};

//----------------------Polygon--------------------
class Polygon {
//private:
public:
    Vertex *_v;
    int _size;
    void resize();
//public:
    Polygon();
    Polygon(Polygon&);
    Polygon(Vertex*);
    Vertex *v() const;
    int size() const;
    Point point() const;
    Edge edge() const;
    Vertex *cw() const;
    Vertex *ccw() const;
    Vertex *neighbor(int rotation) const;
    Vertex *advance(int rotation);
    Vertex *setV(Vertex*);
    Vertex *insert(Point);
    void remove();
    Polygon * split(Vertex*);
    int IsPolygonConvex();
    ~Polygon();
};

bool pointInConvexPolygon(Point, Polygon &);

Vertex *leastVertex(Polygon &p, int (*cmp)(Point *, Point *));

int leftToRightCmp(Point *a, Point *b);

int rightToLeftCmp(Point *a, Point *b);
} //namespace geo_objects