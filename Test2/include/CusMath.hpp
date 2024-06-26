/*
 * @Author: push-rush 1441488761@qq.com
 * @Date: 2024-04-19 11:03:10
 * @LastEditors: push-rush 1441488761@qq.com
 * @LastEditTime: 2024-04-19 11:27:21
 * @FilePath: /CG-CourseWorks/CourseWork3/include/CusMath.hpp
 * @Description: 自定义数学计算工具库
 */
#pragma once

#include <iostream>
#include <vector>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>

#ifdef _WIN32
    #include "../../External/glut-3.7.6-bin/glut.h"
#else
    #include <GL/freeglut.h>
#endif

#define ESC 27
#define Eplison glm::epsilon<float>()
#define PI 3.14159265358979323846

typedef std::vector<std::vector<float>> Matrixf;
typedef std::vector<std::vector<int>> Matrixi;
typedef std::vector<float> Vectorf;
typedef std::vector<int> Vectori;

static void cusPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLdouble xmin, xmax, ymin, ymax;
    
    ymax = zNear * tan(fovy * PI / 360.0);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}

struct Rect
{
    int mCenterX, mCenterY;
    int mWidth, mHeight;

    Rect()
    {
        mCenterX = 0, mCenterY = 0;
        mWidth = 25, mHeight = 25;
    }
};

struct Point
{
    float x;
    float y;

    bool mIsSelected = false;
    Rect mBBox;
    Point(float _x, float _y) : x(_x), y(_y) 
    {
        mBBox.mCenterX = x;
        mBBox.mCenterY = y;
    }

    void update()
    {
        if (mIsSelected)
        {
            mBBox.mCenterX = x;
            mBBox.mCenterY = y;
        }
    }
};

struct Segment
{
    Point p1;
    Point p2;
};

// 检测射线与线段是否相交，并计算交点
static bool raySegmentIntersection(Point rayStart, Point rayDir, Segment segment, Point& intersection) 
{
    // 计算射线和线段的参数方程
    float ray_dx = rayDir.x - rayStart.x;
    float ray_dy = rayDir.y - rayStart.y;
    float segment_dx = segment.p2.x - segment.p1.x;
    float segment_dy = segment.p2.y - segment.p1.y;

    // 计算射线与线段的交点参数 t 和 u
    float t_numer = (segment.p1.x - rayStart.x) * segment_dy - (segment.p1.y - rayStart.y) * segment_dx;
    float u_numer = (segment.p1.x - rayStart.x) * ray_dy - (segment.p1.y - rayStart.y) * ray_dx;
    float denom = ray_dx * segment_dy - ray_dy * segment_dx;

    // 如果射线和线段平行或共线
    if (denom == 0) 
    {
        if (t_numer == 0 && u_numer == 0) 
        {
            // 射线和线段共线
            return true;
        } 
        else 
        {
            // 射线和线段平行且不共线
            return false;
        }
    }

    // 计算交点参数 t 和 u
    float t = t_numer / denom;
    float u = u_numer / denom;

    // 如果 t 和 u 在合理范围内，则射线与线段相交
    if (t >= 0 && u >= 0 && u <= 1) 
        return true;
    return false;
}

// 计算阶乘的辅助函数
static unsigned long long factorial(int n) 
{
    if (n == 0 || n == 1) 
    {
        return 1;
    }
    unsigned long long result = 1;
    
    for (int i = 2; i <= n; ++i) 
    {
        result *= i;
    }
    return result;
}

// 计算二项式系数 C(n, k) = n! / (k! * (n - k)!)
static unsigned long long binomialCoefficient(int n, int k) 
{
    return factorial(n) / (factorial(k) * factorial(n - k));
}

// 矩阵乘法函数
template<typename TMat, typename TVec>
static TVec MatByVec(const TMat& M, const TVec& V) 
{
    int n = M.size();
    assert(V.size() == n);

    TVec result(n, 0.0f);
    for (int i = 0; i < n; ++i) 
    {
        for (int j = 0; j < n; ++j) 
        {
            result[i] += M[i][j] * V[j];
        }
    }
    return result;
}

// 两个矩阵相乘
template<typename TMat, typename TMat2 = Matrixf, typename TVec = Vectorf>
static TMat MatByMat(const TMat& m1, const TMat2& m2)
{
    int m = m1.size();
    int n = m1[0].size();
    int p = m2[0].size();
    assert(n == m2.size());

    // 创建结果矩阵
    TMat result(m, TVec(p, 0));

    for (int i = 0; i < m; ++i) 
    {
        for (int j = 0; j < p; ++j) 
        {
            for (int k = 0; k < n; ++k) 
            {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return result;
}

template<typename T>
static T transposeMatrix(const T& matrix) 
{
    // 获取矩阵的行数和列数
    size_t rows = matrix.size();
    size_t cols = matrix[0].size();

    // 创建一个新的矩阵来存储转置结果
    T transposed(cols, Vectorf(rows));

    // 执行转置操作
    for (size_t i = 0; i < rows; ++i) 
    {
        for (size_t j = 0; j < cols; ++j) 
        {
            transposed[j][i] = matrix[i][j];
        }
    }
    return transposed;
}

// 计算三角形的重心
static Point calculateTriangleCentroid(const Point& p1, const Point& p2, const Point& p3) 
{
    float cx = (p1.x + p2.x + p3.x) / 3.0;
    float cy = (p1.y + p2.y + p3.y) / 3.0;
    return {cx, cy};
}

// 计算多边形的重心
static Point calculatePolygonCentroid(const std::vector<Point>& polygon) 
{
    float centroidX = 0, centroidY = 0;
    float signedArea = 0;
    float x0, y0, x1, y1;
    float a;

    int numVertices = polygon.size();
    for (int i = 0; i < numVertices; ++i) 
    {
        x0 = polygon[i].x;
        y0 = polygon[i].y;
        x1 = polygon[(i + 1) % numVertices].x;
        y1 = polygon[(i + 1) % numVertices].y;
        a = x0 * y1 - x1 * y0;
        signedArea += a;
        centroidX += (x0 + x1) * a;
        centroidY += (y0 + y1) * a;
    }

    signedArea *= 0.5;
    centroidX /= (6.0 * signedArea);
    centroidY /= (6.0 * signedArea);

    return Point{float(centroidX), float(centroidY)};
}

static bool isPerfectSquare(int number) 
{
    // 计算给定数的开平方值
    double sqrtValue = std::sqrt(number);

    // 将开平方值向下取整
    int sqrtInt = std::floor(sqrtValue);

    // 判断开平方值是否等于原始数
    return (sqrtInt * sqrtInt == number);
}