#pragma once

// c++
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

// glm
#include "../include/Vendor/glm/glm/glm.hpp"
#include "../include/Vendor/glm/glm/gtc/matrix_transform.hpp"

#include "./CusMath.hpp"

#include "./Button.hpp"

class CurveSurfaceGenerator
{
public:
    enum CurveAlogrithmType
    {
        EHermiteCurve,
        EFergusonCurve,
        EBezierCurve,
        EBSplineCurve,
        ECurveAlgorNums
    };

public:
    CurveSurfaceGenerator(CurveAlogrithmType type = EBezierCurve);
    ~CurveSurfaceGenerator();

    void setAlgorithmType(CurveAlogrithmType type);
    CurveAlogrithmType getAlgorithmType();

    void draw();
    void setPoints(const vector<Point>& points);
    void setColor(const glm::vec4& color);

    // 计算贝塞尔曲线基矩阵M_n
    Matrixi calcBezierMatrix(int n);
    void update(const glm::vec2& mouse_pos, bool up = false);
    bool containPoint(Point pbox, const glm::vec2& point);
    void updateMousePos(const glm::vec2& pos);
    bool isLocking();
    void updateAdjParam();
    float getTParam();

    static string getCurveAlgorName(CurveAlogrithmType type);
    static TextButton* getParamAdjButton();

private:
    glm::vec4 mColor;
    vector<Point> mPoints;
    CurveAlogrithmType mType;
    glm::vec2 mMousePos;
    bool mIsDragging;
    float mTValue;

    static TextButton* sParamAdjButton;
    static unordered_map<CurveAlogrithmType, string> mCurveAlgorToName;
};