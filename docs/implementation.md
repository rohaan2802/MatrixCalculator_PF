# Implementation and numerical behavior

## Native program

src/matrix_calculator.cpp retains fixed-size double arrays with a maximum square dimension of 10. The original 19 menu options remain available. Most operations copy or read the working matrix. Loading a sample deliberately replaces it.

Input uses token parsing. Integer bounds are checked before conversion to int. Non-finite and out-of-range floating-point input is rejected. EOF exits matrix, vector and scalar entry. Scientific notation is used for tiny and large displayed values.

## Elimination

Determinant, inverse, solve and rank first scale each row by its maximum absolute entry. Partial pivoting then chooses the largest available column entry in that equilibrated system. Pivots smaller than EPS = 1e-9 are treated as numerically zero.

The determinant tracks a normalized fraction and binary exponent while restoring row scales and pivot factors. This avoids premature overflow or underflow when extreme scales cancel in the final product. Inverse and solve apply the same scaling to the augmented right-hand side. The algorithms operate on temporary arrays, preserving the input matrix.

Display/input work takes O(n²), multiplication and elimination take O(n³), and the general cofactor adjugate takes O(n⁵). At the supported maximum n = 10, the cofactor fallback remains bounded. Direct formulas handle sizes 1 to 3. Larger invertible matrices use inverse times determinant, with cofactors providing the singular fallback.

Finite input can still produce overflow, underflow or inaccurate results near floating-point limits. Row scaling improves pivot decisions but does not make ill-conditioned problems exact. The native determinant uses frexp and ldexp to combine scale factors without relying on extended-precision long double. Symmetry and identity checks use an absolute EPS comparison.

## Result lifecycle

The most recent result has its own operation label. Opening About or saving a report does not relabel the stored calculation. Non-finite calculated values are rejected before replacing the stored result. A report includes the latest historical determinant separately from the result operation.

Saving writes a process-specific temporary file, verifies the stream and replaces result.txt. Windows uses MoveFileEx with replace/write-through flags. POSIX uses rename. A failed replacement preserves the previous report.

## Browser implementation

assets/js/matrix.js implements the same row-equilibrated methods in JavaScript. The playground supports display, determinant, inverse, adjugate, transpose, trace, rank, arithmetic, scalar multiplication, solve and property checks. It accepts square matrices with 1 to 10 rows. Separate vector input is used for solve.

The browser uses JavaScript Number and tracks determinant fractions and exponents explicitly, matching the native approach. It reports non-finite results as errors. No matrix data is sent to a server. The scripts use one MatrixCalculator namespace and run in a direct file preview or through an HTTP server.

## Reproducible evidence

data/demo_cases.json drives the native replay script. examples/transcripts stores complete unedited stdout. docs/reports/native_results.json stores case metadata and focused excerpts for the gallery. The 24 frames are generated from those records.

tests/matrix_regression.cpp includes the maintained implementation with its interactive main disabled and checks numerical values, inverse residuals, singular cases, maximum size, input validation and save provenance. tests/browser_math.test.js checks the browser implementation independently. The browser verification script exercises the visible controls and compares representative numerical results.
