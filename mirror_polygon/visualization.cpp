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
        pol.AddPoint(vertexes_[i]);
    }
    pol.AddPoint(vertexes_[0]); //добавляем первую вершину, чтобы фигура была замкнутой
    container.Add(pol.SetFillColor("none"s).SetStrokeColor("black"s));
}

// Реализует метод Draw интерфейса svg::Drawable для Beam
void Beam::Draw(svg::ObjectContainer& container) const {
    container.Add(svg::Polyline().AddPoint(e_.org).AddPoint(e_.dest).SetFillColor("none"s).SetStrokeColor("red"s));
}

// Реализует метод Draw интерфейса svg::Drawable для Beams
void Beams::Draw(svg::ObjectContainer& container) const {
    svg::Polyline pol;
    int size = static_cast<int>(beams_.size());
    for (int i = 0; i < size; ++i) {
        pol.AddPoint(beams_[i].org).AddPoint(beams_[i].dest);
    }
    container.Add(pol.SetFillColor("none"s).SetStrokeColor("red"s));
}

// Реализует метод Draw интерфейса svg::Drawable для Zigzag
void Zigzag::Draw(svg::ObjectContainer& container) const {
    svg::Polyline pol;
    int size = static_cast<int>(z_.size());
    for (int i = 0; i < size; ++i) {
        pol.AddPoint(z_[i]);
    }
    container.Add(pol.SetFillColor("none"s).SetStrokeColor("red"s));
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

void Visualization(ostream& out, request::RequestHandler& rh) {
    Document doc;
    vector<unique_ptr<svg::Drawable>> picture;
    if (!rh.vertexes.empty()) {
        picture.emplace_back(make_unique<PolyGon>(rh.vertexes));
    }
    if (!rh.beam1.empty()) {
        picture.emplace_back(make_unique<Zigzag>(rh.beam1));
    }
    if (!rh.beam2.empty()) {
        picture.emplace_back(make_unique<Zigzag>(rh.beam2));
    }
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

void VisualizationBeams(ostream& out, vector<Edge> beams) {
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

void VisualizationPolygon(ostream& out, vector<Vertex> vertexes) {
    Document doc;
    vector<unique_ptr<svg::Drawable>> picture;
    picture.emplace_back(make_unique<PolyGon>(vertexes));
    DrawPicture(picture, doc);
    doc.Render(out);
}


