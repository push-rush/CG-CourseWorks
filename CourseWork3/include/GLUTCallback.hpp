/*
 * @Author: push-rush 1441488761@qq.com
 * @Date: 2024-04-19 11:03:10
 * @LastEditors: push-rush 1441488761@qq.com
 * @LastEditTime: 2024-04-27 09:52:26
 * @FilePath: /CG-CourseWorks/CourseWork3/include/GLUTCallback.hpp
 * @Description: GLUT回调类
 */
#pragma once

#include <chrono>
#include <unordered_map>
#include <iostream>
using namespace std;

#include <GL/gl.h>
#include <GL/glu.h>

#ifdef _WIN32
    #include "../../External/glut-3.7.6-bin/glut.h"
#else
    #include <GL/freeglut.h>
#endif

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
    static bool mIsSetting;
    static int mCounter;

    static glm::vec2 mStartPoint;
    static glm::vec2 mEndPoint;
    static LineGeneration* mLineGeneration;
    static unordered_map<LineGeneration::LineAlgorithmType, TextButton*> mLineMapToButtons;
    static unordered_map<LineGeneration::LineAlgorithmType, glm::vec4> mLineAlgorToColors;

    static DrawingState mCurDrawingState;

    static PolygonScanConversion* mPolyScanConverter;
    static unordered_map<PolygonScanConversion::PloygonAlgorithmType, TextButton*> mPolygonMapToButtons;
    static bool mIsScanOver;
    static vector<Point> mScanPoints;
    static TextButton* sXAxisButton;
    static TextButton* sYAxisButton;
    
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