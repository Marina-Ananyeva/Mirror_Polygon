#include "svg.h"

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;
using namespace svg;
using namespace shapes;

// Выполняет линейную интерполяцию значения от from до to в зависимости от параметра t
uint8_t Lerp(uint8_t from, uint8_t to, double t) {
    return static_cast<uint8_t>(std::round((to - from) * t + from));
}

// Выполняет линейную интерполяцию Rgb цвета от from до to в зависимости от параметра t
svg::Rgb Lerp(svg::Rgb from, svg::Rgb to, double t) {
    return {Lerp(from.red, to.red, t), Lerp(from.green, to.green, t), Lerp(from.blue, to.blue, t)};
}

template <typename DrawableIterator>
void DrawPicture(DrawableIterator begin, DrawableIterator end, svg::ObjectContainer& target) {
    for (auto it = begin; it != end; ++it) {
        (*it)->Draw(target);
    }
}

template <typename Container>
void DrawPicture(const Container& container, svg::ObjectContainer& target) {
    using namespace std;
    DrawPicture(begin(container), end(container), target);
}

int main() {
    freopen("output_result.xml", "w", stdout);

    Document doc;
    //Оранжевый круг
    doc.Add(Circle()
                    .SetRadius(20)
                    .SetCenter({40.0, 40.0})
                    .SetFillColor("rgba(255, 165, 0, 0.5)"s)
                    .SetStrokeColor("purple"s));

    //10 кругов с интерполяцией цвета
    Rgb start_color{0, 255, 30};
    Rgb end_color{20, 20, 150};

    const int num_circles = 10;
    for (int i = 0; i < num_circles; ++i) {
        const double t = double(i) / (num_circles - 1);
        const Rgb fill_color = Lerp(start_color, end_color, t);
        doc.Add(Circle()
                    .SetFillColor(fill_color)
                    .SetStrokeColor("black"s)
                    .SetCenter({i * 20.0 + 40, 120.0})
                    .SetRadius(20));
    }

    vector<unique_ptr<svg::Drawable>> picture;

    //Красный треугольник
    picture.emplace_back(make_unique<Triangle>(Point{140, 20}, Point{100, 60}, Point{180, 60}));

    //Снеговик
    picture.emplace_back(make_unique<Snowman>(Point{80, 200}, 20.0));
    DrawPicture(picture, doc);

    //Надпись с текстом
    const Text base_text = Text()
                                .SetFontFamily("Verdana"s)
                                .SetFontSize(15)
                                .SetPosition({40, 400})
                                .SetData("Top Systems ;-)"s);
    doc.Add(Text{base_text}
                .SetStrokeColor("yellow"s)
                .SetFillColor("yellow"s)
                .SetStrokeLineJoin(StrokeLineJoin::ROUND)
                .SetStrokeLineCap(StrokeLineCap::ROUND)
                .SetStrokeWidth(10));
    doc.Add(Text{base_text}.SetFillColor("red"s));

    doc.Render(cout);
}


