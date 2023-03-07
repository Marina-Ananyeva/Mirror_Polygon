#include "output_reader.h"

using namespace std;

bool OutputResult(ostream& os, double angle1, double angle2) {
    std::cout << "\nSelect the method of output: 1 - to file, 2 - to console"s << '\n';
    int read_type = 0;                                                     //тип входных данных (1 - файл, 2 - консоль)
    cin >> read_type;
    
    if (read_type == 1) {
        std::ofstream out("output_result.txt");
        std::streambuf *coutbuf = std::cout.rdbuf();
        std::cout.rdbuf(out.rdbuf());
        os << "\nCalculation result:"s << '\n';

        os << fixed << setprecision(3) << "\nFirst angle "<< angle1 << " degrees."s << '\n';
        os << "\nSecond angle "<< angle2 << " degrees."s << setprecision(2) << endl;

        std::cout.rdbuf(coutbuf);
    } else if (read_type == 2) {
        os << "\nCalculation result:"s << '\n';

        os << fixed << setprecision(3) << "\nFirst angle "<< angle1 << " degrees."s <<  '\n';
        os << "\nSecond angle "<< angle2 << " degrees."s << setprecision(2) <<endl;

    } else {
        cout << "\nMethod of input selected incorrectly"s << '\n';
        cout << "\nRepeat the input? Enter \"1 - yes\" or \"2 - no\":"s << '\n';
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