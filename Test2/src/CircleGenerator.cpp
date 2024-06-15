#include "../include/CircleGenerator.hpp"

unordered_map<CircleGenerator::CircleAlgorithmType, string> CircleGenerator::mCircleAlgorToName = {
    {CircleGenerator::EDDA, "DDA"},
    {CircleGenerator::EMidpointCircle, "MidpointLine"},
    {CircleGenerator::ESlopeComparison, "SlopeComparison"},
    {CircleGenerator::EBresenham, "Bresenham"}
};

CircleGenerator::CircleGenerator(CircleAlgorithmType type)
: mType(type)
{

}

CircleGenerator::~CircleGenerator()
{

}

void CircleGenerator::setAlgorithmType(CircleAlgorithmType type)
{
    mType = type;
}

CircleGenerator::CircleAlgorithmType CircleGenerator::getAlgorithmType()
{
    return mType;
}

void CircleGenerator::drawSymmetryPoint(const glm::vec2& point)
{
    glVertex2i(point.x, point.y);
    glVertex2i(point.y, point.x);
    glVertex2i(-point.x, point.y);
    glVertex2i(point.y, -point.x);
    glVertex2i(point.x, -point.y);
    glVertex2i(-point.y, point.x);
    glVertex2i(-point.x, -point.y);
    glVertex2i(-point.y, -point.x);
}

bool CircleGenerator::drawCircle(const glm::vec2& center, const float& radius)
{
    float x = 0, y = radius;

    switch (mType)
    {
        case EMidpointCircle:
        {
            float d = 1.25 - radius;

            // 绘制对称点
            drawSymmetryPoint(glm::vec2{x + center.x, y + center.y});
            while (x <= y)
            {
                if (d < 0)
                    d += (2 * x + 3);
                else
                {
                    d += (2 * (x - y) + 5);
                    y--;
                }
                x++;
                drawSymmetryPoint(glm::vec2{x + center.x, y + center.y});
            }
            break;
        }
        case EBresenham:
        {
            int d1 = 0, d2 = 0, dir = 0;
            int delta = 2 * (1- radius);

            while (x <= y)
            {
                drawSymmetryPoint(glm::vec2{x + center.x, y + center.y});

                if (delta < 0)
                {
                    d1 = 2 * (delta + y) - 1;
                    if (d1 <= 0)
                        dir = 1;
                    else
                        dir = 2;  
                }
                else if (delta > 0)
                {
                    d2 = 2 * (delta - x) - 1;
                    if (d2 <= 0)
                        dir = 2;
                    else
                        dir = 3;
                }
                else
                    dir = 2;
                
                switch (dir)
                {
                    case 1:
                    {
                        x++;
                        delta += (2 * x + 1);
                        break;
                    }
                    case 2:
                    {
                        x++;
                        y--;
                        delta += (2 * (x - y + 1) + 1);
                        break;
                    }
                    case 3:
                    {
                        y--;
                        delta += (-2 * y + 1);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
            break;
        }
        default:
            break;
    }
}

string CircleGenerator::getCircleAlgorName(CircleAlgorithmType type)
{
    if (mCircleAlgorToName.count(type))
        return mCircleAlgorToName[type];
    
    return {};
}