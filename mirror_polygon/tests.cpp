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
    geo_objects::Point a1(100, 300);
    geo_objects::Point a2(180, 300);
    geo_objects::Point a3(200, 100);
    geo_objects::Point a4(100, 100);
    geo_objects::Point a5(300, 200);
    seg s1(a1, a2, 0);
    seg s2(a2, a3, 1);
    seg s3(a3, a4, 2);
    seg s4(a4, a5, 3);
    seg s5(a5, a1, 4);
    vector<seg> a{s1, s2, s3, s4, s5};
    pair<int, int> res = solve(a);
    assert(res == make_pair(4, 1));
}

void TestCheckIsPolygonConvex() {
    {
        geo_objects::Point a1(300, 300);
        geo_objects::Point a2(800, 300);
        geo_objects::Point a3(1000, 600);
        geo_objects::Point a4(800, 700);
        geo_objects::Point a5(600, 600);
        vector<geo_objects::Point> points = {a1, a2, a3, a4, a5};
        Polygon mp;
        for (const auto p : points) {
            mp.insert(geo_objects::Point(p));
        }
        geo_objects::Point a(100, 100);
        geo_objects::Point b(300, 200);
        int test = mp.IsPolygonConvex();
        assert(test == 1);
    }
    {
        geo_objects::Point a1(300, 300);
        geo_objects::Point a2(800, 300);
        geo_objects::Point a3(500, 600);
        geo_objects::Point a4(800, 700);
        geo_objects::Point a5(600, 600);
        vector<geo_objects::Point> points = {a1, a2, a3, a4, a5};
        Polygon mp;
        for (const auto p : points) {
            mp.insert(geo_objects::Point(p));
        }
        geo_objects::Point a(100, 100);
        geo_objects::Point b(300, 200);
        int test = mp.IsPolygonConvex();
        assert(test == -1);
    }

}

void TestCheckPointInsidePolygon() {
    geo_objects::Point a1(300, 300);
    geo_objects::Point a2(800, 300);
    geo_objects::Point a3(1000, 600);
    geo_objects::Point a4(800, 700);
    geo_objects::Point a5(600, 600);
    geo_objects::Point p_min(300, 300);
    geo_objects::Point p_max(1000, 700);
    vector<geo_objects::Point> points = {a1, a2, a3, a4, a5};
    Polygon mp;
    for (const auto p : points) {
        mp.insert(geo_objects::Point(p));
    }
    {
        geo_objects::Point a(400, 400);
        bool test = IsPointInsideBoundingBox(a, p_min, p_max);
        assert(test == true);
        test = IsPointInPolygon(a, mp);
        assert(test == true);
    }
    {
        geo_objects::Point a(100, 100);
        bool test = IsPointInsideBoundingBox(a, p_min, p_max);
        assert(test == false);
        test = IsPointInPolygon(a, mp);
        assert(test == false);
    }
    {
        geo_objects::Point a(300, 300);
        bool test = IsPointInsideBoundingBox(a, p_min, p_max);
        assert(test == true);
        test = IsPointInPolygon(a, mp);
        assert(test == true);
    }
    {
        geo_objects::Point a(400, 300);
        bool test = IsPointInsideBoundingBox(a, p_min, p_max);
        assert(test == true);
        test = IsPointInPolygon(a, mp);
        assert(test == true);
    }
    {
        geo_objects::Point a(900, 350);
        bool test = IsPointInsideBoundingBox(a, p_min, p_max);
        assert(test == true);
        test = IsPointInPolygon(a, mp);
        assert(test == false);
    }
    {
        geo_objects::Point a(300.5, 300.5);
        bool test = IsPointInsideBoundingBox(a, p_min, p_max);
        assert(test == true);
        test = IsPointInPolygon(a, mp);
        assert(test == true);
    }
    {
        geo_objects::Point a(299.5, 299.5);
        bool test = IsPointInsideBoundingBox(a, p_min, p_max);
        assert(test == false);
        test = IsPointInPolygon(a, mp);
        assert(test == false);
    }
}

void TestComputeAngleBeamFromTwoPoints() {
    {
        geo_objects::Point a(200, 300);
        geo_objects::Point b1(400, 300);
        geo_objects::Point b2(400, 500);
        geo_objects::Point b3(200, 500);
        geo_objects::Point b4(0, 500);
        geo_objects::Point b5(0, 300);
        geo_objects::Point b6(0, 100);
        geo_objects::Point b7(200, 100);
        geo_objects::Point b8(400, 100);

        vector<geo_objects::Point> points = {b1, b2, b3, b4, b5, b6, b7, b8};
        vector<double> test(points.size());
        for (int i = 0; i < static_cast<int>(points.size()); ++i) {
            test[i] = ComputeAngleBeamFromTwoPoints(a, points[i]);
        }
        vector<double> ans{0.0, 45.0, 90.0, 135.0, 180.0, -135.0, -90.0, -45.0};
        assert(test == ans);
    }

    {
        geo_objects::Point a(200, 500);
        geo_objects::Point b1(600, 300);
        geo_objects::Point b2(200, 500);
        geo_objects::Point b3(100, 500);
        geo_objects::Point b4(0, 500);
        geo_objects::Point b5(150, 300);
        geo_objects::Point b6(-100, 200);
        geo_objects::Point b7(-200, -100);
        geo_objects::Point b8(-400, 100);

        vector<geo_objects::Point> points = {b1, b2, b3, b4, b5, b6, b7, b8};
        vector<double> test(points.size());
        vector<double> ans{-26.565, 0.0, 180.0, 180.0, -104.036, -135.0, -123.69, -146.31};
        for (int i = 0; i < static_cast<int>(points.size()); ++i) {
            test[i] = ComputeAngleBeamFromTwoPoints(a, points[i]);
            //cout << fixed << setprecision(3) << test[i] << '\n';
            assert(fabs(test[i] - ans[i]) < 0.001);
        }
    }
}

void TestBeamsVisualization() {
    {
        geo_objects::Point a(200, 300);
        geo_objects::Point b1(400, 300);
        geo_objects::Point b2(400, 500);
        geo_objects::Point b3(200, 500);
        geo_objects::Point b4(0, 500);
        geo_objects::Point b5(0, 300);
        geo_objects::Point b6(0, 100);
        geo_objects::Point b7(200, 100);
        geo_objects::Point b8(400, 100);

        //ofstream input("output_result.xml");
        vector<Edge> edges = {Edge(a, b1), Edge(a, b2), Edge(a, b3), Edge(a, b4), Edge(a, b5), Edge(a, b6), Edge(a, b7), Edge(a, b8)};
        //VisualizationBeams(input, edges);
        VisualizationBeams(cout, edges);
    }

    {
        geo_objects::Point a(200, 500);
        geo_objects::Point b1(600, 300);
        geo_objects::Point b2(200, 500);
        geo_objects::Point b3(100, 500);
        geo_objects::Point b4(100, 500);
        geo_objects::Point b5(150, 300);
        geo_objects::Point b6(-100, 200);
        geo_objects::Point b7(-200, -100);
        geo_objects::Point b8(-400, -100);

        //ofstream input("output_result.xml");
        stringstream os;
        vector<Edge> edges = {Edge(a, b1), Edge(a, b2), Edge(a, b3), Edge(a, b4), Edge(a, b5), Edge(a, b6), Edge(a, b7), Edge(a, b8)};
        VisualizationBeams(os, edges);
        string test(os.str());
        string ans(R"/(<?xml version="1.0" encoding="UTF-8" ?>
<svg xmlns="http://www.w3.org/2000/svg" version="1.1">
  <polyline points="200,500 600,300 200,500 200,500 200,500 100,500 200,500 100,500 200,500 150,300 200,500 -100,200 200,500 -200,-100 200,500 -400,-100" fill="red" stroke="red"/>
</svg>)/");
        //assert(test == ans);
    }

}

void Tests() {
    //TestCheckPolygonCoordinatesNotIntersecting();
    //TestCheckIsPolygonConvex();
    TestCheckPointInsidePolygon();
    //TestComputeAngleBeamFromTwoPoints();
    //TestBeamsVisualization();
    using namespace std::literals;
    std::cout << "Test passed!"s << '\n';
}

