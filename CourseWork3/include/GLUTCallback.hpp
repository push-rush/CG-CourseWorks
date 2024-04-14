#pragma once

#include <unordered_map>
#include <iostream>
using namespace std;

#include <GL/gl.h>
#include <GL/glu.h>

#include "../../GLUT/glut-3.7.6-bin/glut.h"

#include "./Button.hpp"
#include "./LineGeneration.hpp"
#include "./PolygonScanConversion.hpp"

class GLUTCallback
{
public:
    enum DrawingState
    {
        EDrawingLine,
        EDrawingPolygon,
        EDrawingStateNums
    };

private:
    static int mWinWidth;
    static int mWinHeight;
    static int mSubWinWidth;
    static int mSubWinHeight;
    static LineGeneration* mLineGeneration;
    static glm::vec2 mStartPoint;
    static glm::vec2 mEndPoint;
    static bool mIsSetting;
    static int mCounter;
    static unordered_map<LineGeneration::LineAlgorithmType, TextButton*> mLineMapToButtons;
    static unordered_map<LineGeneration::LineAlgorithmType, glm::vec4> mLineAlgorToColors;
    static DrawingState mCurDrawingState;
    static PolygonScanConversion* mPolyScanConverter;
    static bool mIsScanOver;
    static vector<Point> mScanPoints;
    
public:
    static void setWinWidth(int w);
    static void setWinHeight(int h);
    static void setSubWinWidth(int w);
    static void setSubWinHeight(int h);
    
    // 主窗口回调函数
    static void myDisplay(void);
    static void myReshape(GLsizei w, GLsizei h);
    static void myKeyboardFunc(unsigned char key, int x, int y);
    static void myMouseFunc(int button, int state, int x, int y);
    
    // 子窗口回调函数
    static void mySubWinDisplay(void);
    static void mySubKeyboardFunc(unsigned char key, int x, int y);
    static void mySubMouseFunc(int button, int state, int x, int y);
};