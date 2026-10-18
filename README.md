
姓名：陈秀萍  
学号：2026001406  

## 作业内容

本次作业使用 R、Rcpp 和 RcppArmadillo 完成，共包括以下四道题。

### 1. 图像卷积

分别使用 R 和 C++ 循环实现 \(3\times3\) 灰度图像卷积，边界采用镜像延拓，并将输出像素限制在 \([0,1]\) 内。程序使用 Gaussian、Sharpen 和 Edge 三种卷积核比较 R 与 C++ 的计算结果，同时记录两种实现的运行时间。

对应文件：

- `image-convolution.cpp`
- `image-convolution.R`


### 2. 多元正态分布的对数概率密度

使用下三角 Cholesky 分解计算多元正态分布的对数概率密度。计算过程中通过求解三角方程得到二次型，不显式计算协方差矩阵的逆矩阵，也不直接计算行列式。

对应文件：

- `mvn-log-density.cpp`
- `mvn-log-density.R`

R 脚本包含一维正态分布、对角协方差矩阵、非对角协方差矩阵和自定义情形的测试。

### 3. Kalman smoothing

在已有 Kalman filtering 程序的基础上，实现前向过滤和后向平滑。程序保存预测均值、预测协方差、过滤均值和过滤协方差，并使用后向递推计算各时刻的平滑状态估计。

对应文件：

- `kalman-filtering.cpp`
- `kalman-smoothing.cpp`
- `kalman-smoothing.R`

运行后输出 filtering 和 smoothing 的位置均方误差。

### 4. Ridge regression 与 Tikhonov regularization

第一部分分别使用 Direct、Leverage 和 SVD 三种方法计算 Ridge regression 的 LOOCV 误差，比较三种方法的数值结果、最优参数和运行时间。

第二部分使用题目提供的 GSVD 辅助函数计算 Tikhonov regularization 在一组参数下的系数和拟合值，并与直接求解得到的参考结果进行比较。

对应文件：

- `ridge-cv.cpp`
- `ridge-cv.R`
- `tikhonov.cpp`
- `tikhonov.R`

## 运行环境

- R 4.6.0
- Rcpp
- RcppArmadillo 15.6.2-1

## 编译与运行

所有 R 和 C++ 文件应放在同一个目录中，并从该目录启动 R。

各 R 脚本开头已经包含 `Rcpp::sourceCpp()`，因此运行 R 脚本时会自动编译对应的 C++ 文件。

```r
source("image-convolution.R")
source("mvn-log-density.R")
source("kalman-smoothing.R")
source("ridge-cv.R")
source("tikhonov.R")
```

其中：

- `image-convolution.R` 编译 `image-convolution.cpp`
- `mvn-log-density.R` 编译 `mvn-log-density.cpp`
- `kalman-smoothing.R` 编译 `kalman-filtering.cpp` 和 `kalman-smoothing.cpp`
- `ridge-cv.R` 编译 `ridge-cv.cpp`
- `tikhonov.R` 编译 `tikhonov.cpp`

各题的实现方法、数值结果、运行时间和结果图片见 `report.pdf`。

## AI 使用说明

本次作业使用 ChatGPT/Codex 作为辅助工具，涉及第 1～4 题。AI 主要用于讨论部分算法的实现思路、为部分 TODO 的写法提供参考、检查代码中可能存在的问题，以及修改实验报告和 README 的文字表述。

完成代码后，我在本地逐题进行了编译和运行，并根据题目要求检查了 R 与 C++ 卷积结果的一致性、多元正态对数密度的参考值、Kalman smoothing 的输出维度和末时刻结果、三种 Ridge LOOCV 方法的数值差异，以及 Tikhonov regularization 与参考解的一致性。最终提交的代码和结果均以本地运行及验证结果为准。
