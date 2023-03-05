#include "tests.h"

#include <vector>
#include <iostream>

using namespace std;

void Tests() {
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
    vector<seg> a;
    a.push_back(s1);
    a.push_back(s2);
    a.push_back(s3);
    a.push_back(s4);
    a.push_back(s5);
    pair<int, int> res = solve(a);
    cout << res.first << " "s << res.second << '\n';
}
