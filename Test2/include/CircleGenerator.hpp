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

class CircleGenerator
{
public:
    enum CircleAlgorithmType
    {
        EMidpointCircle,
        ESlopeComparison,
        EDDA,
        EBresenham,
        ECircleGeneAlgorNum
    };

    CircleGenerator(CircleAlgorithmType type = EBresenham);
    ~CircleGenerator();

    void setAlgorithmType(CircleAlgorithmType type);
    CircleAlgorithmType getAlgorithmType();

    bool drawCircle(const glm::vec2& center, const float& radius);
    void drawSymmetryPoint(const glm::vec2& point);
    static string getCircleAlgorName(CircleAlgorithmType type);

private:
    CircleAlgorithmType mType;
    static unordered_map<CircleAlgorithmType, string> mCircleAlgorToName;
};