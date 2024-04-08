#include "../include/Curve.hpp"

Curve* Curve::mCurve = new Curve();

Curve::Curve()
{
    // 创建心形曲线对象
    mHeartCurveObj = new Curve::CurveObj(Curve::EHeartCurve);
    mHeartCurveObj->mCoeffScales.emplace_back(0.5f);
    mHeartCurveObj->mCoeffScales.emplace_back(1.0f);
    mHeartCurveObj->mXEquation = "x = A0 * (1.0f - A1 * cos(t)) * cos(t)";
    mHeartCurveObj->mYEquation = "y = A0 * (1.0f - A1 * cos(t)) * sin(t)";
    mHeartCurveObj->mCoeffSign = "A";

    // 正弦曲线
    mSineCurveObj = new Curve::CurveObj(Curve::ESineCurve);
    mSineCurveObj->mCoeffScales.emplace_back(0.75f);
    mSineCurveObj->mCoeffScales.emplace_back(16.0f);
    mSineCurveObj->mXEquation = "-PI < x < PI";
    mSineCurveObj->mYEquation = "y = C0 * sin(C1 * x)";
    mSineCurveObj->mCoeffSign = "C";

    // 摆线（开摆）
    mCycloidObj = new Curve::CurveObj(Curve::ECycloid);
    mCycloidObj->mCoeffScales.emplace_back(0.05f);
    mCycloidObj->mCoeffScales.emplace_back(7.0f);
    mCycloidObj->mXEquation = "x = B0 * (t - B1 * sin(t))";
    mCycloidObj->mYEquation = "y = B0 * (1.0 - B1 * cos(t))";
    mCycloidObj->mCoeffSign = "B";

    // 创建矩形对象
    Curve::CurveObj* mRectObj1 = new Curve::CurveObj(Curve::ERectMapToHeadtCurve);
    mRectObj1->mDimension.y *= 0.25f;
    mRectObj1->mOriginalPos.y = 1.0 - mRectObj1->mDimension.y * 0.5f - 0.30f;
    mRectObj1->mCoeffScales = vector<float>(mHeartCurveObj->mCoeffScales);
    createSubRectObj(mRectObj1, 1);
    createSubRectObj(mRectObj1, 2);

    for (int i = 0; i < (int)mRectObj1->mBindIncCurveObjs.size(); i++)
    {
        mRectObj1->mBindIncCurveObjs[i]->mCoeffSign = "A" + to_string(i);
        mRectObj1->mBindDecCurveObjs[i]->mCoeffSign = "A" + to_string(i);
        
        mRectObj1->mBindIncCurveObjs[i]->mBindIncCurveObjs.emplace_back(mHeartCurveObj);
        mRectObj1->mBindDecCurveObjs[i]->mBindDecCurveObjs.emplace_back(mHeartCurveObj);
    }
    mCurveObjs.emplace_back(mRectObj1);

    Curve::CurveObj* mRectObj2 = new Curve::CurveObj(Curve::ERectMapToCycloid);
    mRectObj2->mDimension = mRectObj1->mDimension;
    mRectObj2->mOriginalPos.y = mRectObj1->mOriginalPos.y - 0.65f;
    mRectObj2->mCoeffScales = vector<float>(mCycloidObj->mCoeffScales);
    createSubRectObj(mRectObj2, 1);
    createSubRectObj(mRectObj2, 2);

    for (int i = 0; i < (int)mRectObj1->mBindIncCurveObjs.size(); i++)
    {
        mRectObj2->mBindIncCurveObjs[i]->mCoeffSign = "B" + to_string(i);
        mRectObj2->mBindDecCurveObjs[i]->mCoeffSign = "B" + to_string(i);

        mRectObj2->mBindIncCurveObjs[i]->mBindIncCurveObjs.emplace_back(mCycloidObj);
        mRectObj2->mBindDecCurveObjs[i]->mBindDecCurveObjs.emplace_back(mCycloidObj);
    }
    mCurveObjs.emplace_back(mRectObj2);

    Curve::CurveObj* mRectObj3 = new Curve::CurveObj(Curve::ERectMapToSineCurve);
    mRectObj3->mDimension = mRectObj1->mDimension;
    mRectObj3->mOriginalPos.y = mRectObj2->mOriginalPos.y - 0.65f;
    mRectObj3->mCoeffScales = vector<float>(mSineCurveObj->mCoeffScales);
    createSubRectObj(mRectObj3, 1);
    createSubRectObj(mRectObj3, 2);
    for (int i = 0; i < (int)mRectObj1->mBindIncCurveObjs.size(); i++)
    {
        mRectObj3->mBindIncCurveObjs[i]->mCoeffSign = "C" + to_string(i);
        mRectObj3->mBindDecCurveObjs[i]->mCoeffSign = "C" + to_string(i);

        mRectObj3->mBindIncCurveObjs[i]->mBindIncCurveObjs.emplace_back(mSineCurveObj);
        mRectObj3->mBindDecCurveObjs[i]->mBindDecCurveObjs.emplace_back(mSineCurveObj);
    }
    mCurveObjs.emplace_back(mRectObj3);
}

Curve::~Curve()
{
    
}

Curve::CurveObj* Curve::getSineCurveObj()
{
    return this->mSineCurveObj;
}

Curve::CurveObj* Curve::getCycloidObj()
{
    return this->mCycloidObj;
}

void Curve::createSubRectObj(CurveObj* parent, int num)
{
    Curve::CurveObj* incObj = new Curve::CurveObj(Curve::EIncRect);
    float dim_x = parent->mDimension.x * 0.30f;
    float dim_y = dim_x > parent->mDimension.y ? parent->mDimension.y : dim_x;
    incObj->mDimension = glm::vec3{dim_x, dim_y, 0.0f};
    float x = parent->mOriginalPos.x - parent->mDimension.x * 0.5f - 0.10f - dim_x * 0.5f;
    float y = parent->mOriginalPos.y + parent->mDimension.y - (num - 1) * parent->mDimension.y - 0.10f;
    incObj->mOriginalPos = glm::vec3(x, y, 0.0f);
    incObj->mDisplay = false;

    parent->mBindIncCurveObjs.emplace_back(incObj);

    Curve::CurveObj* decObj = new Curve::CurveObj(Curve::EDecRect);
    decObj->mDimension = glm::vec3{dim_x, dim_y, 0.0f};

    x = parent->mOriginalPos.x + parent->mDimension.x * 0.5f + 0.10f + dim_x * 0.5f;
    decObj->mOriginalPos = glm::vec3(x, y, 0.0f);
    decObj->mDisplay = false;

    parent->mBindDecCurveObjs.emplace_back(decObj);
}

vector<Curve::CurveObj*>& Curve::getCurveObjs()
{
    return this->mCurveObjs;
}

Curve::CurveObj* Curve::getHeartCurveObj()
{
    return this->mHeartCurveObj;
}

// Curve::CurveObj* Curve::getRectObj()
// {
//     return this->mRectObj;
// }

bool Curve::containPoint(CurveObj* obj, const glm::vec2& point)
{
    if (!obj)
        return false;
    
    return (
        obj->mOriginalPos.x + obj->mDimension.x * 0.5f > point.x &&
        obj->mOriginalPos.x - obj->mDimension.x * 0.5f < point.x &&
        obj->mOriginalPos.y + obj->mDimension.y * 0.5f > point.y &&
        obj->mOriginalPos.y - obj->mDimension.y * 0.5f < point.y
    );
}

void Curve::update(const glm::vec2& point)
{
    // cout << "1" << "\n";
    for (auto obj : mCurveObjs)
    {
        if (obj->mDisplay)
        {
            bool sign = false;
            if (obj->mIsSelected)
            {
                for (int i = 0; i < (int)obj->mBindIncCurveObjs.size(); i++)
                {
                    CurveObj *incObj = obj->mBindIncCurveObjs[i], *decObj = obj->mBindDecCurveObjs[i];
                    if (incObj && containPoint(incObj, point))
                    {
                        cout << "Coeff INC..." << "\n";
                        obj->mCoeffScales[i] *= 1.05f;
                        
                        if (*(incObj->mBindIncCurveObjs.begin()))
                            (*(incObj->mBindIncCurveObjs.begin()))->mCoeffScales[i] = obj->mCoeffScales[i];

                        sign = true;
                    }

                    if (decObj && containPoint(decObj, point))
                    {
                        cout << "Coeff DEC..." << "\n";

                        obj->mCoeffScales[i] *= 0.975f;
                        
                        if (*(decObj->mBindDecCurveObjs.begin()))
                            (*(decObj->mBindDecCurveObjs.begin()))->mCoeffScales[i] = obj->mCoeffScales[i];

                        sign = true;
                    }

                    if (sign)
                        break;
                }
            }

            if (!sign)
            {
                if (obj && containPoint(obj, point))
                {
                    obj->mIsSelected = true;
                    // cout << obj->mIsSelected << "\n";
                }
                else
                {
                    obj->mIsSelected = false;
                }
            }
        }
    }
}

void Curve::draw()
{
    for (auto obj : mCurveObjs)
    {
        draw(obj);
    }
}

void Curve::draw(CurveType type, float x0, float y0)
{
    draw(nullptr, type);
}

void Curve::draw(CurveObj* obj, CurveType type)
{
    float x0 = 0.0f, y0 = 0.0f;
    float dim_x = 1.0f, dim_y = 1.0f;
    vector<float> scales = { 1.0f, 1.0f };
    if (obj)
    {
        x0 = obj->mOriginalPos.x, y0 = obj->mOriginalPos.y;
        dim_x = obj->mDimension.x, dim_y = obj->mDimension.y;
        type = obj->mType;
        scales = vector<float>(obj->mCoeffScales);
    }
    
    switch (type)
    {
        case Curve::EHeartCurve:
        {
            float x = 0.0f, y = 0.0f;
            float log_e = log(Eplison);
            for (float t = 0.0f; t < 2 * M_PI; t += (M_PI / 180.0f))
            {
                float diff = (1.0f - scales[1] * glm::cos(t));
                x = x0 + scales[0] * diff * glm::cos(t);
                y = y0 + scales[0] * diff * glm::sin(t);
                glVertex3f(x, y, 0.0f);
            }
            break;
        }
        case Curve::ECycloid:
        {
            float x = 0.0f, y = 0.0f;
            for (float t = -6.0f * M_PI; t < 6.0f * M_PI; t += (M_PI / 180.0f))
            {
                x = scales[0] * (t - scales[1] * glm::sin(t));
                y = scales[0] * (1.0 - scales[1] * glm::cos(t));
                glVertex3f(x, y, 0.0f);
            }
            break;
        }
        case Curve::ESineCurve:
        {
            for (float x = x0 - 10 * M_PI; x < x0 + 10 * M_PI; x += (M_PI / 180.0f))
            {
                float y = scales[0] * glm::sin(scales[1] * x);
                glVertex3f(x, y, 0.0f);
            }
            break;
        }
        case Curve::EIncRect:
        {
            glLineWidth(2.0f);

            glRectf(
                x0 - dim_x * 0.45f, 
                y0 + dim_y * 0.45f, 
                x0 + dim_x * 0.45f, 
                y0 - dim_y * 0.45f
            );

            glBegin(GL_LINE_STRIP);
            {
                glVertex3f(x0 - dim_x * 0.32f, y0, 0.0f);
                glVertex3f(x0 + dim_x * 0.32f, y0, 0.0f);
            }
            glEnd();

            glBegin(GL_LINE_STRIP);
            {
                glVertex3f(x0, y0 + dim_y * 0.32f, 0.0f);
                glVertex3f(x0, y0 - dim_y * 0.32f, 0.0f);
            }
            glEnd();

            break;
        }
        case Curve::EDecRect:
        {
            glLineWidth(2.0f);

            glRectf(
                x0 - dim_x * 0.45f, 
                y0 + dim_y * 0.45f, 
                x0 + dim_x * 0.45f, 
                y0 - dim_y * 0.45f
            );

            glBegin(GL_LINE_STRIP);
            {
                glVertex3f(x0 - dim_x * 0.32f, y0, 0.0f);
                glVertex3f(x0 + dim_x * 0.32f, y0, 0.0f);
            }
            glEnd();

            break;
        }
        case Curve::ERectMapToHeadtCurve:
        case Curve::ERectMapToCycloid:
        case Curve::ERectMapToSineCurve:
        {
            glRectf(
                x0 - dim_x * 0.5f, 
                y0 + dim_y * 0.5f, 
                x0 + dim_x * 0.5f, 
                y0 - dim_y * 0.5f
            );
            break;
        }
        default:
            break;
    }
}

void Curve::addCurveObj(CurveObj* obj)
{
    if (obj)
        mCurveObjs.emplace_back(obj);
}

void Curve::removeCurveObj(CurveObj* obj)
{
    for (auto it = mCurveObjs.begin(); it != mCurveObjs.end();)
    {
        if ((*it) == obj)
        {
            it = mCurveObjs.erase(it);
        }
        else
            it++;
    }
}