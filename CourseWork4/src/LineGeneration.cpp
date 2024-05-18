#include "../include/LineGeneration.hpp"

unordered_map<LineGeneration::LineAlgorithmType, string> LineGeneration::mLineAlgorToName = {
    {LineGeneration::EDDA, "DDA"},
    {LineGeneration::EMidpointLine, "MidpointLine"},
    {LineGeneration::ESlopeComparison, "SlopeComparison"},
    {LineGeneration::EBresenham, "Bresenham"}
};

LineGeneration::LineGeneration(LineAlgorithmType type)
: mType(type)
{

}

LineGeneration::~LineGeneration()
{

}

void LineGeneration::setAlgorithmType(LineAlgorithmType type)
{
    mType = type;
}

LineGeneration::LineAlgorithmType LineGeneration::getAlgorithmType()
{
    return mType;
}

bool LineGeneration::drawLine(const glm::vec2& sp, const glm::vec2& ep)
{
    glm::vec2 st, end;
    
    // 处理特殊情况（水平线/垂直线/起点、终点重合）
    if (sp.x != ep.x)
    {
        if (sp.y > ep.y)
            st = ep, end = sp;
        else if (sp.y < ep.y)
            st = sp, end = ep;
        else
        {   // 水平线
            if (sp.x < ep.x)
                st = sp, end = ep;
            else if (sp.x > ep.x)
                st = ep, end = sp;
            else // 起点与终点重合
                return false;
            
            while (st.x <= end.x)
            {
                glVertex2i(st.x, st.y);
                st.x++;
            }
            return true;
        }
    }
    else
    {
        if (sp.y != ep.y)
        {   // 竖直线
            if (sp.y < ep.y)
                st = sp, end = ep;
            else if (sp.y > ep.y)
                st = ep, end = sp;
            else // 起点与终点重合
                return false;
            
            while (st.y <= end.y)
            {
                glVertex2i(st.x, st.y);
                st.y++;
            }
            return true;
        }
        else // 起点与终点重合
            return false;
    }

    switch (mType)
    {
        case EMidpointLine:
        {   // 中点画线法
            float k = (end.y - st.y) / (end.x - st.x);
            
            // f(x) = ax + b + c
            int a = st.y - end.y; // a = y0 - y1
            int b = end.x - st.x; // b = x1 - x0
            int c = st.x * end.y - end.x * st.y; // c = x0 * y1 - x1 * y0
            
            // 放大2倍，避免小数运算
            int da = 2 * a, db = 2 * b, dab = da + db;
            if (k < 0)
            {
                da = -da, dab = da + db;
            }
            
            int d = 2 * a + b;
            if (k < 0)
            {
                d = b - 2 * a;
                if (abs(k) > 1)
                    d = 2 * b - a;
            }
            else if (k > 1)
                d = 2 * b + a;

            // 绘制起点
            glVertex2i(st.x, st.y);            
            if (end.x > st.x)
            {
                if (abs(k) <= 1)
                {
                    while (st.x <= end.x)
                    {
                        if (d < 0)
                        {
                            st.x++;
                            st.y++;
                            glVertex2i(st.x, st.y);
                            d += dab;   
                        }
                        else
                        {
                            st.x++;
                            glVertex2i(st.x, st.y);
                            d += da;
                        }
                    }
                }
                else
                {
                    while (st.x <= end.x)
                    {
                        if (d >= 0)
                        {
                            st.x++;
                            st.y++;
                            glVertex2i(st.x, st.y);
                            d += dab;   
                        }
                        else
                        {
                            st.y++;
                            glVertex2i(st.x, st.y);
                            d += db;
                        }
                    }
                }
            }
            else
            {
                if (abs(k) <= 1)
                {
                    while (st.x >= end.x)
                    {
                        if (d >= 0)
                        {
                            st.x--;
                            st.y++;
                            glVertex2i(st.x, st.y);
                            d += dab;   
                        }
                        else
                        {
                            st.x--;
                            glVertex2i(st.x, st.y);
                            d += da;
                        }
                    }
                }
                else
                {
                    while (st.x >= end.x)
                    {
                        if (d < 0)
                        {
                            st.x--;
                            st.y++;
                            glVertex2i(st.x, st.y);
                            d += dab;   
                        }
                        else
                        {
                            st.y++;
                            glVertex2i(st.x, st.y);
                            d += db;
                        }
                    }
                }
            }
            break;
        }
        case ESlopeComparison:
        {   // 斜率比较法
            float k = (end.y - st.y) / (end.x - st.x);
            int dx = k > 0 ? 1 : -1;

            int a = st.y - end.y; // a = y0 - y1
            int b = end.x - st.x; // b = x1 - x0
            int c = st.x * end.y - end.x * st.y; // c = x0 * y1 - x1 * y0

            glVertex2i(st.x, st.y);
            if (abs(k) <= 1)
                st.x += dx;
            else
                st.y++;

            if (st.x < end.x)
            {
                while (st.x <= end.x && st.y <= end.y)
                {
                    glVertex2i(st.x, st.y);

                    if (a * st.x + b * st.y + c >= 0)
                        st.x++;
                    else
                        st.y++;
                }
            }
            else
            {
                while (st.x >= end.x && st.y <= end.y)
                {
                    glVertex2i(st.x, st.y);
                    
                    if (a * st.x + b * st.y + c < 0)
                        st.x--;
                    else
                        st.y++;
                }
            }
            break;
        }
        case EDDA:
        {   // 数字微分分析法
            float k = (end.y - st.y) / (end.x - st.x);
            if (abs(k) <= 1)
            {
                if (end.x > st.x)
                {
                    while (st.x <= end.x)
                    {
                        glVertex2i(st.x, int(st.y + 0.5f));
                        st.x++;
                        st.y += k;
                    }
                }
                else
                {
                    while (st.x >= end.x)
                    {
                        glVertex2i(st.x, int(st.y + 0.5f));
                        st.x--;
                        st.y -= k;
                    }
                }
            }
            else
            {
                float rk = 1 / k;
                while (st.y <= end.y)
                {
                    glVertex2i(int(st.x + 0.5f), st.y);
                    st.x += rk;                    
                    st.y++;
                }
            }
            break;
        }
        case EBresenham:
        {   
            float k = (end.y - st.y) / (end.x - st.x);
            float ik = 1 / k;
            
            float d = 0.0f;
            float e = d - 0.5f;
            bool sign = false;

            if (k > 0)
            {   // k > 0
                if (k <= 1)
                {
                    while (st.y <= end.y)
                    {
                        glVertex2i(st.x, st.y);

                        d += k;
                        st.x++;

                        if (d >= 0.5 && !sign)
                            st.y++, sign = true;
                        if (d > 1)
                            d -= 1, sign = false;
                    }
                }
                else
                {
                    while (st.x <= end.x)
                    {
                        glVertex2i(st.x, st.y);
                        
                        d += ik;
                        st.y++;
                        if (d >= 0.5 && !sign)
                            st.x++, sign = true;
                        if (d > 1)
                            d -= 1, sign = false;
                    }
                }
            }
            else
            {   // k < 0
                e = d + 0.5f;
                if (abs(k) <= 1)
                {
                    while (st.y <= end.y)
                    {
                        glVertex2i(st.x, st.y);

                        d += k;
                        st.x--;
                        if (d <= -0.5 && !sign)
                            st.y++, sign = true;
                        if (d < -1)
                            d += 1, sign = false;
                    }
                }
                else
                {
                    while (st.x >= end.x)
                    {
                        glVertex2i(st.x, st.y);

                        d += ik;
                        st.y++;
                        if (d <= -0.5 && !sign)
                            st.x--, sign = true;
                        if (d < -1)
                            d += 1, sign = false;
                    }
                }
            }

            // if (k > 0)
            // {   // k > 0
            //     // cout << "pos k: " << k << "\n";
            //     if (k <= 1)
            //     {
            //         while (st.y <= end.y)
            //         {
            //             glVertex2i(st.x, st.y);
            //             // d += k;
            //             e += k;
            //             st.x++;
            //             if (e > 0)
            //                 st.y++, e -= 1;

            //             // if (d >= 0.5 && !sign)
            //             //     st.y++, sign = true;
            //         }
            //     }
            //     else
            //     {
            //         while (st.x <= end.x)
            //         {
            //             glVertex2i(st.x, st.y);
            //             e += ik;
            //             st.y++;
            //             if (e > 0)
            //                 st.x++, e -= 1;
            //         }
            //     }
            // }
            // else
            // {   // k < 0
            //     // cout << "neg k: " << k << "\n";
            //     e = d + 0.5f;
            //     if (abs(k) <= 1)
            //     {
            //         while (st.y <= end.y)
            //         {
            //             glVertex2i(st.x, st.y);
            //             // d += k;
            //             e += k;
            //             st.x--;
            //             if (e < 0)
            //                 st.y++, e += 1;

            //             // if (d < -1)
            //             //     d += 1, sign = false;
            //             // if (d <= -0.5 && !sign)
            //             //     st.y++, sign = true;
            //         }
            //     }
            //     else
            //     {
            //         while (st.x >= end.x)
            //         {
            //             glVertex2i(st.x, st.y);
            //             // d += ik;
            //             e += ik;
            //             st.y++;
            //             if (e < 0)
            //                 st.x--, e+= 1;

            //             // if (d < -1)
            //             //     d += 1, sign = false;
            //             // if (d <= -0.5 && !sign)
            //             //     st.x--, sign = true;
            //         }
            //     }
            // }
            break;
        }
        default:
            break;
    }
}

string LineGeneration::getLineAlgorName(LineAlgorithmType type)
{
    if (mLineAlgorToName.count(type))
        return mLineAlgorToName[type];
    
    return {};
}