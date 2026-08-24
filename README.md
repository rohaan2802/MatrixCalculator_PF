# MatrixCalculator_PF

Programming Fundamentals **square-matrix calculator** (`PF_Project_2`). Storage is a fixed `int Matrix[n][n]` with `const int n = 10` (capacity 10×10). **Determinant, adjoint, inverse, and Ax = b** are implemented only for **2×2 and 3×3**. Session control uses **`goto`** labels (`New`, `A`, `cont`, `terminate`). Arithmetic is integer-heavy: inverse entries are printed with **integer division**, so fractions truncate toward zero.

**Course / author:** Programming Fundamentals · Mohammad Rohaan · roll **22I-2327**  
**Source on GitHub:** `Source.cpp` · solution `PF_Project_2.sln`  
[https://github.com/rohaan2802](https://github.com/rohaan2802)

---

## Table of contents

1. [What it is](#what-it-is)
2. [Architecture](#architecture)
3. [File-by-file](#file-by-file)
4. [Menu 1–8](#menu-18)
5. [Algorithms](#algorithms)
6. [I/O and formatting](#io-and-formatting)
7. [Goto flow](#goto-flow)
8. [Build and run](#build-and-run)
9. [Known limitations](#known-limitations)
10. [Author](#author)

---

## What it is

`main` prompts `ENTER THE SIZE OF THE SQUARE MATRIX:`, reads `matrix_size`, then `Input_Matrix` fills `mat_size²` integers (1-based labels `Matrix[i+1, j+1]`). A numbered menu (prompt text says “1-7” but **case 8** exists) dispatches to helpers. After each operation the program asks **Y** (same matrix), **N** (new size and entries), or **0** (jump to `terminate`). Globals: `double deter`, `char ch`, `const int n = 10`. A commented `#define n 10` notes that a macro and `const` must not both define `n`.

---

## Architecture

Forward declarations, then `main`, then eight function definitions. Matrices are **row-major `int[10][10]`**. Helpers take `(int Matrix[n][n], int mat_size)`. Several operations **overwrite `Matrix` in place** (adjoint / inverse / solve), so option 1 after option 5 no longer shows the original entries.

| Function | Role |
|----------|------|
| `Input_Matrix` | Nested `cin` into `Matrix[i][j]` |
| `Output_Matrix` | `left << setw(5)` grid |
| `Symetric_Matrix` | Print original + transpose; compare off-diagonals for n = 2 or 3 |
| `Identity_Matrix` | Diagonal 1 / off-diagonal 0 scan, then print + verdict |
| `Determinant_Matrix` | 2×2 `ad−bc`; 3×3 cofactor expansion into **local** `double deter` |
| `Adjoint_Matrix` | 2×2 / 3×3 adjugate; mutates `Matrix`; 3×3 print has a last-row typo |
| `Inverse_Matrix` | Recomputes det + adj; prints `Matrix[i][j] / deter` if `deter != 0` |
| `Matrix_solution` | Prompts a constant vector; `x = A⁻¹ b` style multiply for n = 2 or 3 |

Includes: `<iostream>`, `<iomanip>`. `using namespace std;`

---

## File-by-file

| Path | Role |
|------|------|
| `Source.cpp` | All logic (GitHub name; local extract `MatrixCalc.cpp`) |
| `PF_Project_2.sln` | Visual Studio solution |
| `PF_Project_2.vcxproj` / `.filters` | Project files |
| `.gitattributes` / `.gitignore` | Git metadata |

No header split, no Gaussian-elimination module, no tests.

---

## Menu 1–8

Printed after input (wording from source):

| Option | Label in menu | Function |
|--------|----------------|----------|
| 1 | Display a matrix | `Output_Matrix` |
| 2 | Symmetric or not | `Symetric_Matrix` (name spelled **Symetric**) |
| 3 | Identity or not | `Identity_Matrix` |
| 4 | Determinant | `Determinant_Matrix` |
| 5 | Adjoint | `Adjoint_Matrix` |
| 6 | Inverse | `Inverse_Matrix` |
| 7 | Solution of linear equations | `Matrix_solution` |
| 8 | Terminate | `goto terminate` |

Invalid `option` values fall out of the `switch` with no message, then still hit the Y/N/0 prompt.

---

## Algorithms

### Symmetric (`Symetric_Matrix`)

Always prints the original and the transpose `Matrix[j][i]`. Equality is **not** a full nested compare:

- **n = 2:** `Matrix[0][1] == Matrix[1][0]` only (diagonal ignored, correct for 2×2 symmetry).
- **n = 3:** `(0,1)=(1,0)` and `(0,2)=(2,0)` and `(1,2)=(2,1)`.
- **Other sizes:** transpose is printed; **no** “IS / IS NOT SYMMETRIC” line.

### Identity (`Identity_Matrix`)

Nested loops: if `i == j && Matrix[i][j] != 1` or `i != j && Matrix[i][j] != 0`, set `bool a = 1`. Then `if (a == 0)` claims identity. **`a` is never initialized to 0**, so a true identity matrix leaves `a` uninitialized (undefined behavior). The success message mentions only the principal diagonal.

### Determinant (`Determinant_Matrix`)

**2×2:** `deter = (M00 * M11) − (M01 * M10)`.

**3×3:** minors

- `x = M11*M22 − M21*M12`
- `y = M10*M22 − M20*M12`
- `z = M10*M21 − M20*M11`
- `deter = M00*x − M01*y + M02*z`

This is standard first-row expansion (`y` already omits the usual extra minus on the middle cofactor because the formula subtracts `M01*y`). For `mat_size` other than 2 or 3, **local** `double deter` is uninitialized when printed. This local **shadows** the global `double deter`; option 4 does **not** fill the global used elsewhere.

### Adjoint (`Adjoint_Matrix`)

**2×2:** swap diagonal, negate off-diagonal, **write back into `Matrix`**, print two rows.

**3×3:** cofactor matrix `a[3][3]`, then **transpose into `Matrix`**. Display always prints three rows of three; the last printed line uses `Matrix[1][2]` instead of `Matrix[2][2]` (`Matrix[2][0]`, `Matrix[2][1]`, `Matrix[1][2]`). For n ≠ 2 and n ≠ 3 the 3×3 print still runs and reads unwritten cells.

### Inverse (`Inverse_Matrix`)

Recomputes **`int deter`** (truncates the 3×3 formula). Builds the same adjoint **in place**. If `deter != 0`, prints `Matrix[i][j] / deter` as **ints**. Example: det = 2 and adj entry 1 prints `0`. If det = 0: `Determinat is zero so inverse cannot be find`.

### Linear system (`Matrix_solution`)

Intended model: solve `A x = b` via `x = A⁻¹ b` after replacing `A` with adj(A).

**2×2:** `int arr[2][1]`. Input loops `i < mat_size - 1` (one outer pass) and `j < mat_size` (two `cin`s). That writes `arr[0][0]` and **`arr[0][1]`**, which is **outside** `arr[2][1]`. Display uses `j < mat_size - 1`. Solution line mixes `arr[0][0]`, `arr[0][1]`, `arr[1][0]`.

**3×3:** `int arr[3][1]`, outer `i < mat_size - 2` (one pass), inner `j < mat_size` (three writes into row 0, including out-of-range columns). Uses **global** `deter` for the 3×3 formula. Combine with integer `/ deter` on the adjoint.

There is **no** Gaussian elimination and **no** n = 4…10 solver.

---

## I/O and formatting

- Size: `cin >> matrix_size` (not clamped to `n`; `mat_size > 10` overruns the array).
- Entries: integers only (`int Matrix`).
- Display: `cout << left << setw(5)`.
- Continue prompt: `Press Y … N … 0` into `char ch`. Only `'Y'/'y'`, `'N'/'n'`, `'0'` are handled; other characters fall through to `system("pause"); return 0;`.

---

## Goto flow

```text
New:     allocate Matrix, read size
A:       Input_Matrix          (label A is never jumped to)
cont:    print menu, switch
         case 8 → goto terminate
terminate:  (label is **inside** the switch after case 8)
         print “The Program is terminated.... Thank You”
         then execution **leaves the switch** and still prints Y/N/0
Y/y → cont     N/n → New     0 → terminate (Thank You again, then Y/N/0 again)
```

So option 8 and press `0` do **not** `return`; they reprint the thank-you line and the continue prompt. `system("pause")` runs only when `ch` is none of Y/N/0.

---

## Build and run

```bash
g++ -o MatrixCalculator Source.cpp
./MatrixCalculator
```

Open `PF_Project_2.sln` in Visual Studio. Practical sizes for the full menu: **2 or 3**. For n = 2, inverse of `[[1,0],[0,1]]` prints `1 0 / 0 1`. For n = 2, inverse of `[[1,2],[3,4]]` (det = −2) uses integer division on the adjoint.

Worked **2×2 inverse** as the code does it (option 6), matrix `A = [[1,2],[3,4]]`:

| Step | Value |
|------|--------|
| `deter` | `1*4 − 2*3 = −2` (`int`) |
| After in-place adj | `[[4, −2],[−3, 1]]` |
| Printed `adj / deter` | `4/(-2)=−2`, `(-2)/(-2)=1`, `(-3)/(-2)=1` (truncation), `1/(-2)=0` |

So the printed inverse is **not** the real `[[−2, 1],[1.5, −0.5]]`. Option 1 after this shows the **adjoint**, not `A`.

Worked **2×2 det** (option 4) uses a **local** `double deter`, so a following option 7 (3×3 path) still reads the **global** `deter` (often 0 if never written). Option 5 for 3×3 always prints three rows even when `mat_size != 3`.

Typical session:

1. Size `3`, nine integers.  
2. Option 1 — formatted grid.  
3. Option 4 — first-row expansion.  
4. Option 6 — inverse with `int` division; `Matrix` is now the adjugate.  
5. `Y` — menu again on the **mutated** array; `N` — label `New` (new size); `0` — thank-you then the same prompt again.

`system("pause")` is Windows-oriented (`pause` command). On MinGW it may still work; on other hosts it is a no-op or an error after a non-Y/N/0 character.

---

## Known limitations

| Item | Detail |
|------|--------|
| Sizes 4–10 | Input/display/identity scan work; det/adj/inverse/solve branches do not implement n > 3 |
| Integer truncation | Inverse and solve use `int / int` |
| In-place adjoint | Options 5–7 destroy the original `Matrix` |
| Uninitialized `bool a` | Identity true-path is undefined |
| Uninitialized local `deter` | Option 4 when n ∉ {2,3} |
| Global vs local `deter` | Option 4 does not update the global; 3×3 solve uses the global |
| Constant-vector loops | `mat_size-1` / `mat_size-2` and `arr[][1]` layout |
| 3×3 adjoint last cell | Prints `Matrix[1][2]` twice in spirit |
| `goto` | No structured loop; `terminate` inside `switch` |
| Menu text | Says 1–7 while 8 is implemented |
| No pivoting / GE | Cannot invert general n×n |

**If extending:** `double` throughout; recursive det or GE for n ≤ 10; copy-on-write so display still shows A; initialize `a = false`; move `terminate` to a real `return`.

---

## Author

**Mohammad Rohaan** · roll **22I-2327** · GitHub [rohaan2802](https://github.com/rohaan2802)
