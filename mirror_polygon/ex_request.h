#pragma once

#include "geometric_objects.h"
#include "output_reader.h"

#include <iostream>
#include <vector>

namespace request {
using namespace geo_objects;
class RequestHandler {
public:
    //для расчета
    bool is_convex = false;                         //признак выпуклый/ не выпуклый
    bool is_e_btw_points = false;                   //признак есть ли между точками ребро
    Point p_min;                                    //минимальное значение вершин по оси x
    Point p_max;                                    //минимальное значение вершин по оси y
    std::pair<Point, int> zero_pt;                  //координаты и номер самой маленькой вершины
    std::vector<Vertex> vertexes;                   //все вершины
    std::pair<long double, Point> near_edge_a;      //расстояние и ближайшая точка на ребре (для прямой под прямым углом от начальной точки)
    std::pair<long double, std::pair<Point, Point>> near_edge_ab;     //ближайшее ребро к начальной и конечной точкам

    //для ответа
    std::vector<Point> beam1;                       //луч - ответ на вопрос 1
    std::vector<Point> beam2;                       //луч - ответ на вопрос 2
    double angle1 = 0.0;                            //угол - ответ на вопрос 1
    double angle2 = 0.0;                            //угол - ответ на вопрос 2

    double ComputeAngleBeamFromOnePoint(Polygon &, Point);

    double ComputeAngleBeamFromTwoPoints(Polygon &, Point, Point);

    bool ExecuteRequest(std::ostream&, Polygon &, Point, Point);
}; 
}// namespace request



