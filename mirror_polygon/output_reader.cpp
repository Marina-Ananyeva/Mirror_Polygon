#include "output_reader.h"

using namespace std;

bool OutputResult(ostream& os, double angle1, double angle2) {
    std::cout << "\nВыберите способ вывода ответов: 1 - в файл, 2 - на консоль"s << '\n';
    int read_type = 0;                                                     //тип входных данных (1 - файл, 2 - консоль)
    cin >> read_type;
    
    if (read_type == 1) {
        std::ofstream out("output_result.txt");
        std::streambuf *coutbuf = std::cout.rdbuf();
        std::cout.rdbuf(out.rdbuf());
        os << "\nРезультаты расчетов:"s << '\n';

        os << fixed << setprecision(3) << "\nПервый угол "<< angle1 << " градусов."s << '\n';
        os << "\nВторой угол "<< angle2 << " градусов."s << setprecision(2) << endl;

        std::cout.rdbuf(coutbuf);
    } else if (read_type == 2) {
        os << "\nРезультаты расчетов:"s << '\n';

        os << fixed << setprecision(3) << "\nПервый угол "<< angle1 << " градусов."s <<  '\n';
        os << "\nВторой угол "<< angle2 << " градусов."s << setprecision(2) <<endl;

    } else {
        cout << "\nСпособ ввода данных выбран некорректно"s << '\n';
        cout << "\nПовторить ввод? Введите \"1 - да\" или \"2 - нет\":"s << '\n';
        int ans;
        cin >> ans;
        if (ans == 1) {
            OutputResult(os, angle1, angle2);
        } else {
            return false;
        }
    }
    return true;
}