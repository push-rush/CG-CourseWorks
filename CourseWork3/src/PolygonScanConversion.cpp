#include "../include/PolygonScanConversion.hpp"

PolygonScanConversion::PolygonScanConversion()
{

}

PolygonScanConversion::PolygonScanConversion(const vector<Point>& points) 
: mPoints(points)
{

}

PolygonScanConversion::PolygonScanConversion(const vector<Point>& points, const glm::vec4& color) 
: mPoints(points), mColor(color)
{

}

PolygonScanConversion::~PolygonScanConversion()
{

}

void PolygonScanConversion::draw()
{   // 逐点比较法绘制多边形
    // cout << "point size: " << (int)mPoints.size() << "\n";
    if ((int)mPoints.size() < 3)
        return;

    // 存储线段信息
    vector<Segment> segments;

    // 找出上下左右边界值
    int sz = mPoints.size();
    int min_x = 10000, max_x = -10000; 
    int min_y = 10000, max_y = -10000;
    for (int i = 0; i < sz; i++)
    {
        if (mPoints[i].x < min_x)
            min_x = mPoints[i].x;
        if (mPoints[i].x > max_x)
            max_x = mPoints[i].x;

        if (mPoints[i].y < min_y)
            min_y = mPoints[i].y;
        if (mPoints[i].y > max_y)
            max_y = mPoints[i].y;
        
        segments.emplace_back(Segment{mPoints[i], mPoints[(i+1) % sz]});
    }

    Point rayDir = {1.0f, 0.0f};
    Point intersectPoint = {0.0f, 0.0f};
    for (float i = min_x; i <= max_x; i++)
    {
        for (float j = min_y; j <= max_y; j++)
        {
            Point p = {i, j};
            int num = 0;
            for (int k = 0; k < (int)segments.size(); k++)
            {
                if (raySegmentIntersection(p, rayDir, segments[k], intersectPoint))
                    num++;
            }
            if (num % 2 != 0)
                glVertex2i(p.x, p.y);
        }
    }
}

bool PolygonScanConversion::floodFill(Point p)
{
    return false;
}

void PolygonScanConversion::setPolygon(const vector<Point>& points)
{
    mPoints = points;
}

void PolygonScanConversion::setColor(const glm::vec4& color)
{
    mColor = color;
}

const glm::vec4 PolygonScanConversion::getPolyColor() const
{
    return mColor;
}
