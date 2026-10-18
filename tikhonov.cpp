#include <RcppArmadillo.h>
#include <cmath>
// [[Rcpp::depends(RcppArmadillo)]]

// Provided helper: complete, no student implementation required.
// Supports n >= p and a well-conditioned, invertible p-by-p B only.
// This is not a general-purpose GSVD algorithm for arbitrary or singular B.
// [[Rcpp::export]]
Rcpp::List computeGSVD(const arma::mat& X, const arma::mat& B) {
    if (X.n_cols == 0 || X.n_rows < X.n_cols ||
        B.n_rows != X.n_cols || B.n_cols != X.n_cols ||
        !X.is_finite() || !B.is_finite()) {
        Rcpp::stop("Require finite X (n-by-p, n >= p) and B (p-by-p).");
    }
    const double reciprocal_condition = arma::rcond(B);
    if (!std::isfinite(reciprocal_condition) || reciprocal_condition < 1e-10) {
        Rcpp::stop("This helper requires a nonsingular, well-conditioned B.");
    }

    // Form (X B^{-1})^T by solving a system, without an explicit inverse.
    arma::mat transformed_X_t;
    if (!arma::solve(transformed_X_t, B.t(), X.t(), arma::solve_opts::no_approx)) {
        Rcpp::stop("Failed to construct the transformed design.");
    }
    arma::mat U, V;
    arma::vec s;
    if (!arma::svd_econ(U, s, V, transformed_X_t.t())) {
        Rcpp::stop("SVD failed.");
    }

    arma::vec scale(s.n_elem);
    for (arma::uword i = 0; i < s.n_elem; ++i) {
        scale(i) = std::hypot(s(i), 1.0);
    }
    arma::vec alpha = s / scale;
    arma::vec beta = 1.0 / scale;
    arma::mat scaled_V = V;
    scaled_V.each_row() /= scale.t();
    arma::mat Z;
    if (!arma::solve(Z, B, scaled_V, arma::solve_opts::no_approx)) {
        Rcpp::stop("Failed to construct Z.");
    }

    // X Z = U diag(alpha), B Z = V diag(beta), alpha^2 + beta^2 = 1.
    // Return alpha and beta as R vectors, rather than p-by-1 matrices.
    return Rcpp::List::create(
        Rcpp::Named("U") = U,
        Rcpp::Named("U2") = V,
        Rcpp::Named("Z") = Z,
        Rcpp::Named("alpha") = Rcpp::NumericVector(alpha.begin(), alpha.end()),
        Rcpp::Named("beta") = Rcpp::NumericVector(beta.begin(), beta.end()));
}

// [[Rcpp::export]]
Rcpp::List tikhonovPath(const arma::mat& U, const arma::mat& Z,
                        const arma::vec& alpha, const arma::vec& beta,
                        const arma::vec& y, const arma::vec& lambdas) {
    arma::uword n = U.n_rows;
    arma::uword p = U.n_cols;
    arma::uword M = lambdas.n_elem;

    arma::mat coefficients(p, M, arma::fill::zeros);
    arma::mat fitted(n, M, arma::fill::zeros);

    // Compute U' y only once and reuse it for every lambda.
    arma::vec Uty = U.t() * y;

    arma::vec alpha_squared = arma::square(alpha);
    arma::vec beta_squared = arma::square(beta);

    for (arma::uword m = 0; m < M; ++m) {
        arma::vec denominator =
            alpha_squared + lambdas(m) * beta_squared;

        // Coefficients:
        // sum_i [alpha_i (u_i' y) /
        //        (alpha_i^2 + lambda beta_i^2)] z_i
        arma::vec coefficient_weights =
            (alpha % Uty) / denominator;

        coefficients.col(m) =
            Z * coefficient_weights;

        // Fitted values:
        // sum_i [alpha_i^2 (u_i' y) /
        //        (alpha_i^2 + lambda beta_i^2)] u_i
        arma::vec fitted_weights =
            (alpha_squared % Uty) / denominator;

        fitted.col(m) =
            U * fitted_weights;
    }

    return Rcpp::List::create(
        Rcpp::Named("coefficients") = coefficients,
        Rcpp::Named("fitted") = fitted
    );
}