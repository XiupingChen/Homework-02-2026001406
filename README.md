# 统计计算课程作业二

**姓名：** 陈秀萍  
**学号：** 2026001406

## 作业简介

本项目为统计计算课程第二次作业，主要包括以下内容：

1. 使用 R 和 RcppArmadillo 实现图像卷积
2. 计算多元正态分布的对数概率密度
3. 实现 Kalman filtering 和 Kalman smoothing
4. 实现 Ridge regression 的留一交叉验证及 Tikhonov regularization

## 运行环境

运行代码需要安装 R，以及以下 R 包：

```r
install.packages(c("Rcpp", "RcppArmadillo"))
```

各 R 文件会通过 `Rcpp::sourceCpp()` 自动编译对应的 C++ 文件。

## 文件说明

### 第一题：图像卷积

- `image-convolution.cpp`：使用 C++ 实现 3×3 图像卷积
- `image-convolution.R`：R 版本实现、数值验证、运行时间比较和结果绘图
- `image-convolution-results.png`：原始图像及 Gaussian、Sharpen 和 Edge 卷积结果

程序对图像边界进行镜像处理，并将卷积后的像素值限制在 `[0, 1]` 范围内。

### 第二题：多元正态分布对数概率密度

- `mvn-log-density.cpp`：使用 C++ 计算多元正态分布的对数概率密度
- `mvn-log-density.R`：测试计算结果并与参考值进行比较

程序使用 Cholesky 分解和三角方程求解进行计算，不显式计算协方差矩阵的逆矩阵和行列式。

### 第三题：Kalman Smoothing

- `kalman-smoothing.cpp`：实现 Kalman filtering 和 Rauch–Tung–Striebel smoothing
- `kalman-smoothing.R`：数据模拟、结果验证、MSE 计算和绘图
- `kalman-smoothing-results.png`：Filtering 和 Smoothing 的路径估计结果

本次模拟得到的位置均方误差为：

- Filtering：`0.25161834`
- Smoothing：`0.06381335`

Smoothing 的位置估计误差低于 Filtering。

### 第四题：Ridge 与 Tikhonov Regularization

#### Ridge Regression

- `ridge-cv.cpp`：实现三种 Ridge LOOCV 方法
- `ridge-cv.R`：数值验证、运行时间比较和 LOOCV 曲线绘制
- `ridge-cv-results.png`：三种方法得到的 LOOCV 曲线

实现的三种方法为：

- Direct：逐个删除观测并重新拟合
- Leverage：利用平滑矩阵的对角元素计算 LOOCV
- SVD：对设计矩阵进行一次奇异值分解，并在所有候选参数中重复使用分解结果

三种方法选择了相同的最优参数：

```text
lambda = 0.6951928
```

#### Tikhonov Regularization

- `tikhonov.cpp`：基于 GSVD 实现 Tikhonov regularization
- `tikhonov.R`：与参考解和 Ridge regression 结果进行比较

计算结果表明，GSVD 方法得到的系数和拟合值与参考解一致。当惩罚矩阵为单位矩阵时，Tikhonov regularization 的结果与 Ridge regression 一致。

## 运行方法

首先在终端中进入存放作业文件的文件夹，然后启动 R：

```bash
R
```

依次运行以下程序：

```r
source("image-convolution.R")
source("mvn-log-density.R")
source("kalman-smoothing.R")
source("ridge-cv.R")
source("tikhonov.R")
```

各 R 文件会自动编译对应的 C++ 文件，运行数值验证并输出结果。涉及绘图的程序还会在当前文件夹中保存相应的 PNG 图片。

## 主要结果

- R 与 C++ 的图像卷积结果在浮点计算误差范围内一致。
- C++ 图像卷积的运行速度明显快于使用循环实现的 R 版本。
- 多元正态分布对数概率密度的计算结果与参考值一致。
- 本次模拟中，Kalman smoothing 的位置估计误差低于 Kalman filtering。
- 三种 Ridge LOOCV 方法得到的结果和最优参数一致。
- SVD 方法在三种 Ridge LOOCV 方法中运行速度最快。
- GSVD 方法得到的 Tikhonov regularization 结果与参考解一致。
- 当惩罚矩阵为单位矩阵时，Tikhonov regularization 与 Ridge regression 一致。

## 实验报告

完整实验报告见：

```text
陈秀萍-实验报告.pdf
```
