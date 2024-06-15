#include "../include/Button.hpp"

Button::Button(const glm::vec4& color, const int& lineWidth)
: mCenter(glm::zero<glm::vec2>()), mSize(glm::one<glm::vec2>()), mColor(color), mLineWidth(lineWidth) 
{
    mIsSelected = false;
}

Button::Button(const glm::vec2& center, const glm::vec2& size, 
    const glm::vec4& color, const int& lineWidth) : mCenter(center), mSize(size), mColor(color), mLineWidth(lineWidth)
{
    mIsSelected = false;
}

Button::~Button()
{

}

void Button::draw()
{
    // 设置绘制方式
    // 设置线框绘制
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 设置绘制颜色
    glColor4f(mColor.x, mColor.y, mColor.z, mColor.w);

    // 设置线宽
    glLineWidth(mLineWidth);

    // 绘制button的矩形框
    glm::vec2 lt = glm::vec2{mCenter.x - mSize.x * 0.5f, mCenter.y + mSize.y * 0.5f};
    glm::vec2 rb = glm::vec2{mCenter.x + mSize.x * 0.5f, mCenter.y - mSize.y * 0.5f};
    glRectf(lt.x, lt.y, rb.x, rb.y);
}

void Button::update(const glm::vec2& mouse_pos)
{
    if (containPoint(mouse_pos))
        mIsSelected = true;
    else
        mIsSelected = false;
}

bool Button::containPoint(const glm::vec2& point)
{
    return (
        mCenter.x + mSize.x * 0.5f > point.x &&
        mCenter.x - mSize.x * 0.5f < point.x &&
        mCenter.y + mSize.y * 0.5f > point.y &&
        mCenter.y - mSize.y * 0.5f < point.y
    );
}

const bool Button::isSelected() const
{
    return mIsSelected;
}

void Button::setIsSelected(const bool& isSelected)
{
    mIsSelected = isSelected;
}

void Button::setCenter(const glm::vec2& center)
{
    mCenter = center;
}

void Button::setSize(const glm::vec2& size)
{
    mSize = size;
}

void Button::setColor(const glm::vec4& color)
{
    mColor = color;
}

void Button::setLineWidth(const int& lineWidth)
{
    mLineWidth = lineWidth;
}

const glm::vec2 Button::getCenter() const
{
    return this->mCenter;
}

const glm::vec2 Button::getSize() const
{
    return this->mSize;
}

const glm::vec4 Button::getColor() const
{
    return mColor;
}

const int Button::getLineWidth() const
{
    return mLineWidth;
}

TextButton::TextButton(const int& fontSize)
: mFontSize(fontSize) { }

TextButton::TextButton(const string& text, const int& fontSize)
: mText(text), mFontSize(fontSize), mInputOver(false) { }

TextButton::~TextButton() { }

void TextButton::draw()
{
    Button::draw();

    // 绘制文本
    drawText(mText, mFontPos);
}

void TextButton::setText(const string& text)
{
    mText = text;
}

void TextButton::setFontSize(const int& size)
{
    mFontSize = size;
}

void TextButton::setFontPos(const glm::vec2& pos)
{
    mFontPos = pos;
}

void TextButton::processInput(unsigned char key)
{
    // cout << "[TextButton] Key input: " << (int)key << "\n";
    int len = mInputString.length();

    bool sign = false;
    switch (key)
    {   
        case 43:
        case 45:
        {   // 符号，判断是否合法
            if (len < 1)
                mInputString.push_back(key), sign = true;
            break;
        }
        case 8:
        case 127:
        {   // 输入退格键或删除键，删掉一个字符
            if (len > 0)
            {
                wchar_t wch = mInputString[len - 1];
                mInputString[len - 1] = 0;
                mInputString.resize(len - 1);
                sign = true;
            }
            break;
        }
        case 13:
        {   // 输入回车，输入结束
            mInputOver = true;
            break;
        }
        case 46:
        {
            // 输入小数点，要判断位置是否合法
            if (len > 0)
            {   // 插入
                mInputString.push_back(key);
                sign = true;
            }
            else
            {
                cout << "Invalid input key: " << key << "\n";
            }
            break;
        }
        default:
        {
            if (key >= '0' && key <= '9')
            {
                mInputString.push_back(key);
                sign = true;
            }
            else
            {
                cout << "Invalid input key: " << key << "\n";
            }
            break;
        }
    }

    if (sign)
    {
        cout << "Valid input key: " << key << "\n"; 
    }
}

void TextButton::clearInput()
{
    mInputString.clear();
    mInputOver = false;
}

string TextButton::getInputString()
{
    return this->mInputString;
}

bool TextButton::getInputState()
{
    return this->mInputOver;
}

