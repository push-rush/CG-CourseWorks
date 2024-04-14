#include "../include/GLUTCallback.hpp"

int GLUTCallback::mWinWidth = 1920;
int GLUTCallback::mWinHeight = 1080;

int GLUTCallback::mSubWinWidth = 1920;
int GLUTCallback::mSubWinHeight = 1080;

LineGeneration* GLUTCallback::mLineGeneration = new LineGeneration();

glm::vec2 GLUTCallback::mStartPoint = glm::vec2{0.0f, 0.0f};
glm::vec2 GLUTCallback::mEndPoint = glm::vec2{0.0f, 0.0f};

int GLUTCallback::mCounter = 0;
bool GLUTCallback::mIsSetting = false;

unordered_map<LineGeneration::LineAlgorithmType, TextButton*> GLUTCallback::mLineMapToButtons = {
    {LineGeneration::EDDA, new TextButton()},
    {LineGeneration::EMidpointLine, new TextButton()},
    {LineGeneration::ESlopeComparison, new TextButton()},
    {LineGeneration::EBresenham, new TextButton()},
    {(LineGeneration::LineAlgorithmType)(Button::EPointByPointComparison), new TextButton},
};

unordered_map<LineGeneration::LineAlgorithmType, glm::vec4> GLUTCallback::mLineAlgorToColors = {
    {LineGeneration::EDDA, glm::vec4{0.25f, 0.25f, 0.75f, 1.0f}},
    {LineGeneration::EMidpointLine, glm::vec4{0.65f, 0.25f, 0.25f, 1.0f}},
    {LineGeneration::ESlopeComparison, glm::vec4{0.25f, 0.65f, 0.25f, 1.0f}},
    {LineGeneration::EBresenham, glm::vec4{0.75f, 0.75f, 0.35f, 1.0f}},
    {(LineGeneration::LineAlgorithmType)(Button::EPointByPointComparison), glm::vec4{0.35f, 0.45f, 0.85f, 1.0f}},
};

GLUTCallback::DrawingState GLUTCallback::mCurDrawingState = GLUTCallback::EDrawingLine;

PolygonScanConversion* GLUTCallback::mPolyScanConverter = new PolygonScanConversion();

bool GLUTCallback::mIsScanOver = false;

vector<Point> GLUTCallback::mScanPoints{};

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

    // 退出提示符绘制
    glm::vec4 rect_pos{half_w - 200, half_h - 10, half_w - 10, half_h - 40};
    glRectf(rect_pos.x, rect_pos.y, rect_pos.z, rect_pos.w);

    // 绘制字体
    glm::vec2 pos = glm::vec2{half_w - 190, half_h - 30};
    drawText("Quit: Press ESC...", pos);

    // 绘制直线/多边形
    switch (mCurDrawingState)
    {   
        case EDrawingLine:
        {   // 绘制直线
            // 设置直线颜色
            glm::vec4 color = mLineAlgorToColors[mLineGeneration->getAlgorithmType()];
            glColor4f(color.x, color.y, color.z, color.w);
            // 绘制邻近直线的像素点
            glBegin(GL_POINTS);
                mLineGeneration->drawLine(mStartPoint, mEndPoint);
            glEnd();
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

    TextButton* switchModeButton = mLineMapToButtons[(LineGeneration::LineAlgorithmType)(Button::EPointByPointComparison)];
    if (switchModeButton)
    {   // 绘制模式切换button
        if (switchModeButton->isSelected())
        {   // 更新绘制模式
            mCurDrawingState = (DrawingState)((int)(mCurDrawingState + 1) % EDrawingStateNums);
            switchModeButton->setIsSelected(false);
        }

        glm::vec2 cent = glm::vec2{(rect_pos.x + rect_pos.z) * 0.5f, pos.y} - glm::vec2{0.0f, 60.0f};
        glm::vec2 sz = glm::vec2{rect_pos.z - rect_pos.x, rect_pos.y - rect_pos.w};
        switchModeButton->setCenter(cent);
        switchModeButton->setSize(sz);
        
        // 设置线宽和颜色
        switchModeButton->setLineWidth(4);
        switchModeButton->setColor(mLineAlgorToColors[LineGeneration::LineAlgorithmType(Button::EPointByPointComparison)]);

        // 设置文本位置
        glm::vec3 text_pos = glm::vec3{cent.x - sz.x * 0.5f + 10, cent.y - 6, 0.0f};
        switchModeButton->setFontPos(text_pos);

        // 设置文本内容
        string mode_text = mCurDrawingState < 1 ? "Line" : "Polygon";
        switchModeButton->setText("SwitchMode: " + mode_text);

        // 绘制
        switchModeButton->draw();
    }

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
    cout << "Main window key input: " << key << "\n";

    switch (key)
    {
        case ESC:
        {
            exit(0);
            break;
        }
        case GLUT_KEY_DOWN:
        case GLUT_ENTERED:
        {

            break;
        }
        default:
        {
            mIsScanOver = true;
            mPolyScanConverter->setPolygon(mScanPoints);
            mScanPoints.clear();
            break;
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

    switch (mCurDrawingState)
    {
        case EDrawingLine:
        {
            if (mCounter % 2 != 0)
            {
                if (!mIsSetting)
                    mStartPoint = glm::vec2(mouse_pos.x, mouse_pos.y), mIsSetting = true;
                else
                    mEndPoint = glm::vec2(mouse_pos.x, mouse_pos.y), mIsSetting = false;
            }
            mCounter++;
            break;
        }
        case EDrawingPolygon:
        {
            mScanPoints.emplace_back(Point{mouse_pos.x, mouse_pos.y});
            break;
        }
        default:
            break;
    }
}

void GLUTCallback::mySubMouseFunc(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        glm::vec2 mouse_pos = glm::vec2(float(x), float(y));
        mouse_pos.x -= mSubWinWidth * 0.5f;
        mouse_pos.y = mSubWinHeight * 0.5f - mouse_pos.y;

        for (auto it = mLineMapToButtons.begin(); it != mLineMapToButtons.end(); it++)
        {   // 更新鼠标输入后button状态
            (*it).second->update(mouse_pos);
        }
    }
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