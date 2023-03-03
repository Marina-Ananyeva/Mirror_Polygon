#pragma once

#include "svg.h"
#include "geometric_objects.h"

#include <vector>

using namespace svg;
using namespace shapes;
using namespace geo_objects;

class PolyGon : public svg::Drawable {
public:
    PolyGon(std::vector<svg::Point> vertexes): vertexes_(vertexes) {
    }

    // Реализует метод Draw интерфейса svg::Drawable
    void Draw(svg::ObjectContainer &container) const override;

private:
    std::vector<svg::Point> vertexes_;
};

class Beam : public svg::Drawable {
public:
    Beam(Edge& e): e_(e) {
    }

    // Реализует метод Draw интерфейса svg::Drawable
    void Draw(svg::ObjectContainer &container) const override;

private:
    Edge e_;
};

void Visualization(std::ostream &, std::vector<svg::Point>, Edge &);

//void VisualizationPolygon(std::ostream &out, std::vector<svg::Point> vertexes);

//void VisualizationBeam(std::ostream &out, Edge &e);