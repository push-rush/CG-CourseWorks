#pragma once

#include <unordered_map>
#include <iostream>
using namespace std;

#include <GL/gl.h>
#include <GL/glu.h>

#include "../../GLUT/glut-3.7.6-bin/glut.h"

#include "./CusMath.h"
#include "./Curve.hpp"

class GLUTCallback
{
    static unordered_map<Curve::CurveType, string> mCurveNames;
    static unordered_map<Curve::CurveType, Curve::CurveType> mNameMapToCurves;

    static int mWinWidth;
    static int mWinHeight;
    static int mSubWinWidth;
    static int mSubWinHeight;
    static Curve::CurveType mCurCurveType;

public:
    static void setWinWidth(int w);
    static void setWinHeight(int h);
    static void setSubWinWidth(int w);
    static void setSubWinHeight(int h);

    static void setCurCurveType(Curve::CurveType type);

    static void myDisplay(void);
    static void mySubWinDisplay(void);
    static void myReshape(GLsizei w, GLsizei h);
    static void myKeyboardFunc(unsigned char key, int x, int y);
    static void mySubKeyboardFunc(unsigned char key, int x, int y);
    static void mySubMouseFunc(int button, int state, int x, int y);

    static void drawText(string text, const glm::vec3& pos);
};