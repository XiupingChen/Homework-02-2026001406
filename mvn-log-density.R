Rcpp::sourceCpp("mvn-log-density.cpp")

checkDensity <- function(x, mu, Sigma, expected) {
    actual <- mvnLogDensity(x, mu, Sigma)
    stopifnot(length(actual) == 1L, is.finite(actual),
              abs(actual - expected) < 1e-10)
    print(c(actual = actual, expected = expected))
}

# One dimension: compare with base R's normal log-density.
checkDensity(1.2, 0.3, matrix(2.25, 1, 1),
             dnorm(1.2, mean = 0.3, sd = 1.5, log = TRUE))

# Independent coordinates: sum univariate log-densities.
x <- c(1, -2, 0.5)
mu <- c(0.2, 0, -0.3)
variances <- c(1, 2, 3)
checkDensity(x, mu, diag(variances),
             sum(dnorm(x, mu, sqrt(variances), log = TRUE)))

# A non-diagonal covariance: construct Sigma = L L^T with known L.
L <- matrix(c(1.5, 0.4, -0.2, 0, 1.2, 0.3, 0, 0, 0.8), 3, 3)
v <- c(0.5, -0.7, 1.1)
Sigma <- L %*% t(L)
x <- as.vector(mu + L %*% v)
expected <- -length(x) / 2 * log(2 * pi) - sum(log(diag(L))) - sum(v^2) / 2
checkDensity(x, mu, Sigma, expected)

# Test at the mean with a correlated two-dimensional covariance matrix.
x_custom <- c(1, -1)
mu_custom <- c(1, -1)

Sigma_custom <- matrix(
    c(4, 1,
      1, 2),
    nrow = 2,
    byrow = TRUE
)

expected_custom <- -length(x_custom) / 2 * log(2 * pi) -
    0.5 * as.numeric(determinant(
        Sigma_custom,
        logarithm = TRUE
    )$modulus)

checkDensity(
    x_custom,
    mu_custom,
    Sigma_custom,
    expected_custom
)
