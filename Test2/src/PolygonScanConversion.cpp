#include "../include/PolygonScanConversion.hpp"

unordered_map<PolygonScanConversion::PloygonAlgorithmType, string> PolygonScanConversion::mPolygonAlgorToName = {
    {PolygonScanConversion::EPointByPoint, "PointByPoint"},
    {PolygonScanConversion::EScanLineConversion, "ScanLineConversion"},
    {PolygonScanConversion::EEdgePadding, "EdgePadding"},
    {PolygonScanConversion::EEdgeMark, "EdgeMark"},
    {PolygonScanConversion::EFloodFill, "FloodFill"}
};

PolygonScanConversion::PolygonScanConversion(PloygonAlgorithmType type)
: mType(type)
{

}

PolygonScanConversion::PolygonScanConversion(const vector<Point>& points, PloygonAlgorithmType type) 
: mPoints(points), mType(type)
{

}

PolygonScanConversion::PolygonScanConversion(const vector<Point>& points, const glm::vec4& color, PloygonAlgorithmType type) 
: mPoints(points), mColor(color), mType(type)
{

}

PolygonScanConversion::~PolygonScanConversion()
{

}

void PolygonScanConversion::draw()
{   // 逐点比较法绘制多边形
    // cout << "point size: " << (int)mPoints.size() << "\n";
    int n = mPoints.size();
    if (n < 3)
        return;

    switch (mType)
    {
        case EPointByPoint:
        {   // 逐点比较法

            glBegin(GL_POINTS);

            // 存储线段信息
            vector<Segment> segments;

            // 找出上下左右边界值
            int min_x = 10000, max_x = -10000; 
            int min_y = 10000, max_y = -10000;
            for (int i = 0; i < n; i++)
            {
                if (mPoints[i].x < min_x)
                    min_x = mPoints[i].x;
                if (mPoints[i].x > max_x)
                    max_x = mPoints[i].x;

                if (mPoints[i].y < min_y)
                    min_y = mPoints[i].y;
                if (mPoints[i].y > max_y)
                    max_y = mPoints[i].y;
                
                segments.emplace_back(Segment{mPoints[i], mPoints[(i+1) % n]});
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
                        glVertex2i((int)p.x, (int)p.y);
                }
            }

            glEnd();
            break;
        }
        case EScanLineConversion:
        {   
            // 扫描线转换算法

            glBegin(GL_POINTS);

            // 找出多边形的最高点和最低点的纵坐标
            int yMin = mPoints[0].y, yMax = mPoints[0].y;
            for (int i = 1; i < n; i++) 
            {
                yMin = std::min(yMin, int(mPoints[i].y));
                yMax = std::max(yMax, int(mPoints[i].y));
            }

            // 创建边表（ET）
            std::unordered_map<int, Edge*> edgeTable;

            // 创建多边形的边信息
            for (int i = 0; i < n; i++)
            {
                Point p1 = mPoints[i];
                Point p2 = mPoints[(i + 1) % n];
                
                // 忽略水平边
                if (p1.y == p2.y) 
                    continue;  

                if (p1.y > p2.y) 
                    std::swap(p1, p2);

                Edge *edge = new Edge();
                edge->yMax = p2.y;
                edge->x = p1.x;
                edge->dx = float(p2.x - p1.x) / (p2.y - p1.y);

                if (!edgeTable.count(p1.y))
                    edgeTable.emplace(p1.y, edge);
                else
                {
                    Edge *slow = edgeTable[p1.y];
                    auto fast = slow;
                    while (fast)
                    {
                        if (fast->next)
                        {
                            fast = fast->next;
                            if (edge->x < slow->x)
                            {   // 插入链首
                                edge->next = slow;
                                edgeTable[p1.y] = edge;
                                break;
                            }
                            else if (edge->x >= slow->x && edge->x < fast->x)
                            {   // 插入中间
                                edge->next = fast;
                                slow->next = edge;
                                break;
                            }
                        }
                        else
                        {   // 插入链尾
                            slow->next = edge;
                            break;
                        }
                        slow = slow->next;
                    }
                }
            }

            // 活性表（AET）
            std::vector<Edge*> activeEdgeTable;
            // 扫描线扫描多边形
            for (int y = yMin; y <= yMax; y++) 
            {
                if (edgeTable.count(y))
                {
                    Edge* edge = edgeTable[y];
                    while (edge) 
                    {
                        activeEdgeTable.emplace_back(edge);
                        edge = edge->next;
                    }
                }

                if (activeEdgeTable.size() > 0)
                {
                    activeEdgeTable.erase(
                        std::remove_if(activeEdgeTable.begin(), activeEdgeTable.end(),
                                    [y](Edge* e) { return e->yMax == y; }
                        ),
                        activeEdgeTable.end()
                    );

                    sort(activeEdgeTable.begin(), activeEdgeTable.end(), [](Edge* e1, Edge* e2){
                        return e1->x < e2->x;
                    });

                    // 填充像素
                    for (size_t i = 0; i < activeEdgeTable.size(); i += 2) 
                    {
                        int xStart = (int)std::ceil(activeEdgeTable[i]->x);
                        int xEnd = (int)std::floor(activeEdgeTable[i + 1]->x);

                        for (int x = xStart; x <= xEnd; x++) 
                        {
                            glVertex2i(x, y);
                        }
                        
                        // 更新活性边表中边的x坐标
                        activeEdgeTable[i]->x += activeEdgeTable[i]->dx;
                        activeEdgeTable[i + 1]->x += activeEdgeTable[i + 1]->dx;
                    }
                }
            }

            // 释放手动开辟的所有空间
            for (auto iter : edgeTable)
            {
                auto node = iter.second;
                while (node)
                {
                    auto next = node->next;
                    delete node;
                    node = next;
                }
            }

            glEnd();

            break;
        }
        case EFloodFill:
        {
            // 计算重心
            Point center_of_gravity = calculatePolygonCentroid(mPoints);

            // 找出多边形的最高点和最低点的纵坐标
            int xMin = mPoints[0].x, xMax = mPoints[0].x, yMin = mPoints[0].y, yMax = mPoints[0].y;
            for (int i = 1; i < n; i++) 
            {
                xMin = std::min(xMin, int(mPoints[i].x));
                xMax = std::max(xMax, int(mPoints[i].x));
                yMin = std::min(yMin, int(mPoints[i].y));
                yMax = std::max(yMax, int(mPoints[i].y));
            }
            
            // 存储线段信息
            vector<Segment> segments;

            // 找出上下左右边界值
            int min_x = 10000, max_x = -10000; 
            int min_y = 10000, max_y = -10000;
            for (int i = 0; i < n; i++)
            {
                if (mPoints[i].x < min_x)
                    min_x = mPoints[i].x;
                if (mPoints[i].x > max_x)
                    max_x = mPoints[i].x;

                if (mPoints[i].y < min_y)
                    min_y = mPoints[i].y;
                if (mPoints[i].y > max_y)
                    max_y = mPoints[i].y;
                
                segments.emplace_back(Segment{mPoints[i], mPoints[(i+1) % n]});
            }

            // 漫水法
            glBegin(GL_POINTS);
            floodFill(Point{float(int(center_of_gravity.x)), float((int)center_of_gravity.y)}, 
                Color{float(int(0.10f * 255.)), float(int(0.25f * 255.)), float(int(0.28f * 255.))}, 
                Color{0.25f, 0.30f, 0.75f},
                xMin,
                xMax,
                yMin,
                yMax,
                segments
            );
            glEnd();
            break;
        }
        default:
            break;
    }
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

PolygonScanConversion::PloygonAlgorithmType PolygonScanConversion::getAlgorithmType()
{
    return mType;
}

void PolygonScanConversion::setAlgorithmType(PloygonAlgorithmType type)
{
    mType = type;
}

vector<Point> PolygonScanConversion::getInputPoints() const
{
    return mPoints;
}

// 获取像素颜色
PolygonScanConversion::Color PolygonScanConversion::getPixel(int x, int y) 
{
    if (x < 0 || x >= 1200 || y < 0 || y >= 768) 
        return {1.0f, 1.0f, 1.0f}; // 默认返回白色
    
    GLubyte pixels[4] = { 0 };
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    auto color = Color{float(pixels[0]), float(pixels[1]), float(pixels[2])};

    return color;
}

// 种子填充算法
void PolygonScanConversion::floodFill(Point p, Color targetColor, Color fillColor, int xmin, int xmax, int ymin, int ymax, const std::vector<Segment>& segments) 
{
    std::stack<Point> points;
    points.push(p);
    bool pixelBuffer[768][1200] = { { false } };

    while (!points.empty()) 
    {
        Point p = points.top();
        points.pop();

        if ((int)p.x < xmin || (int)p.x > xmax || (int)p.y < ymin || (int)p.y > ymax || pixelBuffer[(int)p.y + 384][(int)p.x + 600])
            continue;

        // 判断点是否在多边形内
        Point rayDir = {1.0f, 0.0f};
        Point intersectPoint = {0.0f, 0.0f};
        int num = 0;
        for (const auto& segment : segments) 
        {
            if (raySegmentIntersection(p, rayDir, segment, intersectPoint))
                num++;
        }

        // 点在多边形内
        if (num % 2 != 0) 
        {
            glVertex2i((int)p.x, (int)p.y);
            pixelBuffer[(int)p.y + 384][(int)p.x + 600] = true;

            // 检查和添加相邻的点
            if ((int)p.x + 1 <= xmax && !pixelBuffer[(int)p.y + 384][(int)p.x + 1 + 600])
                points.push({p.x + 1, p.y});
            if ((int)p.x - 1 >= xmin && !pixelBuffer[(int)p.y + 384][(int)p.x - 1 + 600])
                points.push({p.x - 1, p.y});
            if ((int)p.y + 1 <= ymax && !pixelBuffer[(int)p.y + 1 + 384][(int)p.x + 600])
                points.push({p.x, p.y + 1});
            if ((int)p.y - 1 >= ymin && !pixelBuffer[(int)p.y - 1 + 384][(int)p.x + 600])
                points.push({p.x, p.y - 1});
        }
    }
}

string PolygonScanConversion::getPolygonAlgorName(PloygonAlgorithmType type)
{
    if (mPolygonAlgorToName.count(type))
        return mPolygonAlgorToName[type];
    return {};
}