#pragma once

#include <iostream>
#include <vector>
using namespace std;

// glm
#include "../include/Vendor/glm/glm/glm.hpp"
#include "../include/Vendor/glm/glm/gtc/matrix_transform.hpp"

#include <GL/gl.h>
#include <GL/glu.h>

#include "../../GLUT/glut-3.7.6-bin/glut.h"

static void drawText(string text, const glm::vec2& pos, int fontSize = 18)
{
    glRasterPos2f(pos.x, pos.y);

    switch (fontSize)
    {
        case 10:
        {
            for (auto ch : text)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ch);
            break;
        }
        case 12:
        {
            for (auto ch : text)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, ch);
            break;
        }
        case 13:
        {
            for (auto ch : text)
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ch);
            break;
        }
        case 15:
        {
            for (auto ch : text)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ch);
            break;
        }
        case 24:
        {
            for (auto ch : text)
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ch);
            break;
        }
        case 18:
        default:
        {
            for (auto ch : text)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ch);
            break;
        }
    }
}

class Button
{
public:
    enum ButtonRelativeType
    {
        ELineMidpoint,
        ELineSlopeComparison,
        ELineDDA,
        ELineBresenham,
        EPointByPointComparison,
        ETypeNums
    };

private:
    glm::vec2 mCenter;
    glm::vec2 mSize;
    glm::vec4 mColor;
    int mLineWidth;
    bool mIsSelected;

public:
    Button(const glm::vec4& color = glm::vec4{0.15f, 0.15f, 0.35f, 1.0f}, const int& lineWidth = 1);
    Button(const glm::vec2& center, const glm::vec2& size, 
        const glm::vec4& color = glm::vec4{0.15f, 0.15f, 0.35f, 1.0f}, const int& lineWidth = 1);
    ~Button();

    virtual void draw();
    virtual void update(const glm::vec2& mouse_pos);
    
    const bool isSelected() const;

    // setters
    void setCenter(const glm::vec2& center);
    void setSize(const glm::vec2& size);
    void setColor(const glm::vec4& color);
    void setLineWidth(const int& lineWidth);
    void setIsSelected(const bool& isSelected);

    // getters
    const glm::vec2 getCenter() const;
    const glm::vec2 getSize() const;
    const glm::vec4 getColor() const;
    const int getLineWidth() const;

private:
    bool containPoint(const glm::vec2& mouse_pos);
};

class TextButton : public Button
{
    string mText;
    int mFontSize;
    glm::vec2 mFontPos;

public:
    TextButton(const int& fontSize = 18);
    TextButton(const string& text, const int& fontSize = 18);
    ~TextButton();

    void draw() override;

    void setText(const string& text);
    void setFontSize(const int& size);
    void setFontPos(const glm::vec2& pos);
};