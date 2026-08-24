# MatrixCalculator_PF

Console **square-matrix calculator** (PF Project 2). Storage `const int n = 10` (up to 10×10). **Determinant / adjoint / inverse / Ax=b** are hard-coded for **2×2 and 3×3**. Control flow uses **`goto`**.

**Source:** `Source.cpp` · [rohaan2802](https://github.com/rohaan2802)

---

## Table of contents

1. [Menu](#menu)
2. [Math (as implemented)](#math-as-implemented)
3. [Session flow](#session-flow)
4. [Build](#build)
5. [Limitations](#limitations)

---

## Menu

After size + entries:

1. **Display**  
2. **Symmetric?** — equal to transpose  
3. **Identity?** — diagonal 1 / off-diagonal 0 (as coded)  
4. **Determinant** → global `deter`  
5. **Adjoint** (2×2, 3×3 cofactor layout)  
6. **Inverse** — adjoint / det if `deter ≠ 0`  
7. **Solve linear system** — prompt constant vector; inverse-based for 2×2 / 3×3  
8. **Terminate**

After each op: **Y** same matrix · **N** new matrix · **0** exit (`goto` labels).

Functions: `Input_Matrix`, `Output_Matrix`, `Symetric_Matrix`, `Identity_Matrix`, `Determinant_Matrix`, `Adjoint_Matrix`, `Inverse_Matrix`, `Matrix_solution`. Formatting: `iomanip` `setw` / `left`.

---

## Math (as implemented)

- **2×2 det:** `ad − bc`.  
- **3×3 det:** expansion (standard cofactor / Sarrus-style as in source).  
- **Adjoint:** transpose of cofactor matrix for those sizes.  
- **Inverse:** `(1/det) * adj`. Many paths use **integer** prints — fractional inverses may truncate.  
- **Solve:** `x = A⁻¹ b` for n=2 or 3.

For n>3, det/adjoint/inverse branches may no-op or only handle the coded sizes — **use 2 or 3 for the full menu**.

---

## Session flow

1. Enter `mat_size` (practical: 2 or 3).  
2. Enter `n²` integers.  
3. Pick 1–8.  
4. Option 7: enter constants for `b`.  
5. Y / N / 0.

IDE: `PF_Project_2.sln`.

---

## Build

```bash
g++ -o MatrixCalculator Source.cpp
./MatrixCalculator
```

---

## Limitations

- No general Gaussian elimination.  
- `goto` instead of loops.  
- Integer-heavy I/O.  
- Identity/symmetry checks follow the exact nested loops in source (off-by-one worth a viva read-through).

**Extend:** recursive det or GE for n≤10; `double` throughout; drop `goto`.

---

## Author

**rohaan2802** · [https://github.com/rohaan2802](https://github.com/rohaan2802)
