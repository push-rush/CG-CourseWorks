# 实验2
## 要求：三阶贝齐尔曲线或曲面的绘制。绘制三阶贝齐尔曲线或曲面 （建议曲线曲面都实现绘制，并使用交互方式输入控制顶点）。
## 贝塞尔曲线
### 贝塞尔曲线的矩阵一般表示形式
$$ 
    B(n) = T_{n} \cdot M_{n} \cdot P
$$
#### 参数向量$T_{n}$
$$
    T_{n} = \begin{bmatrix}
        t^{n} \
        t^{n-1} \
        ··· \
        t^{2} \
        t^{1} \
        1 \
    \end{bmatrix}
$$
#### 贝塞尔基矩阵$M_{n}$
$$
   M = \begin{pmatrix}
    (-1)^n & ... & ... & ... & C_{n}^{j}(-1)^{n-j} \\
    C_{n}^{1}C_{n-1}^0(-1)^{n-1} & C_{n}^{1}C_{n-1}^{1}(-1)^{n-2} & ... & ... & 0 \\
    C_{n}^{2}C_{n-1}^0(-1)^{n-2} & C_{n}^{2}C_{n-1}^{1}(-1)^{n-3} & C_{n}^{2}C_{n}^{2}(-1)^{n-4} & ... & 0 \\
    ... & ... & ... & ... & ... \\
    C_{n}^{i}(-1)^{n-i} & 0 & 0 & 0 & 0
    \end{pmatrix}
$$
贝塞尔基矩阵元素$(M_{ij})$的通用计算公式如下：
$$
    M_{ij} = \begin{cases} 
        C_{n}^{i} \cdot C_{n-i}^{j} \cdot (-1)^{n - i - j} & \text{ if } n \geq (i+j) \\
        0 & \text{otherwise}
    \end{cases}
$$
- **符号调整**: 其中$((-1)^{n - i - j})$根据$(n - i - j)$的奇偶性调整符号。
- **边界条件**：当$n < (i+j)$时，矩阵元素 $(M_{ij})$为0。
## Demo
### 1.贝塞尔曲线
![贝塞尔曲线演示视频](./Video/demo1.gif)
### 2.贝塞尔曲面
![贝塞尔曲面演示视频](./Video/demo2.gif)