Rcpp::sourceCpp("image-convolution.cpp")

# R uses indices 1, ..., length, unlike the C++ helper.
mirrorIndexR <- function(index, length) {
    if (index < 1L) return(1L - index)
    if (index > length) return(2L * length + 1L - index)
    index
}

imageConvR <- function(img, kernel) {
    nRows <- nrow(img)
    nCols <- ncol(img)

    result <- matrix(0, nrow = nRows, ncol = nCols)

    for (i in seq_len(nRows)) {
        for (j in seq_len(nCols)) {
            value <- 0

            for (a in -1:1) {
                for (b in -1:1) {
                    row <- mirrorIndexR(i + a, nRows)
                    col <- mirrorIndexR(j + b, nCols)

                    value <- value +
                        img[row, col] * kernel[a + 2, b + 2]
                }
            }

            if (value < 0) {
                value <- 0
            } else if (value > 1) {
                value <- 1
            }

            result[i, j] <- value
        }
    }

    return(result)
}

# A reproducible grayscale test image, with no extra image-package dependency.
grid <- seq(-1, 1, length.out = 256)
img <- outer(grid, grid, function(x, y) {
    0.2 + 0.5 * (x^2 + y^2 < 0.5) + 0.2 * (abs(x) < 0.2)
})
kernels <- list(
    gaussian = matrix(c(1, 2, 1, 2, 4, 2, 1, 2, 1), 3, 3) / 16,
    sharpen = matrix(c(0, -1, 0, -1, 5, -1, 0, -1, 0), 3, 3),
    edge = matrix(c(-1, -1, -1, -1, 8, -1, -1, -1, -1), 3, 3)
)

results <- lapply(kernels, function(kernel) {
    cpp <- imageConv(img, kernel)
    r <- imageConvR(img, kernel)
    stopifnot(identical(dim(cpp), dim(img)),
              all(cpp >= 0 & cpp <= 1), max(abs(cpp - r)) < 1e-10)
    cpp
})
print(vapply(seq_along(kernels), function(i) {
    max(abs(results[[i]] - imageConvR(img, kernels[[i]])))
}, numeric(1)))

# Compile and warm up outside timing; report elapsed seconds per call.
repetitions <- 10L
kernel <- kernels$gaussian
invisible(imageConv(img, kernel))
invisible(imageConvR(img, kernel))
cpp_time <- system.time(for (i in seq_len(repetitions)) {
    imageConv(img, kernel)
})[["elapsed"]] / repetitions
r_time <- system.time(for (i in seq_len(repetitions)) {
    imageConvR(img, kernel)
})[["elapsed"]] / repetitions
print(data.frame(method = c("C++", "R"), seconds = c(cpp_time, r_time),
                 repetitions = repetitions, rows = nrow(img), cols = ncol(img)))

drawImage <- function(values, title) {
    plot.new()
    plot.window(xlim = c(0, 1), ylim = c(0, 1), asp = 1)
    rasterImage(as.raster(values), 0, 0, 1, 1, interpolate = FALSE)
    title(main = title)
}
png("image-convolution-results.png",
    width = 1600, height = 1600, res = 200)

old_par <- par(mfrow = c(2, 2), mar = c(1, 1, 3, 1))

drawImage(img, "Original")

for (name in names(results)) {
    drawImage(results[[name]], name)
}

par(old_par)
dev.off()

cat("Figure saved as: image-convolution-results.png\n")
