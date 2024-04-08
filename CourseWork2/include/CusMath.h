#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <glut.h>

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