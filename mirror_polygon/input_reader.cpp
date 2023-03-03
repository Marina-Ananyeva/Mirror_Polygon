#include "input_reader.h"

#include <string>
#include <stdexcept>
#include <vector>

using namespace std;

namespace in_reader{
class ValidityError : public logic_error {
public:
    ValidityError(const string& error_text) : logic_error(MakeErrorMessage(error_text)) {
    }
    string MakeErrorMessage(const string& error_text) {
        return "Входные данные некорректны"s + error_text;
    }
};

void ReadFromConsole(std::istream& input, geo_objects::Polygon &mp, geo_objects::Point& a, geo_objects::Point& b) {
    int size = 0;
    input >> size;
    if (size < 3) {
        throw ValidityError(" - число вершин многоугольника не может быть менее трех"s);
    }

    vector<pair<double, double>> coordinates;
    for (int i = 0; i < size; ++i) {
        double x = 0.0, y = 0.0;
        input >> x >> y;
        coordinates.push_back(make_pair(x, y));
    }

    if (!CheckPolygonCoordinatesUniqueValidity(coordinates)) {
        throw ValidityError(" - координаты повторяются"s);
    }

    if (!CheckPolygonCoordinatesOrderValidity(coordinates)) {
        throw ValidityError(" - координаты следуют не порядку"s);
    }

    if (!CheckPolygonCoordinatesNotIntersectingValidity(coordinates)) {
        throw ValidityError(" - многоугольник не должен быть пересекающимся"s);
    }

    for (const auto p : coordinates) {
        mp.insert(geo_objects::Point(p.first, p.second));
    }
    mp.resize();

    double x = 0.0, y = 0.0;
    input >> x >> y;
    if (!CheckPointInsidePolygonValidity(x, y)) {
        throw ValidityError(" - начальная точка находится не внутри многоугольника"s);
    }
    
    double x1 = 0.0, y1 = 0.0;
    input >> x1 >> y1;
    if (!CheckPointInsidePolygonValidity(x1, y1)) {
        throw ValidityError(" - конечная точка находится не внутри многоугольника"s);
    }

    a.SetPoint(x, y);
    b.SetPoint(x1, y1);
}

void ReadFromFile(std::istream& input, geo_objects::Polygon &mp, geo_objects::Point& a, geo_objects::Point& b) {
        int size = 0;
    input >> size;
    if (size < 3) {
        throw ValidityError(" - число вершин многоугольника не может быть менее трех"s);
    }

    vector<pair<double, double>> coordinates;
    for (int i = 0; i < size; ++i) {
        double x = 0.0, y = 0.0;
        input >> x >> y;
        coordinates.push_back(make_pair(x, y));
    }

    if (!CheckPolygonCoordinatesUniqueValidity(coordinates)) {
        throw ValidityError(" - координаты повторяются"s);
    }

    if (!CheckPolygonCoordinatesOrderValidity(coordinates)) {
        throw ValidityError(" - координаты следуют не порядку"s);
    }

    if (!CheckPolygonCoordinatesNotIntersectingValidity(coordinates)) {
        throw ValidityError(" - многоугольник не должен быть самопересекающимся"s);
    }

    for (const auto p : coordinates) {
        mp.insert(geo_objects::Point(p.first, p.second));
    }
    mp.resize();

    double x = 0.0, y = 0.0;
    input >> x >> y;
    if (!CheckPointInsidePolygonValidity(x, y)) {
        throw ValidityError(" - начальная точка находится не внутри многоугольника"s);
    }
    
    double x1 = 0.0, y1 = 0.0;
    input >> x1 >> y1;
    if (!CheckPointInsidePolygonValidity(x1, y1)) {
        throw ValidityError(" - конечная точка находится не внутри многоугольника"s);
    }

    a.SetPoint(x, y);
    b.SetPoint(x1, y1);
}

bool ReadQuery(std::istream& input, char read_type, geo_objects::Polygon &mp, geo_objects::Point& a, geo_objects::Point& b) {
    bool is_success_read = true;
    if (read_type == 'f') {
        try {
            ReadFromFile(input, mp, a, b);
        } catch (const std::exception & e) {
            cout << e.what() << '\n' << "Try again? Enter \"yes\" or \"no\":"s << '\n';
            string str;
            cin >> str;
            //input >> str;
            if (str[0] == 'y') {
                ReadQuery(input, read_type, mp, a, b);
            } else {
                is_success_read = false;
            }
        }
    }
    if (read_type == 'c') {
        try {
            ReadFromConsole(input, mp, a, b);
        } catch (const std::exception & e) {
            cout << e.what() << '\n' << "Try again? Enter \"yes\" or \"no\":"s << '\n';
            string str;
            input >> str;
            if (str[0] == 'y') {
                ReadQuery(input, read_type, mp, a, b);
            } else {
                is_success_read = false;
            }
        }
    }
    return is_success_read;
}
} //namespace in_reader