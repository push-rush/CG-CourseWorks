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

static void cusPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
    GLfloat xmin, xmax, ymin, ymax;
    
    ymax = zNear * tan(fovy * PI / 360.0);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}

struct Point
{
    float x;
    float y;
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