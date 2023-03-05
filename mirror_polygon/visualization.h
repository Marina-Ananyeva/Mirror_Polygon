#pragma once

#include "svg.h"
#include "geometric_objects.h"

#include <vector>

using namespace svg;
using namespace shapes;
using namespace geo_objects;

class PolyGon : public svg::Drawable {
public:
    PolyGon(std::vector<geo_objects::Point> vertexes): vertexes_(vertexes) {
    }

    // Реализует метод Draw интерфейса svg::Drawable
    void Draw(svg::ObjectContainer &container) const override;

private:
    std::vector<geo_objects::Point> vertexes_;
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

class Beams : public svg::Drawable {
public:
    Beams(std::vector<geo_objects::Edge>& beams): beams_(beams) {
    }

    // Реализует метод Draw интерфейса svg::Drawable
    void Draw(svg::ObjectContainer &container) const override;

private:
    std::vector<geo_objects::Edge> beams_;
};

class Zigzag : public svg::Drawable {
public:
    Zigzag(std::vector<geo_objects::Point>& z): z_(z) {
    }

    // Реализует метод Draw интерфейса svg::Drawable
    void Draw(svg::ObjectContainer &container) const override;

private:
    std::vector<geo_objects::Point> z_;
};

void Visualization(std::ostream &, std::vector<geo_objects::Point>, Edge &);

void VisualizationBeam(std::ostream &out, Edge &e);

void VisualizationBeams(std::ostream &, std::vector<Edge>);

void VisualizationZigzag(std::ostream &, std::vector<geo_objects::Point>);

void VisualizationPolygon(std::ostream &out, std::vector<svg::Point> vertexes);

