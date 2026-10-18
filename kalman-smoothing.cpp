#include <RcppArmadillo.h>
#include <vector>
// [[Rcpp::depends(RcppArmadillo)]]

// [[Rcpp::export]]
arma::mat kalmanSmoothing(arma::mat Yobs, arma::vec z0,
                         arma::mat A, arma::mat C,
                         double sigmaQ, double sigmaR) {
    const arma::uword p = z0.n_elem;
    const arma::uword tTotal = Yobs.n_cols;

    if (tTotal == 0) {
        return arma::mat(p, 0);
    }

    arma::vec mu = z0;
    arma::mat Sigma = arma::zeros<arma::mat>(p, p);

    // 保存每个时刻的预测均值和过滤均值
    arma::mat predictedMeans =
        arma::zeros<arma::mat>(p, tTotal);

    arma::mat filteredMeans =
        arma::zeros<arma::mat>(p, tTotal);

    // 保存每个时刻的预测协方差和过滤协方差
    std::vector<arma::mat> predictedCovariances(tTotal);
    std::vector<arma::mat> filteredCovariances(tTotal);

    // 前向Kalman filtering
    for (arma::uword t = 0; t < tTotal; ++t) {
        // Prediction
        arma::vec predictedMean = A * mu;

        arma::mat predictedCovariance =
            A * Sigma * A.t();

        predictedCovariance.diag() += sigmaQ * sigmaQ;

        predictedMeans.col(t) = predictedMean;
        predictedCovariances[t] = predictedCovariance;

        // Measurement update
        arma::mat S =
            C * predictedCovariance * C.t();

        S.diag() += sigmaR * sigmaR;

        arma::mat kalmanGain =
            arma::solve(
                S,
                C * predictedCovariance
            ).t();

        mu = predictedMean +
            kalmanGain *
            (Yobs.col(t) - C * predictedMean);

        Sigma = predictedCovariance -
            kalmanGain * C * predictedCovariance;

        filteredMeans.col(t) = mu;
        filteredCovariances[t] = Sigma;
    }

    // 最后一个时刻的平滑结果等于过滤结果
    arma::mat smoothedMeans = filteredMeans;

    // 后向Kalman smoothing
    for (int t = static_cast<int>(tTotal) - 2;
         t >= 0;
         --t) {
        arma::mat smoothingGain =
            arma::solve(
                predictedCovariances[t + 1],
                A * filteredCovariances[t]
            ).t();

        smoothedMeans.col(t) =
            filteredMeans.col(t) +
            smoothingGain *
            (smoothedMeans.col(t + 1) -
             predictedMeans.col(t + 1));
    }

    return smoothedMeans;
}
