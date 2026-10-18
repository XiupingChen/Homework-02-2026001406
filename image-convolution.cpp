#include <RcppArmadillo.h>
// [[Rcpp::depends(RcppArmadillo)]]

// Only adjacent indices outside the image are needed for a 3-by-3 kernel.
int mirrorIndex(int index, int length) {
    if (index < 0) return -index - 1;
    if (index >= length) return 2 * length - index - 1;
    return index;
}

// [[Rcpp::export]]
arma::mat imageConv(const arma::mat& img, const arma::mat& kernel) {
    int nRows = static_cast<int>(img.n_rows);
    int nCols = static_cast<int>(img.n_cols);

    // 创建与原图尺寸相同的结果矩阵，初始值全部为0
    arma::mat result(nRows, nCols, arma::fill::zeros);

    // 遍历图像中的每一个像素
    for (int i = 0; i < nRows; ++i) {
        for (int j = 0; j < nCols; ++j) {
            double value = 0.0;

            // 遍历3×3卷积核
            for (int a = -1; a <= 1; ++a) {
                for (int b = -1; b <= 1; ++b) {
                    // 对越过图像边界的下标进行镜像处理
                    int row = mirrorIndex(i + a, nRows);
                    int col = mirrorIndex(j + b, nCols);

                    // 直接使用卷积核，不进行翻转
                    value += img(row, col) * kernel(a + 1, b + 1);
                }
            }

            // 将计算结果截断到[0, 1]
            if (value < 0.0) {
                value = 0.0;
            } else if (value > 1.0) {
                value = 1.0;
            }

            result(i, j) = value;
        }
    }

    return result;
}
