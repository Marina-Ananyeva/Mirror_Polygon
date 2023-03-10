#include "geometric_objects.h"
#include "ex_request.h"
#include "input_reader.h"
#include "log_duration.h"
#include "svg.h"
#include "visualization.h"

#include "tests.h"

#include <iostream>
#include <fstream>

using namespace std;

int main() {
    Tests();
    
    geo_objects::Polygon mp;                                                    //многоугольник
    geo_objects::Point a;                                                       //начальная точка
    geo_objects::Point b;                                                       //конечная точка
    request::RequestHandler rh;                                                 //решение

    bool is_success_read = false;                                               //флаг успешный ввод данных
    bool is_success_write = false;                                              //флаг успешный вывод данных
    is_success_read = in_reader::ReadQuery(std::cin, mp, a, b, rh);             //читаем входные данные

    if (is_success_read) {
        LOG_DURATION("\nThe program`s running time is"s);
        is_success_write = rh.ExecuteRequest(std::cout, mp, a, b);              //отвечаем на вопросы в текстовом формате       

        std::ofstream out("output_result.xml");
        std::streambuf *coutbuf = std::cout.rdbuf();
        std::cout.rdbuf(out.rdbuf());

        Visualization(std::cout, rh);                                           //визуализируем результат в формате xml

        std::cout.rdbuf(coutbuf);
        if (!is_success_write) {
            std::cout << "\nFailed to output. Please run the program again."s << "\n"s;
        }
    } else {
        std::cout << "\nFailed to input. Please run the program again."s << "\n"s;
    }
    std::cout << "\nThanks for using our programs!"s << '\n';
    return 0;
}
