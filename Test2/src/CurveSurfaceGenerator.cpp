#include "../include/CurveSurfaceGenerator.hpp"

unordered_map<CurveSurfaceGenerator::CurveAlogrithmType, string> CurveSurfaceGenerator::mCurveAlgorToName = {
    {CurveSurfaceGenerator::EHermiteCurve, "HermiteCurve"},
    {CurveSurfaceGenerator::EFergusonCurve, "FergusonCurve"},
    {CurveSurfaceGenerator::EBezierCurve, "BezierCurve"},
    {CurveSurfaceGenerator::EBSplineCurve, "BSplineCurve"},
    {CurveSurfaceGenerator::EBezierSurface, "BezierSurface"}
};

TextButton* CurveSurfaceGenerator::sParamAdjButton = new TextButton("T: ");

CurveSurfaceGenerator::CurveSurfaceGenerator(CurveAlogrithmType type)
: mType(type), mMousePos(0, 0), mIsDragging(false), mTValue(0.025)
{
    cout << "Size: " << mPoints.size() << "\n";
}

CurveSurfaceGenerator::~CurveSurfaceGenerator()
{

}

void CurveSurfaceGenerator::setAlgorithmType(CurveSurfaceGenerator::CurveAlogrithmType type)
{
    this->mType = type;
}

CurveSurfaceGenerator::CurveAlogrithmType CurveSurfaceGenerator::getAlgorithmType()
{
    return mType;
}

string CurveSurfaceGenerator::getCurveAlgorName(CurveSurfaceGenerator::CurveAlogrithmType type)
{
    return CurveSurfaceGenerator::mCurveAlgorToName[type];
}

void CurveSurfaceGenerator::draw()
{
    // 少于两个点直接退出
    int n = mPoints.size();
    if (n < 2)
        return;

    Point target = Point{0, 0};
    // 设置颜色
    glColor4f(0.25f, 0.30f, 0.75f, 1.0f);
    glPointSize(12);
    glBegin(GL_POINTS);
        for (auto &point : mPoints)
        {
            if (point.mIsSelected)
            {
                point.x = mMousePos.x;
                point.y = mMousePos.y;
                point.update();
                target = point;
            }
            
            glVertex2i(point.x, point.y);
        }
    glEnd();
    
    if (target.mIsSelected)
    {
        glLineWidth(4);
        // 设置颜色
        glColor4f(0.35f, 0.75f, 0.45f, 1.0f);
        glRectf(target.mBBox.mCenterX - target.mBBox.mWidth * 0.5f, target.mBBox.mCenterY + target.mBBox.mHeight * 0.5f,
            target.mBBox.mCenterX + target.mBBox.mWidth * 0.5f, target.mBBox.mCenterY - target.mBBox.mHeight * 0.5f
        );
    }

    // 贝塞尔曲线方程B(t)=T_n * M_n * P_n

    Matrixf P = Matrixf(n, Vectorf(2, 0));
    for (int i = 0; i < n; i++)
    {
        P[i][0] = mPoints[i].x;
        P[i][1] = mPoints[i].y;
    }

    if (mType == EBezierCurve)
    {
        // 1.计算贝塞尔基矩阵M_n
        Matrixi M_n = calcBezierMatrix(n - 1);

        // cout << M_n.size() << " " << M_n[0].size() << "\n";

        glPointSize(4);
        glColor4f(0.75f, 0.30f, 0.35f, 1.0f);
        glBegin(GL_POINTS);
        for (float t = 0; t <= 1; t += mTValue)
        {
            Matrixf T_n = Matrixf(1, Vectorf(n, 0));
            for (int i = 0; i < n; i++)
            {
                T_n[0][i] = pow(t, n - i - 1);
            }
            
            Matrixf results = MatByMat<Matrixf>(MatByMat<Matrixf, Matrixi>(T_n, M_n), P);
            // 绘制曲线上的点
            glVertex2i(results[0][0], results[0][1]);
        }
        glEnd();
    }

    // cout << n << "\n";

    if (mType == EBezierSurface)
    {
        if (!isPerfectSquare(n))
        {
            return;
        }
        n = sqrt(n);
        
        // 计算贝塞尔基矩阵M_n
        Matrixi M_n = calcBezierMatrix(n - 1);

        glPointSize(3);
        glColor4f(0.35f, 0.30f, 0.85f, 1.0f);
        // 贝塞尔曲面绘制
        glBegin(GL_POINTS);
        for (float u = 0; u <= 1; u += mTValue)
        {
            Matrixf T_u = Matrixf(1, Vectorf(n, 0));
            for (int i = 0; i < n; i++)
            {
                T_u[0][i] = pow(u, n - i - 1);
            }
            Matrixf uByM = MatByMat<Matrixf, Matrixi>(T_u, M_n);
            Matrixf transpose_uByM = transposeMatrix<Matrixf>(uByM);
            for (float w = 0; w <= 1; w += mTValue)
            {
                Matrixf T_w = Matrixf(1, Vectorf(n, 0));
                for (int j = 0; j < n; j++)
                {
                    T_w[0][j] = pow(w, n - j - 1);
                }
                Matrixf wByM = MatByMat<Matrixf, Matrixi>(T_w, M_n);
                Matrixf uByw = MatByMat<Matrixf>(transpose_uByM, wByM);

                Matrixf temp = Matrixf(1);
                for (auto vec : uByw)
                {
                    for (auto v : vec)
                    {
                        temp[0].emplace_back(v);
                    }
                }
                Matrixf results = MatByMat<Matrixf>(temp, P);

                // 绘制曲线上的点
                glVertex2i(results[0][0], results[0][1]);
            }
        }
        glEnd();
    }
}

void CurveSurfaceGenerator::setPoints(const vector<Point>& points)
{
    mPoints = points;
}

void CurveSurfaceGenerator::setColor(const glm::vec4& color)
{
    mColor = color;
}

// 生成贝塞尔基矩阵M_n
Matrixi CurveSurfaceGenerator::calcBezierMatrix(int n) 
{
    Matrixi M(n + 1, std::vector<int>(n + 1, 0));

    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; i + j <= n; j++)
        {   
            // 遇到已计算过的位置直接跳过
            if (M[i][j] != 0)
                continue;

            M[i][j] = binomialCoefficient(n, i) * binomialCoefficient(n - i, j);
            if ((n - i - j) % 2 != 0)
            {
                M[i][j] = -M[i][j];
            }

            // 对称矩阵，对称位置值相等
            if (i != j)
                M[j][i] = M[i][j];
        }
    }
    return M;
}

void CurveSurfaceGenerator::update(const glm::vec2& mouse_pos, bool up)
{
    for (auto& point : mPoints)
    {
        if (!up)
        {
            if (containPoint(point, mouse_pos))
            {
                point.mIsSelected = true; 
                mIsDragging = true;
            }
            else
                point.mIsSelected = false;
        }
        else
        {
            point.mIsSelected = false;
            mIsDragging = false;
        }
    }

    // CurveSurfaceGenerator::sParamAdjButton->update(mouse_pos);
}

bool CurveSurfaceGenerator::containPoint(Point pbox, const glm::vec2& point)
{
    return (
        pbox.mBBox.mCenterX + pbox.mBBox.mWidth * 0.5f > point.x &&
        pbox.mBBox.mCenterX - pbox.mBBox.mWidth * 0.5f < point.x &&
        pbox.mBBox.mCenterY + pbox.mBBox.mHeight * 0.5f > point.y &&
        pbox.mBBox.mCenterY - pbox.mBBox.mHeight * 0.5f < point.y
    );
}

void CurveSurfaceGenerator::updateMousePos(const glm::vec2& pos)
{
    mMousePos = pos;
}

bool CurveSurfaceGenerator::isLocking()
{
    return mIsDragging;
}

TextButton* CurveSurfaceGenerator::getParamAdjButton()
{
    return CurveSurfaceGenerator::sParamAdjButton;
}

void CurveSurfaceGenerator::updateAdjParam()
{
    if (sParamAdjButton->getInputState())
    {
        float t = stof(sParamAdjButton->getInputString());
        if (t <= 0.2 && t >= 0.000001)
        {
            mTValue = t;
        }
        sParamAdjButton->clearInput();
    }

    // cout << "TValue: " << mTValue << "\n";
}

float CurveSurfaceGenerator::getTParam()
{
    return this->mTValue;
}