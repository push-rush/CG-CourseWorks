#pragma once

// c++
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

// glm
#include "../include/Vendor/glm/glm/glm.hpp"
#include "../include/Vendor/glm/glm/gtc/matrix_transform.hpp"

#include "./CusMath.hpp"

class PolygonScanConversion
{
private:
    vector<Point> mPoints;
    glm::vec4 mColor;

public:
    PolygonScanConversion();
    PolygonScanConversion(const vector<Point>& points);
    PolygonScanConversion(const vector<Point>& points, const glm::vec4& color);
    ~PolygonScanConversion();

    void draw();
    void setPolygon(const vector<Point>& points);
    void setColor(const glm::vec4& color);

    bool floodFill(Point p);

    const glm::vec4 getPolyColor() const;
};