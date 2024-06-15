#include "../include/GLUTCallback.hpp"

int GLUTCallback::mWinWidth = 1920;
int GLUTCallback::mWinHeight = 1080;

int GLUTCallback::mSubWinWidth = 1920;
int GLUTCallback::mSubWinHeight = 1080;

Point GLUTCallback::mBorder = Point{1200, 768};
Point GLUTCallback::mMousePos = Point{0, 0};
bool GLUTCallback::mIsMouseLeft = false;

glm::vec2 GLUTCallback::mLineStartPoint = glm::vec2{0.0f, 0.0f};
glm::vec2 GLUTCallback::mLineEndPoint = glm::vec2{0.0f, 0.0f};
// 创建直线实例
LineGeneration* GLUTCallback::mLineGeneration = new LineGeneration();

// 创建圆实例
CircleGenerator* GLUTCallback::mCircleGenerator = new CircleGenerator();
glm::vec2 GLUTCallback::mCircleCenter = glm::vec2{0.0f, 0.0f};
float GLUTCallback::mCircleRadius = 0.0f;

// 创建曲线实例
CurveSurfaceGenerator* GLUTCallback::mCurveGenerator = new CurveSurfaceGenerator();

int GLUTCallback::mCounter = 0;
bool GLUTCallback::mIsSetting = false;

unordered_map<LineGeneration::LineAlgorithmType, TextButton*> GLUTCallback::mLineMapToButtons = {
    {LineGeneration::EDDA, new TextButton()},
    {LineGeneration::EMidpointLine, new TextButton()},
    {LineGeneration::ESlopeComparison, new TextButton()},
    {LineGeneration::EBresenham, new TextButton()},
    // {(LineGeneration::LineAlgorithmType)(Button::EPointByPointComparison), new TextButton()},
};

unordered_map<LineGeneration::LineAlgorithmType, glm::vec4> GLUTCallback::mLineAlgorToColors = {
    {LineGeneration::EDDA, glm::vec4{0.25f, 0.25f, 0.75f, 1.0f}},
    {LineGeneration::EMidpointLine, glm::vec4{0.65f, 0.25f, 0.25f, 1.0f}},
    {LineGeneration::ESlopeComparison, glm::vec4{0.25f, 0.65f, 0.25f, 1.0f}},
    {LineGeneration::EBresenham, glm::vec4{0.75f, 0.75f, 0.35f, 1.0f}},
    {(LineGeneration::LineAlgorithmType)(Button::EPointByPointComparison), glm::vec4{0.35f, 0.45f, 0.85f, 1.0f}},
};

unordered_map<PolygonScanConversion::PloygonAlgorithmType, TextButton*> GLUTCallback::mPolygonMapToButtons = {
    {PolygonScanConversion::EPointByPoint, new TextButton()},
    {PolygonScanConversion::EScanLineConversion, new TextButton()},
    {PolygonScanConversion::EEdgePadding, new TextButton()},
    {PolygonScanConversion::EEdgeMark, new TextButton()},
    {PolygonScanConversion::EFloodFill, new TextButton()},
};

unordered_map<CircleGenerator::CircleAlgorithmType, TextButton*> GLUTCallback::mCircleMapToButtons = {
    {CircleGenerator::EDDA, new TextButton()},
    {CircleGenerator::EMidpointCircle, new TextButton()},
    {CircleGenerator::ESlopeComparison, new TextButton()},
    {CircleGenerator::EBresenham, new TextButton()},
    {(CircleGenerator::CircleAlgorithmType)(Button::EPointByPointComparison), new TextButton()},
};

unordered_map<CircleGenerator::CircleAlgorithmType, glm::vec4> GLUTCallback::mCircleAlgorToColors = {
    {CircleGenerator::EDDA, glm::vec4{0.25f, 0.25f, 0.75f, 1.0f}},
    {CircleGenerator::EMidpointCircle, glm::vec4{0.65f, 0.25f, 0.25f, 1.0f}},
    {CircleGenerator::ESlopeComparison, glm::vec4{0.25f, 0.65f, 0.25f, 1.0f}},
    {CircleGenerator::EBresenham, glm::vec4{0.75f, 0.75f, 0.35f, 1.0f}},
    {(CircleGenerator::CircleAlgorithmType)(Button::EPointByPointComparison), glm::vec4{0.35f, 0.45f, 0.85f, 1.0f}},
};

unordered_map<CurveSurfaceGenerator::CurveAlogrithmType, TextButton*> GLUTCallback::mCurveMapToButtons = {
    {CurveSurfaceGenerator::EHermiteCurve, new TextButton()},
    {CurveSurfaceGenerator::EFergusonCurve, new TextButton()},
    {CurveSurfaceGenerator::EBezierCurve, new TextButton()},
    {CurveSurfaceGenerator::EBSplineCurve, new TextButton()},
};

unordered_map<CurveSurfaceGenerator::CurveAlogrithmType, glm::vec4> mCurveAlgorToColors = {
    {CurveSurfaceGenerator::EHermiteCurve, glm::vec4{0.25f, 0.25f, 0.75f, 1.0f}},
    {CurveSurfaceGenerator::EFergusonCurve, glm::vec4{0.65f, 0.25f, 0.25f, 1.0f}},
    {CurveSurfaceGenerator::EBezierCurve, glm::vec4{0.25f, 0.65f, 0.25f, 1.0f}},
    {CurveSurfaceGenerator::EBSplineCurve, glm::vec4{0.75f, 0.75f, 0.35f, 1.0f}},
};

GLUTCallback::DrawingState GLUTCallback::mCurDrawingState = GLUTCallback::EDrawingLine;

PolygonScanConversion* GLUTCallback::mPolyScanConverter = new PolygonScanConversion();

bool GLUTCallback::mIsScanOver = false;

vector<Point> GLUTCallback::mScanPoints{};

// 起点坐标图像中心（0，0）
TextButton* GLUTCallback::sXAxisButton = new TextButton("End X: Input x value...");
TextButton* GLUTCallback::sYAxisButton = new TextButton("End Y: Input y value...");
TextButton* GLUTCallback::sRadiusButton = new TextButton("Radius: Input r value...");
TextButton* GLUTCallback::sModeSwitchButton = new TextButton("SwitchMode: ");
TextButton* GLUTCallback::sMousePosButton = new TextButton("Pos: ");
// TextButton* GLUTCallback::sParamAdjButton = new TextButton("t:");

unordered_map<GLUTCallback::DrawingState, std::string> GLUTCallback::mDrawingMapToNames = {
    {GLUTCallback::EDrawingLine, "Line"},
    {GLUTCallback::EDrawingPolygon, "Polygon"},
    {GLUTCallback::EDrawingCircle, "Circle"},
    {GLUTCallback::EDrawingCurveSurface, "Curve or CurveSurface"}
};

void GLUTCallback::myDisplay(void)
{
    // 设置缓存区背景色
    glClearColor(0.10f, 0.25f, 0.28f, 1.0f);

    // 清空颜色/深度/模板缓存区
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // 设置绘制颜色
    glColor4f(0.75f, 0.75f, 0.75f, 1.0f);

    // 设置线框绘制
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 设置点大小
    glPointSize(5);

    int half_w = mWinWidth / 2;
    int half_h = mWinHeight / 2;

    glLineWidth(2);
    glBegin(GL_LINES);
        glVertex2f(-mBorder.x * 0.5f, 0);
        glVertex2f(mBorder.x * 0.5f, 0);
        glVertex2f(0, mBorder.y * 0.5f);
        glVertex2f(0, -mBorder.y * 0.5f);
    glEnd();

    glLineWidth(1);
    glBegin(GL_LINES);
    for (int i = 0; i <= mBorder.x * 0.5f; i += 20)
    {
        glVertex2i(i, 0);
        glVertex2i(i, (i % 100 != 0 ? 10 : 20));
    }
    for (int i = 0; i >= -mBorder.x * 0.5f; i -= 20)
    {
        glVertex2i(i, 0);
        glVertex2i(i, (i % 100 != 0 ? 10 : 20));
    }

    for (int i = 0; i <= mBorder.y * 0.5f; i += 20)
    {
        glVertex2i(0, i);
        glVertex2i((i % 100 != 0 ? 10 : 20), i);
    }
    for (int i = 0; i >= -mBorder.y * 0.5f; i -= 20)
    {
        glVertex2i(0, i);
        glVertex2i((i % 100 != 0 ? 10 : 20), i);
    }
    glEnd();

    for (int i = 100; i <= mBorder.x * 0.5f; i += 100)
    {
        drawText(to_string(i), glm::vec2{i - 14, -18});
    }
    for (int i = 0; i >= -mBorder.x * 0.5f; i -= 100)
    {
        drawText(to_string(i), glm::vec2{i - 14, -18});
    }

    for (int i = 100; i <= mBorder.y * 0.5f; i += 100)
    {
        drawText(to_string(i), glm::vec2{-40, i - 7});
    }
    for (int i = -100; i >= -mBorder.y * 0.5f; i -= 100)
    {
        drawText(to_string(i), glm::vec2{-40, i - 7});
    }

    if (!mIsMouseLeft && 
        !(sXAxisButton->isSelected() || sYAxisButton->isSelected() || sRadiusButton->isSelected() || 
            mCurveGenerator->getParamAdjButton()->isSelected())
    )
    {
        drawText("(" + to_string(int(mMousePos.x)) + ", " + to_string(int(mMousePos.y)) + ")", glm::vec2{mMousePos.x + 20, mMousePos.y + 20});
    }

    // 退出提示符绘制
    glm::vec4 rect_pos{half_w - 200, half_h - 10, half_w - 10, half_h - 40};
    glRectf(rect_pos.x, rect_pos.y, rect_pos.z, rect_pos.w);

    // 绘制字体
    glm::vec2 pos = glm::vec2{half_w - 190, half_h - 30};
    drawText("Quit: Press ESC...", pos);

    // 绘制x轴和y轴坐标点信息输入框
    glm::vec2 x_bt_pos = {-450, half_h - 35};
    glm::vec2 y_bt_pos = {-100, half_h - 35};
    glm::vec2 radius_bt_pos = {-275, half_h - 100};

    glm::vec2 xy_sz = glm::vec2{135.0f, 50.0f};
    
    // 设置线宽和颜色
    sXAxisButton->setLineWidth(3);
    sYAxisButton->setLineWidth(3);
    sRadiusButton->setLineWidth(3);
    // sMousePosButton->setLineWidth(3);

    sXAxisButton->setColor(glm::vec4{0.55f, 0.55f, 0.55f, 1.0f});
    sYAxisButton->setColor(glm::vec4{0.55f, 0.55f, 0.55f, 1.0f});
    sRadiusButton->setColor(glm::vec4{0.55f, 0.55f, 0.55f, 1.0f});
    // sMousePosButton->setColor(glm::vec4{0.85f, 0.85f, 0.85f, 1.0f});
    
    if (sXAxisButton)
    {
        glm::vec2 x_tx_pos = glm::vec2{x_bt_pos.x - xy_sz.x * 0.5f - 60.0f, x_bt_pos.y + xy_sz.y * 0.25f - 15.0f};
        sXAxisButton->setCenter(x_bt_pos);
        sXAxisButton->setSize(xy_sz);
        sXAxisButton->setFontPos(x_tx_pos);

        if (sXAxisButton->getInputString().length() > 0)
        {
            drawText(sXAxisButton->getInputString(), glm::vec2{x_bt_pos.x - xy_sz.x * 0.5f + 2.0f, x_bt_pos.y - 6.0f}, 24);
        }

        if (sXAxisButton->isSelected())
        {
            sXAxisButton->setColor(glm::vec4{0.85f, 0.85f, 0.85f, 1.0f});
            sXAxisButton->setText("End X: ");
        }
        else if (sXAxisButton->getInputState() && sYAxisButton->getInputState())
        {
            sXAxisButton->setColor(glm::vec4{0.55f, 0.55f, 0.55f, 1.0f});
            sXAxisButton->setText("End X: Input x value...");
        }
        sXAxisButton->draw();
    }
    if (sYAxisButton)
    {
        glm::vec2 y_tx_pos = glm::vec2{y_bt_pos.x - xy_sz.x * 0.5f - 60.0f, y_bt_pos.y + xy_sz.y * 0.25f - 15.0f};
        sYAxisButton->setFontPos(y_tx_pos);
        sYAxisButton->setCenter(y_bt_pos);
        sYAxisButton->setSize(xy_sz);

        if (sYAxisButton->getInputString().length() > 0)
        {
            drawText(sYAxisButton->getInputString(), glm::vec2{y_bt_pos.x - xy_sz.x * 0.5f + 2.0f, y_bt_pos.y - 6.0f}, 24);
        }

        if (sYAxisButton->isSelected())
        {
            sYAxisButton->setColor(glm::vec4{0.85f, 0.85f, 0.85f, 1.0f});
            sYAxisButton->setText("End Y: ");
        }
        else if (sXAxisButton->getInputState() && sYAxisButton->getInputState())
        {
            sYAxisButton->setColor(glm::vec4{0.55f, 0.55f, 0.55f, 1.0f});
            sYAxisButton->setText("End Y: Input y value...");
        }

        sYAxisButton->draw();
    }
    if (sRadiusButton)
    {
        glm::vec2 radius_tx_pos = glm::vec2{radius_bt_pos.x - xy_sz.x * 0.5f - 65.0f, radius_bt_pos.y + xy_sz.y * 0.25f - 15.0f};
        sRadiusButton->setCenter(radius_bt_pos);
        sRadiusButton->setSize(xy_sz);
        sRadiusButton->setFontPos(radius_tx_pos);

        if (sRadiusButton->getInputString().length() > 0)
        {
            drawText(sRadiusButton->getInputString(), glm::vec2{radius_bt_pos.x - xy_sz.x * 0.5f + 2.0f, radius_bt_pos.y - 6.0f}, 24);
        }

        if (sRadiusButton->isSelected())
        {
            sRadiusButton->setColor(glm::vec4{0.85f, 0.85f, 0.85f, 1.0f});
            sRadiusButton->setText("Radius: ");
        }
        else
        {
            sRadiusButton->setColor(glm::vec4{0.55f, 0.55f, 0.55f, 1.0f});
            sRadiusButton->setText("Radius: Input r value...");
        }

        sRadiusButton->draw();
    }

    // 绘制直线/多边形/圆/曲线
    switch (mCurDrawingState)
    {   
        case EDrawingLine:
        {   // 绘制直线
            // 设置直线颜色
            glm::vec4 color = mLineAlgorToColors[mLineGeneration->getAlgorithmType()];
            glColor4f(color.x, color.y, color.z, color.w);

            // 打个时间戳
            auto beforeTime = std::chrono::steady_clock::now();
	
            // 绘制邻近直线的像素点
            glBegin(GL_POINTS);
                mLineGeneration->drawLine(mLineStartPoint, mLineEndPoint);
            glEnd();
    
    	    auto afterTime = std::chrono::steady_clock::now();
            
            float dt = std::chrono::duration<double, std::milli>(afterTime - beforeTime).count();

            // LineGeneration::LineAlgorithmType type = mLineGeneration->getAlgorithmType();
            // switch (type)
            // {
            //     case LineGeneration::EDDA:
            //     {
            //         cout << "DDA algor delay: " << dt << "ms" << "\n";
            //         break;
            //     }    
            //     case LineGeneration::EBresenham:
            //     {
            //         cout << "Bresenham algor delay: " << dt << "ms" << "\n";
            //     }
            //     default:
            //         break;
            // }
            break;
        }
        case EDrawingPolygon:
        {   // 多边形扫描转换
            // 设置颜色
            glColor4f(0.25f, 0.30f, 0.75f, 1.0f);

            // 设置点大小
            glPointSize(8);
            if ((int)mScanPoints.size() > 0)
            {
                int sz = mScanPoints.size();
                glBegin(GL_POINTS);
                    for (int i = 0; i < sz; i++)
                    {
                        glVertex2i(mScanPoints[i].x, mScanPoints[i].y);
                    }
                glEnd();
            }

            glPointSize(1);
            glBegin(GL_POINTS);
                mPolyScanConverter->draw();
            glEnd();
            break;
        }
        case EDrawingCircle:
        {   // 绘制圆形
            // 设置颜色
            glm::vec4 color = mCircleAlgorToColors[mCircleGenerator->getAlgorithmType()];
            glColor4f(color.x, color.y, color.z, color.w);

            // 绘制邻近直线的像素点
            glBegin(GL_POINTS);
                mCircleGenerator->drawCircle(mCircleCenter, mCircleRadius);
            glEnd();

            break;
        }
        case EDrawingCurveSurface:
        {   // 绘制曲线/曲面
            // 设置颜色
            glColor4f(0.25f, 0.30f, 0.75f, 1.0f);

            // 设置点大小
            // 设置点大小
            glPointSize(12);
            if ((int)mScanPoints.size() > 0)
            {
                int sz = mScanPoints.size();
                glBegin(GL_POINTS);
                    for (int i = 0; i < sz; i++)
                    {
                        glVertex2i(mScanPoints[i].x, mScanPoints[i].y);
                    }
                glEnd();
            }

            // glPointSize(4);
            // 设置颜色
            // glColor4f(0.75f, 0.30f, 0.35f, 1.0f);
            // glBegin(GL_POINTS);
            // 绘制曲线
            mCurveGenerator->draw();
            // glEnd();

            // 绘制参数调整按钮
            TextButton* paramAdjButton = mCurveGenerator->getParamAdjButton();
            if (paramAdjButton)
            {
                paramAdjButton->setCenter(glm::vec2{-275, half_h - 165});
                paramAdjButton->setSize(xy_sz);

                glm::vec2 param_tx_pos = glm::vec2{paramAdjButton->getCenter().x - xy_sz.x * 0.5f - 18.0f, paramAdjButton->getCenter().y + xy_sz.y * 0.25f - 15.0f};
                paramAdjButton->setFontPos(param_tx_pos);
                
                if (paramAdjButton->getInputString().length() > 0)
                {
                    drawText(paramAdjButton->getInputString(), glm::vec2{paramAdjButton->getCenter().x - xy_sz.x * 0.5f + 2.0f, paramAdjButton->getCenter().y - 6.0f}, 24);
                }

                if (paramAdjButton->isSelected())
                {
                    paramAdjButton->setColor(glm::vec4{0.85f, 0.85f, 0.85f, 1.0f});
                    paramAdjButton->setText("T: ");
                }
                else
                {
                    paramAdjButton->setColor(glm::vec4{0.55f, 0.55f, 0.55f, 1.0f});
                    paramAdjButton->setText("T: Adjust t value...");
                }
                paramAdjButton->draw();
            }
        }
        default:
            break;
    }
	
    // 前后缓存区交换(双缓存机制)
    glutSwapBuffers();
}

void GLUTCallback::mySubWinDisplay(void)
{
    // 设置缓存区背景色
    glClearColor(0.05f, 0.15f, 0.18f, 1.0f);

    // 清空颜色缓存区
	glClear(GL_COLOR_BUFFER_BIT);

    // 设置线框绘制
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 设置绘制颜色
    glColor4f(0.75f, 0.75f, 0.75f, 1.0f);

    // 设置线宽
    glLineWidth(3);

    // 提示
    int half_w = mSubWinWidth / 2;
    int half_h = mSubWinHeight / 2;

    // 退出提示符绘制
    glm::vec4 rect_pos{-half_w + 5, half_h - 10, half_w - 10, half_h - 50};
    glRectf(rect_pos.x, rect_pos.y, rect_pos.z, rect_pos.w);
    
    // 绘制字体
    glm::vec2 pos = glm::vec2{-half_w + 10, half_h - 35};
    drawText("Click line or polygon gene algor...", pos);

    // TextButton* switchModeButton = mLineMapToButtons[(LineGeneration::LineAlgorithmType)(Button::EPointByPointComparison)];
    if (sModeSwitchButton)
    {   // 绘制模式切换button
        if (sModeSwitchButton->isSelected())
        {   // 更新绘制模式
            mCurDrawingState = (DrawingState)((int)(mCurDrawingState + 1) % EDrawingStateNums);
            sModeSwitchButton->setIsSelected(false);
        }

        glm::vec2 cent = glm::vec2{(rect_pos.x + rect_pos.z) * 0.5f, pos.y} - glm::vec2{0.0f, 60.0f};
        glm::vec2 sz = glm::vec2{rect_pos.z - rect_pos.x, rect_pos.y - rect_pos.w};
        sModeSwitchButton->setCenter(cent);
        sModeSwitchButton->setSize(sz);
        
        // 设置线宽和颜色
        sModeSwitchButton->setLineWidth(4);
        sModeSwitchButton->setColor(mLineAlgorToColors[LineGeneration::LineAlgorithmType(Button::EPointByPointComparison)]);

        // 设置文本位置
        glm::vec2 text_pos = glm::vec2{cent.x - sz.x * 0.5f + 10.0f, cent.y - 6.0f};
        sModeSwitchButton->setFontPos(text_pos);

        // 设置文本内容
        string mode_text = mDrawingMapToNames[mCurDrawingState];
        sModeSwitchButton->setText("SwitchMode: " + mode_text);

        // 绘制
        sModeSwitchButton->draw();
    }

    // cout << "state: " << mCurDrawingState << "\n";
    switch (mCurDrawingState)
    {
        case EDrawingLine:
        {
            for (int i = 1; i < LineGeneration::ELineGeneAlgorNum; i++)
            {
                glm::vec2 lt = {-half_w + 5, half_h - (i + 1) * 80 - 15};
                glm::vec2 rb = {half_w - 10, half_h - (i + 2) * 80};
                
                if (mLineMapToButtons.count((LineGeneration::LineAlgorithmType)i))
                {
                    TextButton* b = mLineMapToButtons[(LineGeneration::LineAlgorithmType)i];
                    b->setCenter((lt + rb) * 0.5f);
                    b->setSize(glm::vec2{rb.x - lt.x, lt.y - rb.y});
                    
                    if (b->isSelected())
                    {
                        mLineGeneration->setAlgorithmType((LineGeneration::LineAlgorithmType)i);
                        // 高亮显示当前选中直线生成算法
                        b->setLineWidth(5);
                        b->setColor(glm::vec4(0.45f, 0.45f, 0.75f, 1.0f));
                    }
                    else if (mLineGeneration->getAlgorithmType() == (LineGeneration::LineAlgorithmType)i)
                    {
                        b->setIsSelected(true);
                    }
                    else
                    {
                        b->setLineWidth(2);
                        b->setColor(glm::vec4(0.25f, 0.25f, 0.35f, 1.0f));
                    }

                    // 设置文本位置
                    glm::vec3 text_pos = glm::vec3{lt.x + 10, (lt.y + rb.y) * 0.5 + 10, 0.0f};
                    b->setFontPos(text_pos);

                    // 设置文本内容
                    b->setText(LineGeneration::getLineAlgorName((LineGeneration::LineAlgorithmType)i));

                    // 绘制
                    b->draw();
                }
            }
            break;
        }
        case EDrawingPolygon:
        {
            for (int i = 1; i < PolygonScanConversion::EAlgorithmNum; i++)
            {
                glm::vec2 lt = {-half_w + 5, half_h - (i + 1) * 60 - 15};
                glm::vec2 rb = {half_w - 10, half_h - (i + 2) * 60};
                
                if (mPolygonMapToButtons.count((PolygonScanConversion::PloygonAlgorithmType)i))
                {
                    TextButton* b = mPolygonMapToButtons[(PolygonScanConversion::PloygonAlgorithmType)i];
                    b->setCenter((lt + rb) * 0.5f);
                    b->setSize(glm::vec2{rb.x - lt.x, lt.y - rb.y});
                    
                    if (b->isSelected())
                    {
                        mPolyScanConverter->setAlgorithmType((PolygonScanConversion::PloygonAlgorithmType)i);
                        // 高亮显示当前选中直线生成算法
                        b->setLineWidth(5);
                        b->setColor(glm::vec4(0.45f, 0.45f, 0.75f, 1.0f));
                    }
                    else if (mPolyScanConverter->getAlgorithmType() == (PolygonScanConversion::PloygonAlgorithmType)i)
                    {
                        b->setIsSelected(true);
                    }
                    else
                    {
                        b->setLineWidth(2);
                        b->setColor(glm::vec4(0.25f, 0.25f, 0.35f, 1.0f));
                    }

                    // 设置文本位置
                    glm::vec3 text_pos = glm::vec3{lt.x + 10, (lt.y + rb.y) * 0.5 - 10, 0.0f};
                    b->setFontPos(text_pos);

                    // 设置文本内容
                    b->setText(PolygonScanConversion::getPolygonAlgorName((PolygonScanConversion::PloygonAlgorithmType)i));

                    // 绘制
                    b->draw();
                }
            }
            break;
        }
        case EDrawingCircle:
        {
            for (int i = 1; i < CircleGenerator::ECircleGeneAlgorNum; i++)
            {
                glm::vec2 lt = {-half_w + 5, half_h - (i + 1) * 60 - 15};
                glm::vec2 rb = {half_w - 10, half_h - (i + 2) * 60};
                
                if (mCircleMapToButtons.count((CircleGenerator::CircleAlgorithmType)i))
                {
                    TextButton* b = mCircleMapToButtons[(CircleGenerator::CircleAlgorithmType)i];
                    b->setCenter((lt + rb) * 0.5f);
                    b->setSize(glm::vec2{rb.x - lt.x, lt.y - rb.y});
                    
                    if (b->isSelected())
                    {
                        mCircleGenerator->setAlgorithmType((CircleGenerator::CircleAlgorithmType)i);
                        // 高亮显示当前选中直线生成算法
                        b->setLineWidth(5);
                        b->setColor(glm::vec4(0.45f, 0.45f, 0.75f, 1.0f));
                    }
                    else if (mCircleGenerator->getAlgorithmType() == (CircleGenerator::CircleAlgorithmType)i)
                    {
                        b->setIsSelected(true);
                    }
                    else
                    {
                        b->setLineWidth(2);
                        b->setColor(glm::vec4(0.25f, 0.25f, 0.35f, 1.0f));
                    }

                    // 设置文本位置
                    glm::vec3 text_pos = glm::vec3{lt.x + 10, (lt.y + rb.y) * 0.5 - 10, 0.0f};
                    b->setFontPos(text_pos);

                    // 设置文本内容
                    b->setText(CircleGenerator::getCircleAlgorName((CircleGenerator::CircleAlgorithmType)i));

                    // 绘制
                    b->draw();
                }
            }
            break;
        }
        case EDrawingCurveSurface:
        {
            for (int i = 1; i < CurveSurfaceGenerator::ECurveAlgorNums; i++)
            {
                glm::vec2 lt = {-half_w + 5, half_h - (i + 1) * 60 - 15};
                glm::vec2 rb = {half_w - 10, half_h - (i + 2) * 60};
                
                if (mCurveMapToButtons.count((CurveSurfaceGenerator::CurveAlogrithmType)i))
                {
                    TextButton* b = mCurveMapToButtons[(CurveSurfaceGenerator::CurveAlogrithmType)i];
                    b->setCenter((lt + rb) * 0.5f);
                    b->setSize(glm::vec2{rb.x - lt.x, lt.y - rb.y});
                    
                    if (b->isSelected())
                    {
                        mCurveGenerator->setAlgorithmType((CurveSurfaceGenerator::CurveAlogrithmType)i);
                        // 高亮显示当前选中直线生成算法
                        b->setLineWidth(5);
                        b->setColor(glm::vec4(0.45f, 0.45f, 0.75f, 1.0f));
                    }
                    else if (mCurveGenerator->getAlgorithmType() == (CurveSurfaceGenerator::CurveAlogrithmType)i)
                    {
                        b->setIsSelected(true);
                    }
                    else
                    {
                        b->setLineWidth(2);
                        b->setColor(glm::vec4(0.25f, 0.25f, 0.35f, 1.0f));
                    }

                    // 设置文本位置
                    glm::vec3 text_pos = glm::vec3{lt.x + 10, (lt.y + rb.y) * 0.5 - 10, 0.0f};
                    b->setFontPos(text_pos);

                    // 设置文本内容
                    b->setText(CurveSurfaceGenerator::getCurveAlgorName((CurveSurfaceGenerator::CurveAlogrithmType)i));

                    // 绘制
                    b->draw();
                }
            }
            break;
        }
        default:
            break;
    }
    
    // 前后缓存区交换(双缓存机制)
    glutSwapBuffers();
}

void GLUTCallback::myReshape(GLsizei w, GLsizei h)
{
    // 设置视口
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // 透视投影
    // cusPerspective(glm::radians(45.0f), (double)(w / h), -1.0f, 1000.0f);
    
    int half_w = w / 2;
    int half_h = h / 2;
    
    // 正交投影
    glOrtho(-half_w, half_w, -half_h, half_h, -1.0f, 1000.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLUTCallback::myKeyboardFunc(unsigned char key, int x, int y)
{
    cout << "Main window key input: " << (int)key << "\n";

    switch (key)
    {
        case ESC:
        {
            exit(0);
            break;
        }
        case GLUT_KEY_DOWN:
        case GLUT_ENTERED:
        case 32:
        {
            mIsScanOver = true;
            switch (mCurDrawingState)
            {
                case EDrawingPolygon:
                {
                    mPolyScanConverter->setPolygon(mScanPoints);
                    mScanPoints.clear();
                    break;
                }
                case EDrawingCurveSurface:
                {
                    mCurveGenerator->setPoints(mScanPoints);
                    mScanPoints.clear();
                    break;
                }
                default:
                    break;
            }
            break;
        }
        default:
        {
            break;
        }
    }

    if (sXAxisButton->isSelected())
    {
        sXAxisButton->processInput(key);
    }
    else if (sYAxisButton->isSelected())
    {
        sYAxisButton->processInput(key);
    }
    else if (sRadiusButton->isSelected())
    {
        sRadiusButton->processInput(key);

        if (sRadiusButton->getInputState())
        {
            mCircleRadius= stoi(sRadiusButton->getInputString());
            sRadiusButton->clearInput();
        }
    }
    else if (mCurDrawingState == EDrawingCurveSurface && 
        mCurveGenerator->getParamAdjButton()->isSelected()
    )
    {
        mCurveGenerator->getParamAdjButton()->processInput(key);
        if (mCurveGenerator->getParamAdjButton()->getInputState())
        {
            mCurveGenerator->updateAdjParam();
        }
    }
    
    if (sXAxisButton->isSelected() || sYAxisButton->isSelected())
    {
        if (sXAxisButton->getInputState() && sYAxisButton->getInputState())
        {
            if ((mCounter++) % 2 == 0)
            {
                mLineStartPoint = glm::vec2{stoi(sXAxisButton->getInputString()), stoi(sYAxisButton->getInputString())};
            }
            else
            {
                mLineEndPoint = glm::vec2{stoi(sXAxisButton->getInputString()), stoi(sYAxisButton->getInputString())};
            }
            
            cout << "clear input..." << "\n";
            sXAxisButton->clearInput();
            sYAxisButton->clearInput();
        }
    }

    if (mCurDrawingState == EDrawingCircle)
    {
        if (sXAxisButton->getInputState() && sYAxisButton->getInputState())
        {
            mCircleCenter = glm::vec2{stoi(sXAxisButton->getInputString()), stoi(sYAxisButton->getInputString())};
            sXAxisButton->clearInput();
            sYAxisButton->clearInput();
        }
    }
}

void GLUTCallback::mySubKeyboardFunc(unsigned char key, int x, int y)
{
    cout << "Key: " << key << "\n";

    switch (key)
    {
        case ESC:
        {
            break;
        }
        default:
        {
            break;
        }
    }
}

void GLUTCallback::myMouseFunc(int button, int state, int x, int y)
{
    glm::vec2 mouse_pos = glm::vec2(float(x), float(y));
    mouse_pos.x -= mWinWidth * 0.5f;
    mouse_pos.y = mWinHeight * 0.5f - mouse_pos.y;
    mMousePos = Point(mouse_pos.x, mouse_pos.y);

    if (state == GLUT_DOWN)
    {
        // 更新x轴输入
        sXAxisButton->update(mouse_pos);

        // 更新y轴输入
        sYAxisButton->update(mouse_pos);

        // 更新圆半径
        sRadiusButton->update(mouse_pos);

        if (mCurDrawingState == EDrawingCurveSurface)
        {
            mCurveGenerator->getParamAdjButton()->update(mouse_pos);
        }
    }

    if (sXAxisButton->isSelected() || sYAxisButton->isSelected() ||
        sRadiusButton->isSelected() || mCurveGenerator->getParamAdjButton()->isSelected()
    )
    {
        return;
    }
    else if (state == GLUT_DOWN)
    {
        switch (mCurDrawingState)
        {
            case EDrawingLine:
            {
                if (mCounter % 2 != 0)
                {
                    if (!mIsSetting)
                        mLineStartPoint = glm::vec2(mouse_pos.x, mouse_pos.y), mIsSetting = true;
                    else
                        mLineEndPoint = glm::vec2(mouse_pos.x, mouse_pos.y), mIsSetting = false;
                }
                mCounter++;
                break;
            }
            case EDrawingPolygon:
            case EDrawingCurveSurface:
            {
                mCurveGenerator->update(mouse_pos);
                mCurveGenerator->updateMousePos(mouse_pos);
                if (!mCurveGenerator->isLocking())
                {
                    mScanPoints.emplace_back(Point{mouse_pos.x, mouse_pos.y});
                }
                break;
            }
            case EDrawingCircle:
            {
                mCircleCenter = glm::vec2{mouse_pos.x, mouse_pos.y};
                break;
            }
            default:
                break;
        }
    }
    else if (state == GLUT_UP)
    {
        switch (mCurDrawingState)
        {
            case EDrawingCurveSurface:
            {
                mCurveGenerator->update(glm::vec2{0, 0}, true);
                break;
            }
            default:
                break;
        }
    }
}

void GLUTCallback::mySubMouseFunc(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        glm::vec2 mouse_pos = glm::vec2(float(x), float(y));
        mouse_pos.x -= mSubWinWidth * 0.5f;
        mouse_pos.y = mSubWinHeight * 0.5f - mouse_pos.y;

        // 更新模式切换按钮状态
        sModeSwitchButton->update(mouse_pos);

        switch (mCurDrawingState)
        {
            case EDrawingLine:
            {
                for (auto it = mLineMapToButtons.begin(); it != mLineMapToButtons.end(); it++)
                {   // 更新鼠标输入后button状态
                    (*it).second->update(mouse_pos);
                }
                break;
            }
            case EDrawingPolygon:
            {
                for (auto it = mPolygonMapToButtons.begin(); it != mPolygonMapToButtons.end(); it++)
                {   
                    (*it).second->update(mouse_pos);
                }
                break;
            }
            case EDrawingCircle:
            {
                for (auto it = mCircleMapToButtons.begin(); it != mCircleMapToButtons.end(); it++)
                {   
                    (*it).second->update(mouse_pos);
                }
                break;
            }
            case EDrawingCurveSurface:
            {
                for (auto it = mCurveMapToButtons.begin(); it != mCurveMapToButtons.end(); it++)
                {   
                    (*it).second->update(mouse_pos);
                }
                break;
            }
            default:
                break;
        }
    }
}

// 鼠标移动回调函数（按下某个按键后移动）
void GLUTCallback::myMouseMotionFunc(int x, int y) 
{
    glm::vec2 mouse_pos = glm::vec2(float(x), float(y));
    mouse_pos.x -= mWinWidth * 0.5f;
    mouse_pos.y = mWinHeight * 0.5f - mouse_pos.y;
    mMousePos = Point(mouse_pos.x, mouse_pos.y);
    
    switch (mCurDrawingState)
    {
        case EDrawingCurveSurface:
        {
            mCurveGenerator->updateMousePos(mouse_pos);
            break;
        }
        default:
            break;
    }
    // std::cout << "Mouse motion at (" << mouse_pos.x << ", " << mouse_pos.y << ")" << std::endl;
}

void GLUTCallback::setWinWidth(int w)
{
    mWinWidth = w;
}

void GLUTCallback::setWinHeight(int h)
{
    mWinHeight = h;
}

void GLUTCallback::setSubWinWidth(int w)
{
    mSubWinWidth = w;
}

void GLUTCallback::setSubWinHeight(int h)
{
    mSubWinHeight = h;
}

void GLUTCallback::handleEntry(int state)
{
    if (state == GLUT_ENTERED) 
    {
        mIsMouseLeft = false;
        std::cout << "Mouse entered the window" << std::endl;
    } 
    else if (state == GLUT_LEFT) 
    {
        mIsMouseLeft = true;
        std::cout << "Mouse left the window" << std::endl;
    }
}