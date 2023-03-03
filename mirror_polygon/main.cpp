#include "input_reader.h"
#include "ex_request.h"
#include "geometric_objects.h"
#include "svg.h"
#include "visualization.h"
#include "log_duration.h"

#include <iostream>
#include <fstream>

using namespace std;

int main() {
    LOG_DURATION("Total"s);
    std::ifstream in("input.txt");
    std::streambuf *cinbuf = std::cin.rdbuf();
    std::cin.rdbuf(in.rdbuf());

    geo_objects::Polygon mp;                                                    //Многоугольник
    geo_objects::Point a;                                                       //Начальная точка
    geo_objects::Point b;                                                       //Конечная точка
    char read_type = 'f';                                                       //тип входных данных ('f' файл, 'c' консоль)
    bool is_success_read = in_reader::ReadQuery(std::cin, read_type, mp, a, b); //Читаем входные данные

    //RequestHandler rh;
    {
        std::ofstream out("output_result.txt");
        std::streambuf *coutbuf = std::cout.rdbuf();
        std::cout.rdbuf(out.rdbuf());

        if(is_success_read) {
            //rh.ExecuteRequest(std::cout, mp);                                     //Отвечаем на вопросы в текстовом формате
        } else {
            //
        }

        std::cin.rdbuf(cinbuf);
        std::cout.rdbuf(coutbuf);
    }

    {
        std::ofstream out("output_result.xml");
        std::streambuf *coutbuf = std::cout.rdbuf();
        std::cout.rdbuf(out.rdbuf());

        int size = mp.size();
        std::vector<svg::Point> vertexes(size + 1);
        for (int i = 0; i < size + 1; ++i) {
            geo_objects::Point p_v = mp.point();
            svg::Point p(p_v.x, p_v.y);
            vertexes[i] = p;
            mp.advance(1);
        }
        geo_objects::Edge e(a, b);
        //Visualization(std::cout, rh);                                         //Визуализируем результат в формате xml
        Visualization(std::cout, vertexes, e);
        std::cin.rdbuf(cinbuf);
        std::cout.rdbuf(coutbuf);
    }

    return 0;
}