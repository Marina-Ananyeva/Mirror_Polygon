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

    double operator[](int);
    
    bool operator==(Point) const;
    bool operator!=(Point) const;

    bool operator<(Point) const;
    bool operator>(Point) const;
  
    double distance(Point);

    //определяет можно ли провести из точки на отрезок нормаль и реквизиты точки пересечения
    std::pair<bool, Point> distance_normal(Edge);

    struct PointHasher {
        std::size_t operator()(const Point& p) const {
            uint64_t hash = (size_t)(p.x) * 37 + (size_t)(p.y) * 37 * 37;
            return static_cast<size_t>(hash);
        }
    };
};

//определяет полярнй угол между двумя точками
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
    friend class Polygon;
};

//----------------------Polygon--------------------
class Polygon {
private:
public:
    Vertex *_v;
    int _size;
    void resize();
public:
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
    int IsPolygonConvex();
    ~Polygon();
};

Vertex *leastVertex(Polygon &p, int (*cmp)(Point *, Point *));

int leftToRightCmp(Point *a, Point *b);

int rightToLeftCmp(Point *a, Point *b);
} //namespace geo_objects