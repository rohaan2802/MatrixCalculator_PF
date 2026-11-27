<div align="center">

# MatrixCalculator_PF

**Square-matrix calculations in C++17, with a browser playground and reproducible numerical evidence.**

[![C++17](https://img.shields.io/badge/C%2B%2B-17-16635b?logo=cplusplus&logoColor=white)](src/matrix_calculator.cpp)
[![CMake](https://img.shields.io/badge/Build-CMake-16635b?logo=cmake&logoColor=white)](CMakeLists.txt)
[![Visual Studio](https://img.shields.io/badge/IDE-Visual_Studio-16635b?logo=visualstudio&logoColor=white)](MatrixCalculator_PF.sln)
[![Native demo](https://img.shields.io/badge/Native_cases-24-16635b?logo=windows&logoColor=white)](docs/reports/native_results.json)
[![Browser](https://img.shields.io/badge/Browser-Calculator-16635b?logo=javascript&logoColor=white)](#interactive-browser)
[![Tests](https://img.shields.io/badge/Checks-Numerical-16635b?logo=githubactions&logoColor=white)](tests/matrix_regression.cpp)
[![Source](https://img.shields.io/badge/GitHub-Source-16635b?logo=github&logoColor=white)](https://github.com/rohaan2802/MatrixCalculator_PF)
[![Pages](https://img.shields.io/badge/Pages-Case_study-16635b?logo=githubpages&logoColor=white)](https://rohaan2802.github.io/MatrixCalculator_PF/)

</div>

## Live Links

| What | Link |
| --- | --- |
| Repository | [https://github.com/rohaan2802/MatrixCalculator_PF](https://github.com/rohaan2802/MatrixCalculator_PF) |
| GitHub Pages | [https://rohaan2802.github.io/MatrixCalculator_PF/](https://rohaan2802.github.io/MatrixCalculator_PF/) |
| CMake tutorial | [https://cmake.org/cmake/help/latest/guide/tutorial/index.html](https://cmake.org/cmake/help/latest/guide/tutorial/index.html) |

## Contents

1. [Live Links](#live-links)
2. [Overview](#overview)
3. [What's Inside](#whats-inside)
4. [Architecture and Design](#architecture-and-design)
5. [Project Breakdown](#project-breakdown)
6. [Console Workflow](#console-workflow)
7. [Setup and Build](#setup-and-build)
8. [Reproduce the Demo](#reproduce-the-demo)
9. [Results and Verification](#results-and-verification)
10. [Engineering Quality Signals](#engineering-quality-signals)
11. [Folder Structure](#folder-structure)
12. [Limitations](#limitations)
13. [Interactive Browser](#interactive-browser)
14. [Source and Downloads](#source-and-downloads)
15. [Figure Index](#figure-index)
16. [Screenshots](#screenshots)
17. [About the Author](#about-the-author)

## Overview

MatrixCalculator_PF is a C++17 square-matrix console calculator developed from a Programming Fundamentals project at FAST-NUCES. It retains 19 menu options and supports matrices from 1x1 to 10x10 using fixed-size double arrays.

The maintained version adds a reproducible CMake build, numerical regression checks, real executable transcripts, and an interactive browser implementation. The native application and browser playground have separate entry points, with the same row-equilibrated elimination approach for determinant, inverse, solve and rank.

## What's Inside

The repository keeps the maintained source, IDE project, input fixtures, saved outputs and verification tools together.

| Material | Location |
| --- | --- |
| Native C++ application | src/matrix_calculator.cpp |
| CMake build and CTest target | CMakeLists.txt |
| Visual Studio solution | MatrixCalculator_PF.sln |
| Recorded demo inputs | data/demo_cases.json |
| Actual native stdout | examples/transcripts |
| Saved result example | examples/saved_result.txt |
| Browser math and interface | assets/js/matrix.js and assets/js/main.js |
| Native replay report | docs/reports/native_results.json |
| Historical implementation and panels | docs/archive/README.md |

## Architecture and Design

The project is split into four deliberate layers: the native application owns the original interactive workflow, the browser layer provides a zero-install demonstration, the evidence layer records repeatable outputs, and the presentation layer turns those outputs into a reviewable portfolio.

The C++ and browser calculators share the same numerical choices—row equilibration, partial pivoting, finite-value checks and bounded 1–10 dimensions—but remain separate entry points so the native assignment stays easy to compile and inspect. The replay data is the contract between the executable and the evidence gallery.

This separation keeps the demo honest: figures are generated from captured executable output, while the browser playground is clearly labelled as a live local calculation. A recruiter can inspect one source file, run one build command, reproduce the test cases and then explore the UI without credentials or a service dependency.

| Layer | Responsibility | Primary files |
| --- | --- | --- |
| Native application | 19-option console workflow and numerical operations | src/matrix_calculator.cpp |
| Verification | Known inputs, regression assertions and replayed stdout | tests/ · data/ · examples/ |
| Browser playground | Live local calculations, JSON export, themes and gallery controls | index.html · assets/js/ · assets/css/ |
| Presentation | Shared README/site content, figures and provenance reports | tools/build_portfolio.py · docs/reports/ |

## Project Breakdown

Input is entered one value at a time in the console, with confirmation before matrix or vector entry. The browser accepts comma-separated or space-separated rows. Both paths validate square dimensions and finite numeric input.

The working matrix is preserved by calculations. Loading a sample is the explicit replacement action. A separate result label keeps the calculation identity intact when About or Save is selected.

For elimination, each row is divided by its largest absolute entry. Partial pivoting selects the largest available column entry in the equilibrated system. A normalized pivot below 1e-9 is treated as numerically zero. Determinant calculations restore row scale factors with fraction/exponent tracking to avoid premature range loss, while inverse and solve scale their augmented right-hand sides.

Direct adjugate formulas handle orders 1 to 3. Larger invertible matrices use inverse times determinant in the native program. The cofactor fallback also handles larger singular matrices, including the recorded singular 4x4 example.

| Operation family | Method | Work and storage |
| --- | --- | --- |
| Display, transpose, addition, scaling | Bounded nested loops | O(n²) time |
| Matrix multiplication | Row-column dot products | O(n³) time |
| Determinant, inverse, solve, rank | Row equilibration and partial pivoting | O(n³) time |
| General adjugate fallback | Determinants of transposed cofactors | O(n⁵) time |
| Working matrices | Fixed arrays, maximum order 10 | O(n²) storage |

## Console Workflow

Start with option 1, enter the square order, confirm matrix input and enter values row by row. The menu then offers the original operations 1 to 8 and the extended tools 9 to 19.

After a calculation, Y reuses the current matrix, N returns to the welcome screen for another matrix, and 0 exits. Confirm screens also accept a back action. A zero entered as a matrix or vector value remains a numeric zero.

Save writes result.txt in the process working directory. The report identifies the operation behind the stored result and labels the most recent historical determinant separately. A process-specific temporary file and replacement step protect the previous report if a write cannot complete.

| Menu | Available action |
| --- | --- |
| 1–3 | Display, symmetry and identity checks |
| 4–7 | Determinant, adjugate, inverse and solve |
| 8 | Exit |
| 9–15 | Transpose, addition, subtraction, multiplication, scalar, trace and rank |
| 16–19 | Sample presets, About, save and last-result review |

## Setup and Build

The native application needs a C++17 compiler and CMake 3.20 or newer. Visual Studio 2022 with Desktop development with C++ can open MatrixCalculator_PF.sln directly. CMake can also build with GCC or Clang.

Run the following from the repository root. For a single-configuration generator, the executable is build/matrix_calculator, with .exe on Windows. Visual Studio generators place the Release executable in build/Release. run.bat builds and starts the Windows program. build.ps1 -Test builds and runs CTest.

Python with Pillow regenerates the presentation. Playwright and installed Google Chrome run the browser verification script. The source scripts are listed in requirements-dev.txt. The browser page itself uses local JavaScript and needs no package download.

On Windows, build.ps1 selects Ninja and GCC when both are on PATH, or uses the default CMake generator otherwise. An existing build cache keeps its selected generator. For an explicit GCC build, use cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++.

Install the optional presentation and verification tools with python -m pip install -r requirements-dev.txt. Node.js is needed only for npm test; the JavaScript tests use built-in modules.

~~~text
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
python -m pip install -r requirements-dev.txt
python tools/collect_results.py
python tools/build_portfolio.py
npm test
python -m http.server 8000
~~~

## Reproduce the Demo

The recorded 2x2 working matrix is A = [[4,7],[2,6]]. Its determinant is 10. Its inverse is [[0.6,-0.7],[-0.2,0.4]]. For b = [18,14], the known solution is x = [1,2].

The native replay script reads data/demo_cases.json, supplies inputs through the executable's normal menu, checks expected output and saves full stdout. The 24 figures use focused excerpts from those actual runs. Each figure links to its full transcript.

The webpage playground calculates new inputs locally in the browser. Use a saved example to populate a matrix, choose an operation and calculate. The saved-case viewer shows native results separately, making it possible to compare live browser calculations with recorded console evidence.

## Results and Verification

The native replay records 24 passing cases. Four additional input-flow checks cover truncated matrix, vector and scalar input, and an overflowing size token. The regression executable checks known determinants, inverse residuals, source preservation, singular behavior, small scales, maximum order and report provenance.

The browser math tests cover arithmetic, inverse residuals, row scaling, singular adjugates, malformed input, mismatched dimensions and non-finite outputs. Responsive browser checks exercise both themes, matrix calculations, downloads, gallery controls and local paths.

The verification report distinguishes executed checks from platforms and interfaces that still require separate testing. Saved results represent the included inputs and compiler environment.

| Recorded input | Expected observation |
| --- | --- |
| [[4,7],[2,6]] | det(A) = 10 and a finite inverse |
| [[0,2],[3,4]] | A row swap produces determinant -6 |
| [[1,2],[2,4]] | No inverse or unique solution |
| diag(1e-12,2e-12) | Inverse entries 1e12 and 5e11 |
| diag(1,2,3,0) | Singular 4x4 adjugate has bottom-right value 6 |
| diag(1,2,...,10) | Regression determinant 3,628,800 |

## Engineering Quality Signals

The maintained build keeps the assignment recognizable while making its behavior easier to review: one source file contains the native workflow, CMake and Visual Studio entry points are both retained, and the replay tool exercises the same menus a person would use.

Quality checks cover both expected results and failure paths. The suite checks pivoting, singular matrices, extreme scales, malformed tokens, non-finite values, oversized dimensions, report provenance and preservation of the working matrix. The browser tests independently cover calculations, downloads, themes, navigation and gallery controls.

Evidence is stored beside the code instead of being described only in prose. A recruiter can open a transcript, compare it with a 1440 × 900 figure, inspect the focused JSON record and rerun the case from data/demo_cases.json.

| Signal | What it demonstrates | Evidence |
| --- | --- | --- |
| Reproducible build | Fresh native executable and CTest target | CMakeLists.txt · build.ps1 |
| Numerical regression | Known values, residuals, pivots, ranges and singular behavior | tests/matrix_regression.cpp |
| Recorded behavior | 24 normal menu flows plus four defensive input checks | data/demo_cases.json · examples/transcripts |
| Reviewable presentation | Shared captions, source links and figure dimensions | README.md · index.html · assets/figures/ |

## Folder Structure

~~~text
MatrixCalculator_PF/
  src/                  Maintained native C++ source
  tests/                Native and browser math checks
  data/                 Native replay cases
  examples/             Transcripts and saved text output
  tools/                Replay, figure build and browser verification
  assets/               Website CSS, scripts and 24 figure pairs
  docs/                 Methods, reports and historical archive
  CMakeLists.txt        Portable build and CTest entry point
  MatrixCalculator_PF.sln
  MatrixCalculator_PF.vcxproj
  index.html
  README.md
~~~

## Limitations

The supported order is 1 to 10 and all inputs are square. Multiplication uses two matrices of the same order. The solver reports whether a unique solution exists; it does not classify inconsistent systems separately from systems with infinitely many solutions.

Calculations use floating-point arithmetic. Row scaling improves pivot selection but does not guarantee accuracy for ill-conditioned input. Very large values can overflow, tiny determinants can underflow, and a tolerance can classify nearly dependent rows as rank deficient. Both implementations track determinant scale factors as a fraction and binary exponent; the browser uses JavaScript Number.

Symmetry and identity checks use the configured absolute 1e-9 tolerance. A very wide 10x10 console output may require a wider terminal. Matrix data remains in the local process or browser. The report is a text snapshot of the latest result, rather than a persistent database of the complete session.
## Interactive Browser

Open [index.html](index.html) directly or use the HTTP preview. The browser accepts new inputs, reports errors inline and exports a JSON result. Ctrl + Enter calculates while editing. The saved-case viewer displays native evidence separately.

The gallery has search, filters, zoom, Previous/Next, arrow-key navigation and swipe. Light/dark choice is saved locally. Wide tables and code scroll horizontally.

## Source and Downloads

| File | Purpose |
| --- | --- |
| [Native source](src/matrix_calculator.cpp) | Maintained 19-option console program |
| [CMake build](CMakeLists.txt) | Build and CTest targets |
| [Visual Studio solution](MatrixCalculator_PF.sln) | IDE project |
| [Architecture notes](docs/architecture.md) | Layer boundaries and evidence flow |
| [Implementation](docs/implementation.md) | Methods, tolerance and result lifecycle |
| [Recorded inputs](data/demo_cases.json) | Reproducible menu cases |
| [Replay script](tools/collect_results.py) | Executes and checks native cases |
| [Portfolio builder](tools/build_portfolio.py) | Regenerates figures, README and webpage |
| [Native report](docs/reports/native_results.json) | Actual stdout excerpts and metadata |
| [Saved text result](examples/saved_result.txt) | Actual result.txt output |
| [Verification](docs/reports/verification.md) | Executed checks and scope |
| [Archive index](docs/archive/README.md) | Historical source and earlier illustrations |

## Figure Index

1. [Labeled matrix display](#01_display_v2)
2. [Symmetric matrix check](#02_symmetric_v2)
3. [Asymmetric matrix check](#03_asymmetric_v2)
4. [Identity matrix check](#04_identity_v2)
5. [2x2 determinant](#05_determinant_2_v2)
6. [3x3 determinant](#06_determinant_3_v2)
7. [2x2 inverse](#07_inverse_v2)
8. [Singular inverse rejection](#08_singular_inverse_v2)
9. [Unique linear-system solution](#09_solve_v2)
10. [Nonunique system rejection](#10_singular_solve_v2)
11. [Matrix transpose](#11_transpose_v2)
12. [Matrix addition](#12_addition_v2)
13. [Matrix subtraction](#13_subtraction_v2)
14. [Matrix multiplication](#14_multiplication_v2)
15. [Scalar multiplication](#15_scalar_v2)
16. [Matrix trace](#16_trace_v2)
17. [Rank of dependent rows](#17_rank_v2)
18. [3x3 adjugate](#18_adjugate_v2)
19. [Singular 4x4 adjugate](#19_singular_adjugate_v2)
20. [Small-scale inverse](#20_small_scale_v3)
21. [Zero first pivot](#21_pivot_swap_v2)
22. [Saved result provenance](#22_saved_history_v3)
23. [Finite-value validation](#23_validation_v2)
24. [Native regression report](#24_regression_v2)

## Screenshots

Each frame uses actual native output or its regression report. Full images are 1440 × 900 and previews are 960 × 600.

<a id="01_display_v2"></a>

### Labeled matrix display

<p align="left"><a href="assets/figures/full/01_display_v2.png"><img src="assets/figures/preview/01_display_v2.png" width="960" height="600" alt="Labeled matrix display"></a></p>

The native calculator displays a 2x2 working matrix with labeled rows and columns.

[Native transcript](examples/transcripts/display.txt)

<a id="02_symmetric_v2"></a>

### Symmetric matrix check

<p align="left"><a href="assets/figures/full/02_symmetric_v2.png"><img src="assets/figures/preview/02_symmetric_v2.png" width="960" height="600" alt="Symmetric matrix check"></a></p>

The two off-diagonal entries match, so the pairwise symmetry check succeeds.

[Native transcript](examples/transcripts/symmetric.txt)

<a id="03_asymmetric_v2"></a>

### Asymmetric matrix check

<p align="left"><a href="assets/figures/full/03_asymmetric_v2.png"><img src="assets/figures/preview/03_asymmetric_v2.png" width="960" height="600" alt="Asymmetric matrix check"></a></p>

The original input is compared with its transpose and correctly classified as asymmetric.

[Native transcript](examples/transcripts/asymmetric.txt)

<a id="04_identity_v2"></a>

### Identity matrix check

<p align="left"><a href="assets/figures/full/04_identity_v2.png"><img src="assets/figures/preview/04_identity_v2.png" width="960" height="600" alt="Identity matrix check"></a></p>

A 3x3 identity input passes the diagonal and off-diagonal checks.

[Native transcript](examples/transcripts/identity.txt)

<a id="05_determinant_2_v2"></a>

### 2x2 determinant

<p align="left"><a href="assets/figures/full/05_determinant_2_v2.png"><img src="assets/figures/preview/05_determinant_2_v2.png" width="960" height="600" alt="2x2 determinant"></a></p>

Gaussian elimination returns determinant 10 for the bundled 2x2 example.

[Native transcript](examples/transcripts/determinant_2.txt)

<a id="06_determinant_3_v2"></a>

### 3x3 determinant

<p align="left"><a href="assets/figures/full/06_determinant_3_v2.png"><img src="assets/figures/preview/06_determinant_3_v2.png" width="960" height="600" alt="3x3 determinant"></a></p>

The classic 3x3 example has determinant 1, providing a known-value calculation.

[Native transcript](examples/transcripts/determinant_3.txt)

<a id="07_inverse_v2"></a>

### 2x2 inverse

<p align="left"><a href="assets/figures/full/07_inverse_v2.png"><img src="assets/figures/preview/07_inverse_v2.png" width="960" height="600" alt="2x2 inverse"></a></p>

The native inverse has entries 0.6, -0.7, -0.2 and 0.4.

[Native transcript](examples/transcripts/inverse.txt)

<a id="08_singular_inverse_v2"></a>

### Singular inverse rejection

<p align="left"><a href="assets/figures/full/08_singular_inverse_v2.png"><img src="assets/figures/preview/08_singular_inverse_v2.png" width="960" height="600" alt="Singular inverse rejection"></a></p>

Dependent rows are detected, and the calculator reports that an inverse does not exist.

[Native transcript](examples/transcripts/singular_inverse.txt)

<a id="09_solve_v2"></a>

### Unique linear-system solution

<p align="left"><a href="assets/figures/full/09_solve_v2.png"><img src="assets/figures/preview/09_solve_v2.png" width="960" height="600" alt="Unique linear-system solution"></a></p>

For A x = [18,14], elimination returns the known solution x = [1,2].

[Native transcript](examples/transcripts/solve.txt)

<a id="10_singular_solve_v2"></a>

### Nonunique system rejection

<p align="left"><a href="assets/figures/full/10_singular_solve_v2.png"><img src="assets/figures/preview/10_singular_solve_v2.png" width="960" height="600" alt="Nonunique system rejection"></a></p>

A dependent system is rejected as having no unique solution.

[Native transcript](examples/transcripts/singular_solve.txt)

<a id="11_transpose_v2"></a>

### Matrix transpose

<p align="left"><a href="assets/figures/full/11_transpose_v2.png"><img src="assets/figures/preview/11_transpose_v2.png" width="960" height="600" alt="Matrix transpose"></a></p>

Rows become columns while the stored working matrix stays available for later operations.

[Native transcript](examples/transcripts/transpose.txt)

<a id="12_addition_v2"></a>

### Matrix addition

<p align="left"><a href="assets/figures/full/12_addition_v2.png"><img src="assets/figures/preview/12_addition_v2.png" width="960" height="600" alt="Matrix addition"></a></p>

Adding [[1,2],[3,4]] produces [[5,9],[5,10]].

[Native transcript](examples/transcripts/addition.txt)

<a id="13_subtraction_v2"></a>

### Matrix subtraction

<p align="left"><a href="assets/figures/full/13_subtraction_v2.png"><img src="assets/figures/preview/13_subtraction_v2.png" width="960" height="600" alt="Matrix subtraction"></a></p>

Subtracting [[1,2],[3,4]] produces [[3,5],[-1,2]].

[Native transcript](examples/transcripts/subtraction.txt)

<a id="14_multiplication_v2"></a>

### Matrix multiplication

<p align="left"><a href="assets/figures/full/14_multiplication_v2.png"><img src="assets/figures/preview/14_multiplication_v2.png" width="960" height="600" alt="Matrix multiplication"></a></p>

The triple-loop product produces [[25,36],[20,28]] for the two recorded inputs.

[Native transcript](examples/transcripts/multiplication.txt)

<a id="15_scalar_v2"></a>

### Scalar multiplication

<p align="left"><a href="assets/figures/full/15_scalar_v2.png"><img src="assets/figures/preview/15_scalar_v2.png" width="960" height="600" alt="Scalar multiplication"></a></p>

A negative scalar multiplies every cell without replacing the working matrix.

[Native transcript](examples/transcripts/scalar.txt)

<a id="16_trace_v2"></a>

### Matrix trace

<p align="left"><a href="assets/figures/full/16_trace_v2.png"><img src="assets/figures/preview/16_trace_v2.png" width="960" height="600" alt="Matrix trace"></a></p>

The trace is the sum of the two diagonal entries, 4 + 6.

[Native transcript](examples/transcripts/trace.txt)

<a id="17_rank_v2"></a>

### Rank of dependent rows

<p align="left"><a href="assets/figures/full/17_rank_v2.png"><img src="assets/figures/preview/17_rank_v2.png" width="960" height="600" alt="Rank of dependent rows"></a></p>

Row reduction finds one independent row in this 3x3 matrix.

[Native transcript](examples/transcripts/rank.txt)

<a id="18_adjugate_v2"></a>

### 3x3 adjugate

<p align="left"><a href="assets/figures/full/18_adjugate_v2.png"><img src="assets/figures/preview/18_adjugate_v2.png" width="960" height="600" alt="3x3 adjugate"></a></p>

The direct cofactor implementation transposes the cofactor matrix to form the adjugate.

[Native transcript](examples/transcripts/adjugate.txt)

<a id="19_singular_adjugate_v2"></a>

### Singular 4x4 adjugate

<p align="left"><a href="assets/figures/full/19_singular_adjugate_v2.png"><img src="assets/figures/preview/19_singular_adjugate_v2.png" width="960" height="600" alt="Singular 4x4 adjugate"></a></p>

The larger singular case has a valid adjugate with bottom-right value 6.

[Native transcript](examples/transcripts/singular_adjugate.txt)

<a id="20_small_scale_v3"></a>

### Small-scale inverse

<p align="left"><a href="assets/figures/full/20_small_scale_v3.png"><img src="assets/figures/preview/20_small_scale_v3.png" width="960" height="600" alt="Small-scale inverse"></a></p>

Row equilibration preserves the inverse of a tiny, well-scaled diagonal matrix.

[Native transcript](examples/transcripts/small_scale.txt)

<a id="21_pivot_swap_v2"></a>

### Zero first pivot

<p align="left"><a href="assets/figures/full/21_pivot_swap_v2.png"><img src="assets/figures/preview/21_pivot_swap_v2.png" width="960" height="600" alt="Zero first pivot"></a></p>

Partial pivoting swaps rows and preserves the determinant sign.

[Native transcript](examples/transcripts/pivot_swap.txt)

<a id="22_saved_history_v3"></a>

### Saved result provenance

<p align="left"><a href="assets/figures/full/22_saved_history_v3.png"><img src="assets/figures/preview/22_saved_history_v3.png" width="960" height="600" alt="Saved result provenance"></a></p>

A saved report keeps the calculation name even after the About screen is opened.

[Native transcript](examples/transcripts/saved_history.txt)

<a id="23_validation_v2"></a>

### Finite-value validation

<p align="left"><a href="assets/figures/full/23_validation_v2.png"><img src="assets/figures/preview/23_validation_v2.png" width="960" height="600" alt="Finite-value validation"></a></p>

NaN, infinity and overflowing numeric tokens are rejected before a valid scalar is accepted.

[Native transcript](examples/transcripts/validation.txt)

<a id="24_regression_v2"></a>

### Native regression report

<p align="left"><a href="assets/figures/full/24_regression_v2.png"><img src="assets/figures/preview/24_regression_v2.png" width="960" height="600" alt="Native regression report"></a></p>

The regression executable checks numerical values, residuals, boundary sizes and save provenance.

[Native transcript](examples/transcripts/regression.txt)

## About the Author

Mohammad Rohaan · BS Computer Science, FAST-NUCES · 22I-2327.

This project demonstrates array-based C++, input handling, numerical methods, reproducible builds, regression checks and an interactive technical presentation. The historical source and earlier generated illustrations remain available in the archive.

[Author profile](https://github.com/rohaan2802)
