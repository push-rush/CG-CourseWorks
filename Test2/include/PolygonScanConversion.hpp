#pragma once

// c++
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <stack>
using namespace std;

// glm
#include "../include/Vendor/glm/glm/glm.hpp"
#include "../include/Vendor/glm/glm/gtc/matrix_transform.hpp"

#include "./CusMath.hpp"

class PolygonScanConversion
{
public:
    // 用于存储像素的颜色
    struct Color 
    {
        float R, G, B;
    };

    struct Edge
    {
        float x;    // 扫描线与多边形交点的横坐标
        float dx;   // 下一条扫描线与多边形交点横坐标的偏移量/交点直线的斜率倒数
        int yMax;   // 焦点直线的上端点y值
        Edge* next; // 下一个交点信息

        Edge() : next(nullptr) { }
    };

    enum PloygonAlgorithmType
    {
        EPointByPoint,
        EScanLineConversion,
        EEdgePadding,
        EEdgeMark,
        EFloodFill,
        EAlgorithmNum
    };

    PloygonAlgorithmType mType;
    static unordered_map<PloygonAlgorithmType, string> mPolygonAlgorToName;
    
    PloygonAlgorithmType getAlgorithmType();
    void setAlgorithmType(PloygonAlgorithmType type);
    
    // 获取像素颜色
    Color getPixel(int x, int y);

private:
    vector<Point> mPoints;
    glm::vec4 mColor;

public:
    PolygonScanConversion(PloygonAlgorithmType type = EFloodFill);
    PolygonScanConversion(const vector<Point>& points, PloygonAlgorithmType type = EFloodFill);
    PolygonScanConversion(const vector<Point>& points, const glm::vec4& color, PloygonAlgorithmType type = EFloodFill);
    ~PolygonScanConversion();

    void draw();
    void setPolygon(const vector<Point>& points);
    void setColor(const glm::vec4& color);
    // 种子填充算法
    void floodFill(Point p, Color targetColor, Color fillColor, int xmin, int xmax, int ymin, int ymax, const std::vector<Segment>& segments);

    const glm::vec4 getPolyColor() const;
    vector<Point> getInputPoints() const;
    static string getPolygonAlgorName(PloygonAlgorithmType type);
};