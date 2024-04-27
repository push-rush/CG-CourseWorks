/*
 * @Author: push-rush 1441488761@qq.com
 * @Date: 2024-04-19 11:03:10
 * @LastEditors: push-rush 1441488761@qq.com
 * @LastEditTime: 2024-04-19 11:27:21
 * @FilePath: /CG-CourseWorks/CourseWork3/include/CusMath.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

// #include "../../External/glut-3.7.6-bin/glut.h"

#define ESC 27
#define Eplison glm::epsilon<float>()
#define PI 3.14159265358979323846

static void cusPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLdouble xmin, xmax, ymin, ymax;
    
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