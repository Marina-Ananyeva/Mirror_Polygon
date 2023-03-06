#include "input_reader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>

using namespace std;

namespace in_reader{
void ReadFromConsole(std::istream& input, Polygon &mp, Point& a, Point& b, request::RequestHandler& rh) {
    cout << "\nВведите количество вершин многоугольника (не менее 3):" << '\n';
    int size = 0;
    input >> size;
    CheckPolygonSize(size);

    cout << "\nВведите координаты вершин (по порядку):" << '\n';

    for (int i = 0; i < size; ++i) {
        double x = 0.0, y = 0.0;
        input >> x >> y;
        Point p(x, y);
        mp.insert(Point(p));
    }
    mp.resize();
    
    cout << "\nВведите координаты начальной и конечной точек:" << '\n';
    double x1 = 0.0, y1 = 0.0, x2 = 0.0, y2 = 0.0;
    input >> x1 >> y1 >> x2 >> y2; 
    a.SetPoint(x1, y1);
    b.SetPoint(x2, y2);

    PolygonValidation(mp, a, b, rh);
}

void ReadFromFile(std::istream& input, Polygon &mp, Point& a, Point& b, request::RequestHandler& rh) {
    int size = 0;
    input >> size;
    CheckPolygonSize(size);

    for (int i = 0; i < size; ++i) {
        double x = 0.0, y = 0.0;
        input >> x >> y;
        Point p(x, y);
        mp.insert(Point(p));
    }
    mp.resize();

    double x1 = 0.0, y1 = 0.0, x2 = 0.0, y2 = 0.0;
    input >> x1 >> y1 >> x2 >> y2; 
    a.SetPoint(x1, y1);
    b.SetPoint(x2, y2);

    PolygonValidation(mp, a, b, rh);
}

bool ReadQuery(std::istream& input, Polygon &mp, Point& a, Point& b, request::RequestHandler& rh) {
    std::cout << "\nВыберите способ ввода данных: 1 - из файла, 2 - с консоли"s << '\n';
    int read_type = 0;                                                     //тип входных данных (1 - файл, 2 - консоль)
    input >> read_type;
    
    if (read_type == 1) {
        std::ifstream in("input.txt");
        std::streambuf *cinbuf = std::cin.rdbuf();
        std::cin.rdbuf(in.rdbuf());

        try {
            ReadFromFile(input, mp, a, b, rh);
        } catch (const std::exception & e) {
           std::cin.rdbuf(cinbuf);

            cout << e.what() << '\n' << "\nПовторить ввод? Введите \"1 - да\" или \"2 - нет\":"s << '\n';
            int ans;
            input >> ans;
            if (ans == 1) {
                ReadQuery(input, mp, a, b, rh);
            } else {
                cout << "\nБлагодарим за использование нашей программы!"s << '\n';
                return false;
            }
        }

        std::cin.rdbuf(cinbuf);
    } else if (read_type == 2) {
        try {
            ReadFromConsole(input, mp, a, b, rh);
        } catch (const std::exception & e) {
            cout << e.what() << '\n' << "\nПовторить ввод? Введите \"1 - да\" или \"2 - нет\":"s << '\n';
            int ans;
            input >> ans;
            if (ans == 1) {
                ReadQuery(input, mp, a, b, rh);
            } else {
                return false;
            }
        }
    } else {
        cout << "\nСпособ ввода данных выбран некорректно"s << '\n';
        cout << "\nПовторить ввод? Введите \"1 - да\" или \"2 - нет\":"s << '\n';
        int ans;
        input >> ans;
        if (ans == 1) {
            ReadQuery(input, mp, a, b, rh);
        } else {
            return false;
        }
    }
    return true;
}
} //namespace in_reader