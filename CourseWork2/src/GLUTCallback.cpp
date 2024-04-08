#include "../include/GLUTCallback.hpp"

int GLUTCallback::mWinWidth = 1920;
int GLUTCallback::mWinHeight = 1080;

int GLUTCallback::mSubWinWidth = 1920;
int GLUTCallback::mSubWinHeight = 1080;

Curve::CurveType GLUTCallback::mCurCurveType = Curve::ECycloid;

unordered_map<Curve::CurveType, string> GLUTCallback::mCurveNames = {
    {Curve::EHeartCurve, "HeartCurve"},
    {Curve::ECycloid, "Cycloid"},
    {Curve::ESineCurve, "SineCurve"},
    {Curve::ERectMapToHeadtCurve, "MapToHeartCurve"},
    {Curve::ERectMapToCycloid, "MapToCycloid"},
    {Curve::ERectMapToSineCurve, "MapToSineCurve"}
};

unordered_map<Curve::CurveType, Curve::CurveType> GLUTCallback::mNameMapToCurves = {
    {Curve::ERectMapToHeadtCurve, Curve::EHeartCurve},
    {Curve::ERectMapToCycloid, Curve::ECycloid},
    {Curve::ERectMapToSineCurve, Curve::ESineCurve}
};

void GLUTCallback::drawText(string text, const glm::vec3& pos)
{
    glRasterPos3f(pos.x, pos.y, pos.z);

    for (auto ch : text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ch);
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

void GLUTCallback::setCurCurveType(Curve::CurveType type)
{
    mCurCurveType = type;
}

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

    Curve* instance = Curve::getInstance();    
    Curve::CurveObj* obj = nullptr;

    // 退出提示符绘制
    glRectf(1.0f - 0.34f, 1.0f - 0.05f, 1.0f - 0.05f, 1.0f - 0.20f);
    // 绘制字体
    glm::vec3 pos = glm::vec3{1.0f - 0.32f, 1.0f - 0.05f - 0.10f, 0.0f};
    drawText("Quit: Press ESC...", pos);

    // 设置绘制顶点
    glBegin(GL_LINE_STRIP);
    {
        switch (mCurCurveType)
        {
            case Curve::EHeartCurve:
            {
                obj = instance->getHeartCurveObj();
                instance->draw(obj);
                break;
            }    
            case Curve::ESineCurve:
            {
                obj = instance->getSineCurveObj();
                instance->draw(obj);
                break;
            }
            case Curve::ECycloid:
            {
                obj = instance->getCycloidObj();
                instance->draw(obj);
                break;
            }
            default:
            {
                break;
            }
        }
    }
    glEnd();
	
    if (obj)
    {   // 绘制字体
        glm::vec3 pos = glm::vec3{(-mWinWidth * 0.90f) / mWinWidth, (mWinHeight * 0.90f) / mWinHeight, 0.0f};
        drawText(obj->mXEquation, pos);

        pos.y -= 0.10f;
        drawText(obj->mYEquation, pos);

        for (int i = 0; i < (int)obj->mCoeffScales.size(); i++)
        {
            pos.y -= 0.10f;
            drawText(obj->mCoeffSign + to_string(i) + ": " + to_string(obj->mCoeffScales[i]), pos);
        }
    }
    // cout << "update..." << "\n";

    // 前后缓存区交换(双缓存机制)
    glutSwapBuffers();
}

void GLUTCallback::mySubWinDisplay(void)
{
    // 设置缓存区背景色
    glClearColor(0.05f, 0.15f, 0.18f, 1.0f);

    // 清空颜色缓存区
	glClear(GL_COLOR_BUFFER_BIT);

    // 提示
    // 绘制字体
    glColor4f(0.35f, 0.50f, 0.85f, 1.0f);
    glm::vec3 pos = glm::vec3{-1.0f + 0.05f, 1.0f - 0.10f, 0.0f};
    drawText("Click a box to activate curve...", pos);
    
    Curve* instance = Curve::getInstance();

    // 设置绘制颜色
    for (auto obj : instance->getCurveObjs())
    {
        // 设置填充绘制
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(5.0f);
        
        if (obj->mDisplay)
        {
            if (obj->mIsSelected)
            {
                mCurCurveType = mNameMapToCurves[obj->mType];
                glColor4f(0.25f, 0.30f, 0.65f, 1.0f);

                for (int i = 0; i < (int)obj->mBindIncCurveObjs.size(); i++)
                {
                    if (obj->mBindIncCurveObjs[i])
                    {
                        instance->draw(obj->mBindIncCurveObjs[i]);

                        // 绘制字体
                        glm::vec3 pos = glm::vec3{
                            obj->mBindIncCurveObjs[i]->mOriginalPos.x - obj->mBindIncCurveObjs[i]->mDimension.x * 0.42f, 
                            obj->mBindIncCurveObjs[i]->mOriginalPos.y + obj->mBindIncCurveObjs[i]->mDimension.y * 0.075f, 
                            0.0f
                        };
                        drawText(obj->mBindIncCurveObjs[i]->mCoeffSign, pos);
                    }
                    
                    if (obj->mBindDecCurveObjs[i])
                    {
                        instance->draw(obj->mBindDecCurveObjs[i]);

                        // 绘制字体
                        glm::vec3 pos = glm::vec3{
                            obj->mBindDecCurveObjs[i]->mOriginalPos.x - obj->mBindDecCurveObjs[i]->mDimension.x * 0.42f, 
                            obj->mBindDecCurveObjs[i]->mOriginalPos.y + obj->mBindDecCurveObjs[i]->mDimension.y * 0.075f, 
                            0.0f
                        };
                        drawText(obj->mBindDecCurveObjs[i]->mCoeffSign, pos);
                    }
                }
            }
            else
            {
                glColor4f(0.15f, 0.20f, 0.25f, 1.0f);
            }

            // 绘制图形
            instance->draw(obj);

            // 绘制字体
            glm::vec3 pos = glm::vec3{obj->mOriginalPos.x - obj->mDimension.x * 0.45f, obj->mOriginalPos.y, 0.0f};
            drawText(mCurveNames[obj->mType], pos);
        }
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

    // 正交投影
    glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLUTCallback::myKeyboardFunc(unsigned char key, int x, int y)
{
    switch (key)
    {
        case ESC:
        {
            exit(0);
            break;
        }
        default:
            break;
    }
}

void GLUTCallback::mySubKeyboardFunc(unsigned char key, int x, int y)
{
    switch (key)
    {
        case ESC:
        {
            exit(0);
            break;
        }
        default:
            break;
    }
}

void GLUTCallback::mySubMouseFunc(int button, int state, int x, int y)
{
    glm::vec2 mouse_pos = glm::vec2(float(x), float(y));
    mouse_pos.x -= mSubWinWidth * 0.5f;
    mouse_pos.y = mSubWinHeight * 0.5f - mouse_pos.y;

    mouse_pos.x /= (mSubWinWidth * 0.5f);
    mouse_pos.y /= (mSubWinHeight * 0.5f);
    // cout << mouse_pos.x << " " << mouse_pos.y << "\n";

    Curve::getInstance()->update(mouse_pos);
}