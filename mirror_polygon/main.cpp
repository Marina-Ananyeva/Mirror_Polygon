#include "input_reader.h"
#include "ex_request.h"
#include "geometric_objects.h"
#include "svg.h"
#include "visualization.h"
#include "log_duration.h"
#include "tests.h"

#include <iostream>
#include <fstream>

using namespace std;

int main() {
    Tests();
    /*
    LOG_DURATION("Total"s);

    geo_objects::Polygon mp;                                                    //Многоугольник
    geo_objects::Point a;                                                       //Начальная точка
    geo_objects::Point b;                                                       //Конечная точка

    bool is_success_read = false;
    is_success_read = in_reader::ReadQuery(std::cin, mp, a, b); // Читаем входные данные

    
    if (is_success_read) {
        //{
                std::ofstream out("output_result.txt");
                std::streambuf *coutbuf = std::cout.rdbuf();
                std::cout.rdbuf(out.rdbuf());            

                //RequestHandler rh;
                //rh.ExecuteRequest(std::cout, mp);                                     //Отвечаем на вопросы в текстовом формате
                std::cout.rdbuf(coutbuf);        
        //}

        //{
                std::ofstream out1("output_result.xml");
                //std::streambuf *coutbuf = std::cout.rdbuf();
                std::cout.rdbuf(out1.rdbuf());

                int size = mp.size();
                std::vector<geo_objects::Point> vertexes(size + 1);
                for (int i = 0; i < size + 1; ++i) {
                    geo_objects::Point p = mp.point();
                    //svg::Point p(p_v.x, p_v.y);
                    vertexes[i] = p;
                    mp.advance(1);
                }
                geo_objects::Edge e(a, b);
                //Visualization(std::cout, rh);                                         //Визуализируем результат в формате xml
                Visualization(std::cout, vertexes, e);
                //std::cin.rdbuf(cinbuf);
                std::cout.rdbuf(coutbuf);
        //}
    } else {
        std::cout << "\nНе удалось считать данные. Пожалуйста запустите программу повторно."s << "\n"s;
    }
    std::cout << "\nБлагодарим за использование нашей программы!"s << '\n';
    */
    return 0;
}
/*
6
100 100 200 100 300 150 200 200 150 300 100 200
150 150 120 120
*/