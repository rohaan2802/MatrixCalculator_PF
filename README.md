# MatrixCalculator_PF

Professional **square-matrix console calculator** for Programming Fundamentals — rebuilt from the original course project with every menu feature preserved, numerical bugs fixed, and production-minded extras added.

**Author:** Mohammad Rohaan · **22I-2327**  
**Capacity:** `const int N = 10` (square matrices up to 10×10)  
**Source:** [`Source.cpp`](Source.cpp) · solution [`PF_Project_2.sln`](PF_Project_2.sln)  
**Repository:** [github.com/rohaan2802/MatrixCalculator_PF](https://github.com/rohaan2802/MatrixCalculator_PF)

---

## Visual walkthrough

Open the [Screenshot gallery](#screenshot-gallery) below for an end-to-end tour (~30 screenshots): banner and menu, validated size entry, matrix input, course operations 1–7, extras (addition through history), singular solve, 4×4 GE determinant, save/`result.txt`, continue Y vs N, and a complete feature map.

**About colors:** Gallery panels are stylized for README readability. The live Windows build uses the same layout and **real console colors** via `SetConsoleTextAttribute` (green titles, cyan prompts, red errors, bright matrix values). Run `PF_Project_2.exe` in a normal Command Prompt / Windows Terminal to see them.

Detailed I/O in the live app includes: row-by-row entry with per-cell confirmation, matrix preview after input, labeled `r#`/`c#` output grids, and a clear Y / N / 0 continue prompt.

Regenerate images anytime:

```bash
pip install Pillow
python docs/generate_screenshots.py
```

---

## Ready to run

1. Open [`PF_Project_2.sln`](PF_Project_2.sln) in Visual Studio 2022.
2. Set configuration to **Release** and platform to **x64**.
3. Build the solution (`Ctrl+Shift+B` or **Build → Build Solution**).
4. Run the executable from the project folder: `x64\Release\PF_Project_2.exe`  
   (Command Prompt / Windows Terminal — so console colors show correctly.)

MSBuild one-liner (same config):

```bat
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" PF_Project_2.sln /p:Configuration=Release /p:Platform=x64
```

---

## Requirements checklist

| # | Requirement | Status |
|---|-------------|--------|
| 1 | Display matrix A (`setw` / `setprecision`) | Done |
| 2 | Symmetric check (full pairwise for all `n`) | Done |
| 3 | Identity check (initialized flag; diag 1 / off-diag 0) | Done |
| 4 | Determinant (GE + partial pivoting; `lastDet`) | Done |
| 5 | Adjoint / adjugate (cofactors `n≤3`; `Inv·det` when invertible) | Done |
| 6 | Inverse (GE on `[A \| I]`; refuse singular) | Done |
| 7 | Solve `Ax = b` (correct **n×1** `b`; GE solve) | Done |
| 8 | Exit (clean loop; no `goto`) | Done |
| 9 | Transpose | Done |
| 10 | Addition `A + B` | Done |
| 11 | Subtraction `A − B` | Done |
| 12 | Multiplication `A · B` | Done |
| 13 | Scalar `k · A` | Done |
| 14 | Trace | Done |
| 15 | Rank (row-reduced GE) | Done |
| 16 | Sample presets | Done |
| 17 | Banner / about | Done |
| 18 | Save `result.txt` | Done |
| 19 | History (last op + last result) | Done |
| — | GE for all square sizes **n ≤ 10** | Done |
| — | **`double`** throughout (det / inv / solve) | Done |
| — | Working matrix **A preserved** (ops use temps) | Done |
| — | **30** README screenshots in `docs/screenshots/` | Done |

**Session control:** after each op — **Y** same matrix · **N** new size/entries · **0** exit. Nested `while` loops (no broken `goto`).

---

## Features

| # | Feature | Notes |
|---|---------|--------|
| 1 | Display matrix A | `setw` / `setprecision` double grid |
| 2 | Symmetric check | Full `A[i][j] == A[j][i]` for all `n`; prints transpose |
| 3 | Identity check | Flag **initialized**; diagonal 1 / off-diagonal 0 |
| 4 | Determinant | Gaussian elimination + partial pivoting; updates `lastDet` |
| 5 | Adjoint / adjugate | Cofactors for `n ≤ 3`; `A⁻¹·det` for invertible `n ≤ 10` |
| 6 | Inverse | GE on augmented `[A \| I]`; refuses singular matrices |
| 7 | Solve `Ax = b` | Correct **n×1** `b` input; GE solve |
| 8 | Exit | Clean loop exit (no `goto`) |
| 9 | Transpose | Print / store last result |
| 10 | Addition `A + B` | Prompts square `B` |
| 11 | Subtraction `A − B` | Prompts square `B` |
| 12 | Multiplication `A · B` | Same-size square multiply |
| 13 | Scalar `k · A` | Double scalar |
| 14 | Trace | Sum of diagonal |
| 15 | Rank | Row-reduced GE |
| 16 | Sample presets | I, Hilbert-ish, singular, symmetric, classic 2×2 / 3×3 |
| 17 | Banner / about | Pretty professional banner |
| 18 | Save `result.txt` | Optional dump of last result |
| 19 | History | Last operation name (+ last numeric result) |

**Session control:** after each op — **Y** same matrix · **N** new size/entries · **0** exit. Implemented with nested `while` loops (no broken `goto`).

**Invariant:** working matrix `A` is never overwritten; all mutating algorithms run on temporary copies.

---

## Screenshot gallery

### 01 — Main banner + full detailed menu listing 1–19
![01 main banner menu](docs/screenshots/01-main-banner-menu.png)

### 02 — Size input (1..10 validation)
![02 size input](docs/screenshots/02-size-input.png)

### 03 — Matrix entry
![03 matrix entry](docs/screenshots/03-matrix-entry.png)

### 04 — Display matrix
![04 display matrix](docs/screenshots/04-display-matrix.png)

### 05 — Symmetric check (yes)
![05 symmetric yes](docs/screenshots/05-symmetric-yes.png)

### 06 — Identity check
![06 identity check](docs/screenshots/06-identity-check.png)

### 07 — Determinant 2×2
![07 determinant 2x2](docs/screenshots/07-determinant-2x2.png)

### 08 — Determinant 3×3
![08 determinant 3x3](docs/screenshots/08-determinant-3x3.png)

### 09 — Adjoint
![09 adjoint](docs/screenshots/09-adjoint.png)

### 10 — Inverse (double)
![10 inverse double](docs/screenshots/10-inverse-double.png)

### 11 — Solve Ax = b
![11 solve axb](docs/screenshots/11-solve-axb.png)

### 12 — Transpose
![12 transpose](docs/screenshots/12-transpose.png)

### 13 — Multiply
![13 multiply](docs/screenshots/13-multiply.png)

### 14 — Rank & trace
![14 rank trace](docs/screenshots/14-rank-trace.png)

### 15 — Singular: no inverse
![15 singular no inverse](docs/screenshots/15-singular-no-inverse.png)

### 16 — Continue prompt
![16 continue prompt](docs/screenshots/16-continue-prompt.png)

### 17 — Sample presets
![17 sample presets](docs/screenshots/17-sample-presets.png)

### 18 — Full session demo
![18 full session demo](docs/screenshots/18-full-session-demo.png)

### 19 — Matrix addition
![19 matrix addition](docs/screenshots/19-matrix-addition.png)

### 20 — Matrix subtraction
![20 matrix subtraction](docs/screenshots/20-matrix-subtraction.png)

### 21 — Scalar multiplication
![21 scalar multiply](docs/screenshots/21-scalar-multiply.png)

### 22 — Asymmetric check
![22 asymmetric check](docs/screenshots/22-asymmetric-check.png)

### 23 — Determinant 4×4 (GE)
![23 det 4x4 ge](docs/screenshots/23-det-4x4-ge.png)

### 24 — Solve singular system
![24 solve singular](docs/screenshots/24-solve-singular.png)

### 25 — Save result.txt
![25 save result file](docs/screenshots/25-save-result-file.png)

### 26 — Operation history
![26 operation history](docs/screenshots/26-operation-history.png)

### 27 — Invalid size error
![27 invalid size error](docs/screenshots/27-invalid-size-error.png)

### 28 — New matrix prompt (N)
![28 new matrix prompt](docs/screenshots/28-new-matrix-prompt.png)

### 29 — Trace-only demo
![29 trace only demo](docs/screenshots/29-trace-only-demo.png)

### 30 — Complete feature map
![30 complete feature map](docs/screenshots/30-complete-feature-map.png)

Full menu map plus numerical engine note: **GE = Gaussian Elimination** (with partial pivoting), supported for all square sizes **n = 1 … 10** (`n ≤ 10`), used for determinant, inverse, `Ax = b`, and rank.

---

## Architecture / algorithms

Single translation unit [`Source.cpp`](Source.cpp) (PF style). Helpers live in the same file as free functions / shared session state.

| Piece | Role |
|-------|------|
| `determinantGE` | Partial-pivoting GE on a **copy**; writes `lastDet` |
| `inverseGE` | Row-reduce `[A \| I]` → `A⁻¹` |
| `solveGE` | Row-reduce `[A \| b]` → `x` |
| `rankGE` | Count nonzero pivots after reduction |
| `adjointCofactors` | Course-style cofactor/adjugate for `n ≤ 3` |
| `adjointViaInverse` | `adj(A) = A⁻¹ · det(A)` when invertible |
| Session state | `lastDet`, `lastOpName`, last matrix/vector result, optional `result.txt` |

**Gaussian elimination (preferred for all `n ≤ 10`):** for each column, select the largest-magnitude pivot below the diagonal, swap rows, scale, and eliminate. Determinant is the signed product of pivots. Inverse and solve use the same pivot strategy on an augmented matrix.

**Adjoint:** for small `n`, cofactors match the course worksheet. For larger invertible matrices, `Inv * det` yields the adjugate without building every minor explicitly.

Includes: `<iostream> <iomanip> <cmath> <fstream> <string>`. No C++20 requirement.

---

## Menu map

```
┌─ COURSE ──────────────────────────────────────┐
│ 1 Display · 2 Symmetric · 3 Identity          │
│ 4 Det · 5 Adjoint · 6 Inverse · 7 Ax=b · 8 Exit│
├─ EXTRAS ──────────────────────────────────────┤
│ 9 Transpose · 10 + · 11 − · 12 * · 13 scalar  │
│ 14 Trace · 15 Rank · 16 Samples · 17 Banner   │
│ 18 Save result.txt · 19 History               │
└───────────────────────────────────────────────┘
         Y / N / 0  after each operation
```

---

## Build & run

### Visual Studio 2022 (recommended)

1. Open `PF_Project_2.sln`
2. Configuration: **Release** · Platform: **x64**
3. Build Solution (`Ctrl+Shift+B`)
4. Run `x64\Release\PF_Project_2.exe`

MSBuild (Community 2022 full path):

```bat
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" PF_Project_2.sln /p:Configuration=Release /p:Platform=x64
```

### g++

```bash
g++ -std=c++17 -O2 -o MatrixCalc Source.cpp
./MatrixCalc
```

---

## Test cases (worked examples)

| Case | Input | Expected |
|------|--------|----------|
| Det 2×2 | `[[2,1],[5,3]]` | `det = 1` |
| Inv 2×2 | same | `[[3,-1],[-5,2]]` |
| Adj 2×2 | same | `[[3,-1],[-5,2]]` |
| Solve | `A` above, `b = [1,1]` | `x = [2, -3]` |
| Det 3×3 | `[[1,2,3],[0,1,4],[5,6,0]]` | `det = 1` |
| Trace | same 3×3 | `trace = 2` |
| Rank full | same 3×3 | `rank = 3` |
| Singular | `[[1,2,3],[2,4,6],[1,1,1]]` | `det = 0`, inverse refused, `rank = 2` |
| Symmetric | `[[2,1,0],[1,3,4],[0,4,5]]` | reports **SYMMETRIC** |
| Identity | `I₃` | reports identity |
| Size guard | `12` or `0` | error; re-prompt until `1..10` |
| Multiply | `[[1,2],[3,4]] * [[2,0],[1,2]]` | `[[4,4],[10,8]]` |

---

## Comparison: old bugs fixed

| Issue in original | Fix in this rewrite |
|-------------------|---------------------|
| Menu text said “1-7” while case 8 existed | Menu clearly lists **1–19**; course DNA 1–8 intact |
| `goto New / cont / terminate` (fragile) | Nested `while` loops + flags |
| No size validation | Size must be **1..10** |
| Identity `bool a` uninitialized | `isId = true` then disprove |
| Det / inv / solve only for 2×2 & 3×3 | GE for **all n ≤ 10** |
| Integer matrices + integer division on inverse | **`double` throughout**; formatted output |
| Adjoint / inverse / solve **overwrote** `A` | Ops use temps; `A` preserved |
| Shared `deter` vs local shadowing | Shared `lastDet` updated by GE |
| Symmetric only hardcoded for n=2,3 | Full pairwise compare for all `n` |
| `Ax=b` wrong loop bounds for `b` | Correct **n×1** vector input |
| 3×3 adjoint print typo (`Matrix[1][2]` twice) | Correct row/column print of temp adjugate |
| No extras | Transpose, ±, *, scalar, trace, rank, samples, save, history |

---

## Author

**Mohammad Rohaan** · Roll **22I-2327** · Programming Fundamentals square-matrix project (professional rebuild).
