#include "visualization.h"

#include <vector>
#include <memory>

using namespace std;
using namespace svg;
using namespace shapes;
using namespace geo_objects;

// Реализует метод Draw интерфейса svg::Drawable для PolyGon
void PolyGon::Draw(svg::ObjectContainer& container) const {
    svg::Polyline pol;
    svg::Point p;
    int size = static_cast<int>(vertexes_.size());
    for (int i = 0; i < size; ++i) {
        p = vertexes_[i];
        pol.AddPoint(p);
    }
    container.Add(pol.SetFillColor("none"s).SetStrokeColor("black"s));
}

// Реализует метод Draw интерфейса svg::Drawable для Beam
void Beam::Draw(svg::ObjectContainer& container) const {
    svg::Point a(e_.org.x, e_.org.y);
    svg::Point b(e_.dest.x, e_.dest.y);
    container.Add(svg::Polyline().AddPoint(a).AddPoint(b).SetFillColor("red"s).SetStrokeColor("red"s));
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

void Visualization(ostream& out, vector<svg::Point> vertexes, Edge& e) {
    Document doc;
    vector<unique_ptr<svg::Drawable>> picture;
    picture.emplace_back(make_unique<PolyGon>(vertexes));
    picture.emplace_back(make_unique<Beam>(e));
    DrawPicture(picture, doc);
    doc.Render(out);
}

/*
void VisualizationPolygon(ostream& out, vector<svg::Point> vertexes) {
    Document doc;
    vector<unique_ptr<svg::Drawable>> picture;
    picture.emplace_back(make_unique<PolyGon>(vertexes));
    DrawPicture(picture, doc);
    doc.Render(out);
}

void VisualizationBeam(ostream& out, Edge& e) {
    Document doc;
    vector<unique_ptr<svg::Drawable>> picture;
    picture.emplace_back(make_unique<Beam>(e));
    DrawPicture(picture, doc);
    doc.Render(out);
}
