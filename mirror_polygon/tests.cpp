#include "tests.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;
using namespace geo_objects;

void TestCheckPolygonCoordinatesNotIntersecting() {
    Point a1(100, 300);
    Point a2(180, 300);
    Point a3(200, 100);
    Point a4(100, 100);
    Point a5(300, 200);
    Point a(150, 120);
    Point b(170, 120);
    seg s1(a1, a2, 0);
    seg s2(a2, a3, 1);
    seg s3(a3, a4, 2);
    seg s4(a4, a5, 3);
    seg s5(a5, a1, 4);
    seg s6(a, b, 5);
    vector<seg> v{s1, s2, s3, s4, s5, s6};
    bool test = false;
    pair<int, int> res = IsPolygonIntersected(v, test);
    assert(res == make_pair(4, 1));
}

void TestCheckIsPolygonConvex() {
    {
        Point a1(300, 300);
        Point a2(800, 300);
        Point a3(1000, 600);
        Point a4(800, 700);
        Point a5(600, 600);
        vector<Point> points = {a1, a2, a3, a4, a5};
        Polygon mp;
        for (const auto p : points) {
            mp.insert(Point(p));
        }
        Point a(100, 100);
        Point b(300, 200);
        int test = mp.IsPolygonConvex();
        assert(test == 1);
    }
    {
        Point a1(300, 300);
        Point a2(800, 300);
        Point a3(500, 600);
        Point a4(800, 700);
        Point a5(600, 600);
        vector<Point> points = {a1, a2, a3, a4, a5};
        Polygon mp;
        for (const auto p : points) {
            mp.insert(Point(p));
        }
        Point a(100, 100);
        Point b(300, 200);
        int test = mp.IsPolygonConvex();
        assert(test == -1);
    }

}

void TestCheckPointInsidePolygon() {
    Point a1(300, 300);
    Point a2(800, 300);
    Point a3(1000, 600);
    Point a4(800, 700);
    Point a5(600, 600);
    Point p_min(300, 300);
    Point p_max(1000, 700);
    vector<Point> points = {a1, a2, a3, a4, a5};
    Polygon mp;
    for (const auto p : points) {
        mp.insert(Point(p));
    }
    {
        Point a(400, 400);
        bool test = IsPointInsideBoundingBox(a, p_min, p_max);
        assert(test == true);
        test = IsPointInPolygon(a, mp);
        assert(test == true);
    }
    {
        Point a(100, 100);
        bool test = IsPointInsideBoundingBox(a, p_min, p_max);
        assert(test == false);
        test = IsPointInPolygon(a, mp);
        assert(test == false);
    }
    {
        Point a(300, 300);
        bool test = IsPointInsideBoundingBox(a, p_min, p_max);
        assert(test == true);
        test = IsPointInPolygon(a, mp);
        assert(test == true);
    }
    {
        Point a(400, 300);
        bool test = IsPointInsideBoundingBox(a, p_min, p_max);
        assert(test == true);
        test = IsPointInPolygon(a, mp);
        assert(test == true);
    }
    {
        Point a(900, 350);
        bool test = IsPointInsideBoundingBox(a, p_min, p_max);
        assert(test == true);
        test = IsPointInPolygon(a, mp);
        assert(test == false);
    }
    {
        Point a(300.5, 300.5);
        bool test = IsPointInsideBoundingBox(a, p_min, p_max);
        assert(test == true);
        test = IsPointInPolygon(a, mp);
        assert(test == true);
    }
    {
        Point a(299.5, 299.5);
        bool test = IsPointInsideBoundingBox(a, p_min, p_max);
        assert(test == false);
        test = IsPointInPolygon(a, mp);
        assert(test == false);
    }
}

void TestComputeAngleBeamFromOnePoint() {
    {
        Point a1(100, 200);
        Point a2(300, 400);
        Point a3(600, 100);
        Edge e1(a1, a2);
        Edge e2(a3, a2);

        Point a(300, 200);
        pair<bool, Point> test1 = a.distance_normal(e1);
        pair<bool, Point> test2 = a.distance_normal(e2);
        assert(test1.first == true);
        assert(test2.first == true);

        Point b(300, 500);
        pair<bool, Point> test3 = b.distance_normal(e1);
        pair<bool, Point> test4 = b.distance_normal(e2);
        assert(test3.first == false);
        assert(test4.first == false);

        Point c(500, 350);
        pair<bool, Point> test5 = c.distance_normal(e1);
        pair<bool, Point> test6 = c.distance_normal(e2);
        assert(test5.first == false);
        assert(test6.first == true);

        Point d(300, 400);
        pair<bool, Point> test7 = d.distance_normal(e1);
        pair<bool, Point> test8 = d.distance_normal(e2);
        assert(test7.first == true);
        assert(test8.first == true);
    }
}

void TestComputeAngleBeamFromTwoPoints() {
    {
        Point a(200, 300);
        Point b1(400, 300);
        Point b2(400, 500);
        Point b3(200, 500);
        Point b4(0, 500);
        Point b5(0, 300);
        Point b6(0, 100);
        Point b7(200, 100);
        Point b8(400, 100);

        vector<Point> points = {b1, b2, b3, b4, b5, b6, b7, b8};
        vector<double> test(points.size());
        for (int i = 0; i < static_cast<int>(points.size()); ++i) {
            test[i] = polarAnglePoints(a, points[i]);
        }
        vector<double> ans{0.0, 45.0, 90.0, 135.0, 180.0, -135.0, -90.0, -45.0};
        assert(test == ans);
    }

    {
        Point a(200, 500);
        Point b1(600, 300);
        Point b2(200, 500);
        Point b3(100, 500);
        Point b4(0, 500);
        Point b5(150, 300);
        Point b6(-100, 200);
        Point b7(-200, -100);
        Point b8(-400, 100);

        vector<Point> points = {b1, b2, b3, b4, b5, b6, b7, b8};
        vector<double> test(points.size());
        vector<double> ans{-26.565, 0.0, 180.0, 180.0, -104.036, -135.0, -123.69, -146.31};
        for (int i = 0; i < static_cast<int>(points.size()); ++i) {
            test[i] = geo_objects::polarAnglePoints(a, points[i]);
            assert(fabs(test[i] - ans[i]) < 0.001);
        }
    }
}

void TestBeamsVisualization() {
    {
        Point a(200, 300);
        Point b1(400, 300);
        Point b2(400, 500);
        Point b3(200, 500);
        Point b4(0, 500);
        Point b5(0, 300);
        Point b6(0, 100);
        Point b7(200, 100);
        Point b8(400, 100);

        ofstream input("output_result.xml");
        vector<Edge> edges = {Edge(a, b1), Edge(a, b2), Edge(a, b3), Edge(a, b4), Edge(a, b5), Edge(a, b6), Edge(a, b7), Edge(a, b8)};
        VisualizationBeams(input, edges);
        //VisualizationBeams(cout, edges);
    }

    {
        Point a(200, 500);
        Point b1(600, 300);
        Point b2(200, 500);
        Point b3(100, 500);
        Point b4(100, 500);
        Point b5(150, 300);
        Point b6(-100, 200);
        Point b7(-200, -100);
        Point b8(-400, -100);

        ofstream input("output_result.xml");
        stringstream os;
        vector<Edge> edges = {Edge(a, b1), Edge(a, b2), Edge(a, b3), Edge(a, b4), Edge(a, b5), Edge(a, b6), Edge(a, b7), Edge(a, b8)};
        //VisualizationBeams(os, edges);
        VisualizationBeams(input, edges);
        string test(os.str());
        string ans(R"/(<?xml version="1.0" encoding="UTF-8" ?>
<svg xmlns="http://www.w3.org/2000/svg" version="1.1">
  <polyline points="200,500 600,300 200,500 200,500 200,500 100,500 200,500 100,500 200,500 150,300 200,500 -100,200 200,500 -200,-100 200,500 -400,-100" fill="red" stroke="red"/>
</svg>)/");
        //assert(test == ans);
    }

}

void Tests() {
    TestCheckPolygonCoordinatesNotIntersecting();
    TestCheckIsPolygonConvex();
    TestCheckPointInsidePolygon();
    TestComputeAngleBeamFromOnePoint();
    TestComputeAngleBeamFromTwoPoints();
    TestBeamsVisualization();
    using namespace std::literals;
    std::cout << "Test passed!"s << '\n';
}

