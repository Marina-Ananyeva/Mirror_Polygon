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
    int size = static_cast<int>(vertexes_.size());
    for (int i = 0; i < size; ++i) {
        svg::Point p(vertexes_[i].x, vertexes_[i].y);
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

// Реализует метод Draw интерфейса svg::Drawable для Beams
void Beams::Draw(svg::ObjectContainer& container) const {
    svg::Polyline pol;
    int size = static_cast<int>(beams_.size());
    for (int i = 0; i < size; ++i) {
        svg::Point p1(beams_[i].org.x, beams_[i].org.y);
        svg::Point p2(beams_[i].dest.x, beams_[i].dest.y);
        pol.AddPoint(p1).AddPoint(p2);
    }
    container.Add(pol.SetFillColor("red"s).SetStrokeColor("red"s));
}

// Реализует метод Draw интерфейса svg::Drawable для Zigzag
void Zigzag::Draw(svg::ObjectContainer& container) const {
    svg::Polyline pol;
    int size = static_cast<int>(z_.size());
    for (int i = 0; i < size; ++i) {
        svg::Point p(z_[i].x, z_[i].y);
        pol.AddPoint(p);
    }
    container.Add(pol.SetFillColor("red"s).SetStrokeColor("red"s));
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

void Visualization(ostream& out, vector<geo_objects::Point> vertexes, geo_objects::Edge& e) {
    Document doc;
    vector<unique_ptr<svg::Drawable>> picture;
    picture.emplace_back(make_unique<PolyGon>(vertexes));
    picture.emplace_back(make_unique<Beam>(e));
    DrawPicture(picture, doc);
    doc.Render(out);
}

void VisualizationBeam(ostream& out, geo_objects::Edge& e) {
    Document doc;
    vector<unique_ptr<svg::Drawable>> picture;
    picture.emplace_back(make_unique<Beam>(e));
    DrawPicture(picture, doc);
    doc.Render(out);
}

void VisualizationBeams(ostream& out, vector<geo_objects::Edge> beams) {
    Document doc;
    vector<unique_ptr<svg::Drawable>> picture;
    picture.emplace_back(make_unique<Beams>(beams));
    DrawPicture(picture, doc);
    doc.Render(out);
}

void VisualizationZigzag(ostream& out, vector<geo_objects::Point> z) {
    Document doc;
    vector<unique_ptr<svg::Drawable>> picture;
    picture.emplace_back(make_unique<Zigzag>(z));
    DrawPicture(picture, doc);
    doc.Render(out);
}

void VisualizationPolygon(ostream& out, vector<geo_objects::Point> vertexes) {
    Document doc;
    vector<unique_ptr<svg::Drawable>> picture;
    picture.emplace_back(make_unique<PolyGon>(vertexes));
    DrawPicture(picture, doc);
    doc.Render(out);
}


