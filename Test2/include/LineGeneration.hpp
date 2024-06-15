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

class LineGeneration
{
public:
    enum LineAlgorithmType
    {
        EMidpointLine,
        ESlopeComparison,
        EDDA,
        EBresenham,
        ELineGeneAlgorNum
    };

    LineGeneration(LineAlgorithmType type = EBresenham);
    ~LineGeneration();

    void setAlgorithmType(LineAlgorithmType type);
    LineAlgorithmType getAlgorithmType();

    bool drawLine(const glm::vec2& sp, const glm::vec2& ep);

    static string getLineAlgorName(LineAlgorithmType type);
private:
    LineAlgorithmType mType;
    static unordered_map<LineAlgorithmType, string> mLineAlgorToName;
};