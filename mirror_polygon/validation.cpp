#include "validation.h"

#include <algorithm>
#include <set>
#include <unordered_set>

using namespace std;

namespace in_reader{
void CheckPolygonSize (int size) {
    if (size < 3) {
        throw ValidityError(" - число вершин многоугольника не может быть менее трех"s);
    }
}
void CheckPolygonCoordinatesUnique(const vector<geo_objects::Point>& coordinates) {
    unordered_set<geo_objects::Point, geo_objects::Point::PointHasher> set(coordinates.begin(), coordinates.end());
    if (set.size() < coordinates.size()) {
        throw ValidityError(" - координаты повторяются"s);
    }
}

void CheckPolygonCoordinatesNotIntersecting(const vector<geo_objects::Point>& coordinates) {
    int size = static_cast<int>(coordinates.size());
    vector<seg> a(size - 1);
    for (int i = 0; i < size - 1; ++i) {
        a[i] = seg(coordinates[i], coordinates[i + 1], i);
    }
    a.push_back(seg(coordinates[size - 1], coordinates[0], size - 1));//добавляем ребро между последней и первой вершинами
    pair<int, int> validation_res = solve(a);
    if (validation_res != make_pair(-1, -1)) {
        throw ValidityError(" - многоугольник не должен быть пересекающимся"s);
    }
}

void CheckPointInsidePolygon(geo_objects::Polygon& mp, geo_objects::Point p_min, geo_objects::Point p_max, geo_objects::Point a, geo_objects::Point b) {
    bool is_inside = IsPointInsideBoundingBox(a, p_min, p_max);
    if (is_inside) {
        bool is_inside = IsPointInsideBoundingBox(b, p_min, p_max);
        if (is_inside) {
            int is_convex = mp.IsPolygonConvex();
            if (!is_convex) {
                is_inside = IsPointInPolygon(a, mp);
                if (is_inside) {
                    is_inside = IsPointInPolygon(a, mp);
                } else {
                    throw ValidityError(" - начальная точка находится не внутри многоугольника"s);
                }
                if (!is_inside) {
                    throw ValidityError(" - конечная точка находится не внутри многоугольника"s);
                }
            } else {
                is_inside = IsPointInPolygonBinarySearch();
                if (is_inside) {
                    is_inside = IsPointInPolygonBinarySearch();
                } else {
                    throw ValidityError(" - начальная точка находится не внутри многоугольника"s);
                }
                if (!is_inside) {
                    throw ValidityError(" - конечная точка находится не внутри многоугольника"s);
                }               
            }
        } else {
            throw ValidityError(" - конечная точка находится не внутри многоугольника"s);
        }
    } else {
        throw ValidityError(" - начальная точка находится не внутри многоугольника"s);
    }
}
} //namespace in_reader

/*
class ParsingError : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

double ReadNumber(std::istream& input) {
    std::string parsed_num;

    // Считывает в parsed_num очередной символ из input
    auto read_char = [&parsed_num, &input] {
        parsed_num += static_cast<char>(input.get());
        if (!input) {
            throw ParsingError("Ошибка чтения числа"s);
        }
    }; 
    // Считывает одну или более цифр в parsed_num из input
    auto read_digits = [&input, read_char] {
        if (!std::isdigit(input.peek())) {
            throw ParsingError("A digit is expected"s);
        }
        while (std::isdigit(input.peek())) {
            read_char();
        }
    };

}

Node LoadNumber(std::istream& input) {
    std::string parsed_num;

    // Считывает в parsed_num очередной символ из input
    auto read_char = [&parsed_num, &input] {
        parsed_num += static_cast<char>(input.get());
        if (!input) {
            throw ParsingError("Failed to read number from stream"s);
        }
    };

    // Считывает одну или более цифр в parsed_num из input
    auto read_digits = [&input, read_char] {
        if (!std::isdigit(input.peek())) {
            throw ParsingError("A digit is expected"s);
        }
        while (std::isdigit(input.peek())) {
            read_char();
        }
    };

    if (input.peek() == '-') {
        read_char();
    }
    // Парсим целую часть числа
    if (input.peek() == '0') {
        read_char();
        // После 0 в JSON не могут идти другие цифры
    } else {
        read_digits();
    }

    bool is_int = true;
    // Парсим дробную часть числа
    if (input.peek() == '.') {
        read_char();
        read_digits();
        is_int = false;
    }

    // Парсим экспоненциальную часть числа
    if (int ch = input.peek(); ch == 'e' || ch == 'E') {
        read_char();
        if (ch = input.peek(); ch == '+' || ch == '-') {
            read_char();
        }
        read_digits();
        is_int = false;
    }

    try {
        if (is_int) {
            // Сначала пробуем преобразовать строку в int
            try {
                return std::stoi(parsed_num);
            } catch (...) {
                // В случае неудачи, например, при переполнении
                // код ниже попробует преобразовать строку в double
            }
        }
        return std::stod(parsed_num);
    } catch (...) {
        throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
    }
}

Node LoadNode(std::istream& input) {
    char c;
    if (!(input >> c)) {
        throw ParsingError("Unexpected EOF"s);
    }
    switch (c) {
        case '[':
            return LoadArray(input);
        case '{':
            return LoadDict(input);
        case '"':
            return LoadString(input);
        case 't':
            // Атрибут [[fallthrough]] (провалиться) ничего не делает, и является
            // подсказкой компилятору и человеку, что здесь программист явно задумывал
            // разрешить переход к инструкции следующей ветки case, а не случайно забыл
            // написать break, return или throw.
            // В данном случае, встретив t или f, переходим к попытке парсинга
            // литералов true либо false
            [[fallthrough]];
        case 'f':
            input.putback(c);
            return LoadBool(input);
        case 'n':
            input.putback(c);
            return LoadNull(input);
        default:
            input.putback(c);
            return LoadNumber(input);
    }
}
*/