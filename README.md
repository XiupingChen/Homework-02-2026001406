# Homework 2

姓名：陈秀萍  
学号：2026001406  

## 作业内容

本次作业共包括四部分：

1. 使用 R 和 RcppArmadillo 实现图像卷积，并比较两种实现的计算结果和运行时间。
2. 使用 Cholesky 分解计算多元正态分布的对数概率密度。
3. 在 Kalman filtering 的基础上实现 Kalman smoothing。
4. 实现 Ridge regression 的三种 LOOCV 方法和 Tikhonov regularization。

## 运行环境

- R 4.6.0
- Rcpp
- RcppArmadillo 15.6.2-1

## 编译与运行

所有文件放在同一目录下，从该目录启动 R，然后依次运行：

```r
source("image-convolution.R")
source("mvn-log-density.R")
source("kalman-smoothing.R")
source("ridge-cv.R")
source("tikhonov.R")
```

各 R 脚本通过 `Rcpp::sourceCpp()` 编译对应的 C++ 文件。具体实现、测试结果和运行时间见 `report.pdf`。

## AI 使用说明

本次作业使用了 ChatGPT/Codex，涉及第 1～4 题，主要用于检查部分实现思路、排查代码问题、核对测试结果，以及修改报告和 README 的文字表述。完成后，我在本地运行了全部 R 脚本，并按照题目要求检查了数值误差、输出维度和不同方法之间的一致性。
