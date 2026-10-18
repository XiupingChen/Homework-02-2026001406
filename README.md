# 第二次作业

**Name:** Xiuping Chen  
**Student ID:** 2026001406

## Overview

This repository contains the code and report for Statistics Computing Homework 2. The assignment includes four main tasks:

1. Image convolution using R and RcppArmadillo
2. Multivariate normal log-density calculation
3. Kalman filtering and smoothing
4. Ridge regression, LOOCV, and Tikhonov regularization

## Requirements

The code requires R and the following packages:

```r
install.packages(c("Rcpp", "RcppArmadillo"))
```

The C++ files are compiled automatically through `Rcpp::sourceCpp()` when the corresponding R scripts are run.

## Files

### Question 1: Image Convolution

- `image-convolution.cpp`: C++ implementation of 3-by-3 image convolution
- `image-convolution.R`: R implementation, numerical comparison, timing, and plotting
- `image-convolution-results.png`: convolution results for the Gaussian, sharpen, and edge kernels

The implementation uses reflected boundary indices and clips all output pixels to the interval `[0, 1]`.

### Question 2: Multivariate Normal Log-Density

- `mvn-log-density.cpp`: C++ implementation of the multivariate normal log-density
- `mvn-log-density.R`: numerical tests and comparison with reference values

The calculation uses a Cholesky decomposition and triangular solves. It does not explicitly calculate the inverse or determinant of the covariance matrix.

### Question 3: Kalman Smoothing

- `kalman-smoothing.cpp`: implementation of Kalman filtering and Rauch–Tung–Striebel smoothing
- `kalman-smoothing.R`: simulation, validation, MSE calculation, and plotting
- `kalman-smoothing-results.png`: comparison of the filtering and smoothing paths

For the simulated data, the position MSE values are:

- Filtering: `0.25161834`
- Smoothing: `0.06381335`

### Question 4: Ridge and Tikhonov Regularization

#### Ridge Regression

- `ridge-cv.cpp`: implementations of the three LOOCV methods
- `ridge-cv.R`: validation, timing comparison, and LOOCV curve
- `ridge-cv-results.png`: LOOCV curves for the three methods

The three methods are:

- Direct leave-one-out refitting
- Leverage-based LOOCV
- SVD-based LOOCV

All three methods select the same value:

```text
lambda = 0.6951928
```

#### Tikhonov Regularization

- `tikhonov.cpp`: GSVD-based Tikhonov regularization
- `tikhonov.R`: comparison with the reference solution and Ridge regression

The numerical differences are at the level of floating-point rounding error.

## Running the Code

Open a terminal, change to the directory containing the files, and start R:

```bash
R
```

Run the scripts one at a time:

```r
source("image-convolution.R")
source("mvn-log-density.R")
source("kalman-smoothing.R")
source("ridge-cv.R")
source("tikhonov.R")
```

The scripts compile the corresponding C++ files, run the numerical checks, print the results, and save the required figures.

## Main Results

- The R and C++ image-convolution results agree within numerical precision.
- The C++ convolution implementation is substantially faster than the loop-based R implementation.
- The multivariate normal log-density agrees with all reference values.
- Kalman smoothing produces a lower MSE than Kalman filtering in the simulation.
- The Direct, Leverage, and SVD Ridge LOOCV methods give equivalent results.
- The SVD-based LOOCV method is the fastest of the three methods.
- The GSVD-based Tikhonov results agree with the direct reference solution.
- When the penalty matrix is the identity matrix, the Tikhonov result agrees with Ridge regression.

## Report

The complete experimental report is provided in:

```text
陈秀萍-实验报告.docx
```
