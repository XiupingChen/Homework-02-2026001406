#include <RcppArmadillo.h>
// [[Rcpp::depends(RcppArmadillo)]]

// [[Rcpp::export]]
double mvnLogDensity(const arma::vec& x, const arma::vec& mu,
                     const arma::mat& Sigma) {
    // Cholesky分解：Sigma = L * L^T
    arma::mat L = arma::chol(Sigma, "lower");

    // 计算x与mu之差
    arma::vec difference = x - mu;

    // 求解L * v = x - mu，不计算L的逆矩阵
    arma::vec v = arma::solve(arma::trimatl(L), difference);

    // 计算Cholesky因子对角线元素对数之和
    double logDiagonalSum = arma::sum(arma::log(L.diag()));

    // 计算二次型v^T v
    double quadraticTerm = arma::dot(v, v);

    double p = static_cast<double>(x.n_elem);

    return -0.5 * p * std::log(2.0 * arma::datum::pi)
           - logDiagonalSum
           - 0.5 * quadraticTerm;
}
