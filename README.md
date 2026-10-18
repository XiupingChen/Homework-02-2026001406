# Statistics Computing Homework 2

姓名：陈秀萍  
学号：2026001406

## 作业内容

本次作业包括四道题：

1. 使用 R 和 C++ 实现图像卷积
2. 计算多元正态分布的对数密度
3. 实现 Kalman filtering 和 Kalman smoothing
4. 计算 Ridge regression 的 LOOCV，并实现 Tikhonov regularization

## 文件说明

### 第一题

- `image-convolution.cpp`
- `image-convolution.R`
- `image-convolution-results.png`

### 第二题

- `mvn-log-density.cpp`
- `mvn-log-density.R`

### 第三题

- `kalman-smoothing.cpp`
- `kalman-smoothing.R`
- `kalman-smoothing-results.png`

### 第四题

- `ridge-cv.cpp`
- `ridge-cv.R`
- `ridge-cv-results.png`
- `tikhonov.cpp`
- `tikhonov.R`

## 运行环境

- R 4.6.0
- Rcpp
- RcppArmadillo

如果没有安装相应的 R 包，可以运行：

```r
install.packages(c("Rcpp", "RcppArmadillo"))
```

## 运行代码

在 R 中依次运行：

```r
source("image-convolution.R")
source("mvn-log-density.R")
source("kalman-smoothing.R")
source("ridge-cv.R")
source("tikhonov.R")
```

## 实验报告

实验过程和结果见：

```text
陈秀萍-实验报告.pdf
```
