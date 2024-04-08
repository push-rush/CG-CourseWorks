#pragma once

#include <iostream>
#include <vector>

using namespace std;

// glm
#include "../include/Vendor/glm/glm/glm.hpp"
#include "../include/Vendor/glm/glm/gtc/matrix_transform.hpp"

#include "./CusMath.h"

class Curve
{
public:
    enum CurveType
    {
        EHeartCurve,
        ECycloid,
        ESineCurve,
        ERectMapToHeadtCurve,
        ERectMapToCycloid,
        ERectMapToSineCurve,
        EIncRect,
        EDecRect
    };

    struct CurveObj
    {
        bool mDisplay;
        bool mIsSelected;
        CurveType mType;
        glm::vec3 mOriginalPos;
        glm::vec3 mDimension;
        glm::vec3 mScale;
        glm::vec3 mRotation;
        glm::vec3 mTranslation;
        string mXEquation;
        string mYEquation;
        string mCoeffSign;
        vector<CurveObj*> mBindIncCurveObjs;
        vector<CurveObj*> mBindDecCurveObjs;
        vector<float> mCoeffScales;

        CurveObj(CurveType type) : mType(type)
        {
            mIsSelected = false;
            mDisplay = true;

            mXEquation = "";
            mYEquation = "";
            mCoeffSign = "";

            mOriginalPos = glm::vec3{0.0f, 0.0f, 0.0f};
            mDimension = glm::vec3{1.0f, 1.0f, 0.0f};
            mScale = glm::vec3{1.0f, 1.0f, 1.0f};
            mRotation = glm::vec3{0.0f, 0.0f, 0.0f};
            mTranslation = glm::vec3{0.0f, 0.0f, 0.0f};
        }

        CurveObj(
            CurveType type,
            const glm::vec3& pos,
            const glm::vec3& dim,
            const glm::vec3& scale, 
            const glm::vec3& rot,
            const glm::vec3& trans
        ) : mDisplay(true), mXEquation(""), mYEquation(""), 
            mCoeffSign(""), mType(type), mIsSelected(false), mOriginalPos(pos), mDimension(dim), mScale(scale), mRotation(rot), mTranslation(trans) { }
    };

    bool containPoint(CurveObj* obj, const glm::vec2& point);
    
    void addCurveObj(CurveObj* obj);
    void removeCurveObj(CurveObj* obj);

    void draw();
    void draw(CurveObj* obj, CurveType type = ERectMapToHeadtCurve);
    void draw(CurveType type, float x = 0.0f, float y = 0.0f);

    void update(const glm::vec2& point);

public:
    static Curve* getInstance()
    {
        return mCurve;
    }

    Curve();
    ~Curve();

    CurveObj* getHeartCurveObj();
    CurveObj* getSineCurveObj();
    CurveObj* getCycloidObj();

    vector<CurveObj*>& getCurveObjs();

    void createSubRectObj(CurveObj* parent, int num);

private:
    CurveObj* mHeartCurveObj;
    CurveObj* mCycloidObj;
    CurveObj* mSineCurveObj;
    
    vector<CurveObj*> mCurveObjs;

    static Curve* mCurve;
};