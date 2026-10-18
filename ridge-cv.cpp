#include <RcppArmadillo.h>
// [[Rcpp::depends(RcppArmadillo)]]

// All methods use sum of squared errors + lambda * squared coefficient norm.
// Return one LOOCV sum of squared prediction errors per positive lambda.

// [[Rcpp::export]]
arma::vec ridgeCVDirect(const arma::mat& X, const arma::vec& y,
                        const arma::vec& lambdas) {
    arma::uword n = X.n_rows;
    arma::uword M = lambdas.n_elem;
    arma::vec cv_error(M, arma::fill::zeros);

    for (arma::uword m = 0; m < M; ++m) {
        double lambda = lambdas(m);

        for (arma::uword i = 0; i < n; ++i) {
            // Copy the data and remove observation i.
            arma::mat X_train = X;
            arma::vec y_train = y;
            X_train.shed_row(i);
            y_train.shed_row(i);

            // Solve (X'X + lambda I) beta = X'y.
            arma::mat system = X_train.t() * X_train;
            system.diag() += lambda;

            arma::vec beta = arma::solve(
                system,
                X_train.t() * y_train,
                arma::solve_opts::likely_sympd
            );

            double prediction = arma::as_scalar(X.row(i) * beta);
            double error = y(i) - prediction;
            cv_error(m) += error * error;
        }
    }

    return cv_error;
}


// [[Rcpp::export]]
arma::vec ridgeCVLeverage(const arma::mat& X, const arma::vec& y,
                          const arma::vec& lambdas) {
    arma::uword M = lambdas.n_elem;
    arma::vec cv_error(M, arma::fill::zeros);

    arma::mat XtX = X.t() * X;
    arma::vec Xty = X.t() * y;

    for (arma::uword m = 0; m < M; ++m) {
        arma::mat system = XtX;
        system.diag() += lambdas(m);

        // Fit ridge regression using all observations.
        arma::vec beta = arma::solve(
            system,
            Xty,
            arma::solve_opts::likely_sympd
        );

        arma::vec fitted = X * beta;
        arma::vec residuals = y - fitted;

        // Compute the diagonal of
        // S = X (X'X + lambda I)^(-1) X'
        // without explicitly calculating an inverse.
        arma::mat solved_Xt = arma::solve(
            system,
            X.t(),
            arma::solve_opts::likely_sympd
        );

        arma::vec leverage =
            arma::sum(X % solved_Xt.t(), 1);

        arma::vec loo_residuals =
            residuals / (1.0 - leverage);

        cv_error(m) =
            arma::dot(loo_residuals, loo_residuals);
    }

    return cv_error;
}


// [[Rcpp::export]]
arma::vec ridgeCVSVD(const arma::mat& X, const arma::vec& y,
                     const arma::vec& lambdas) {
    arma::uword M = lambdas.n_elem;
    arma::vec cv_error(M, arma::fill::zeros);

    // Compute the economy-size SVD only once.
    arma::mat U;
    arma::mat V;
    arma::vec singular_values;
    arma::svd_econ(U, singular_values, V, X);

    arma::vec squared_values =
        arma::square(singular_values);

    arma::vec Uty = U.t() * y;
    arma::mat squared_U = arma::square(U);

    for (arma::uword m = 0; m < M; ++m) {
        // d_j^2 / (d_j^2 + lambda)
        arma::vec shrinkage =
            squared_values / (squared_values + lambdas(m));

        arma::vec fitted =
            U * (shrinkage % Uty);

        arma::vec leverage =
            squared_U * shrinkage;

        arma::vec loo_residuals =
            (y - fitted) / (1.0 - leverage);

        cv_error(m) =
            arma::dot(loo_residuals, loo_residuals);
    }

    return cv_error;
}