#!/usr/bin/env python3
"""
Generate console-style dark-panel screenshots for MatrixCalculator_PF README.
Requires: Pillow  (pip install Pillow)
Output: docs/screenshots/01-...png through 30-...png
"""

from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

OUT = Path(__file__).resolve().parent / "screenshots"
OUT.mkdir(parents=True, exist_ok=True)

BG = (18, 22, 28)
PANEL = (28, 34, 44)
BORDER = (70, 130, 180)
TITLE = (120, 200, 255)
GREEN = (100, 220, 140)
YELLOW = (240, 200, 90)
WHITE = (220, 225, 230)
DIM = (140, 150, 165)
RED = (240, 110, 110)
PROMPT = (180, 220, 255)

W, H = 920, 560
PAD = 28
LINE_H = 18


def font(size=15, bold=False):
    candidates = [
        "C:/Windows/Fonts/consola.ttf",
        "C:/Windows/Fonts/lucon.ttf",
        "C:/Windows/Fonts/cour.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",
    ]
    for path in candidates:
        if Path(path).exists():
            return ImageFont.truetype(path, size)
    return ImageFont.load_default()


F = font(14)
F_TITLE = font(16)
F_SMALL = font(12)


def new_canvas(height=None):
    h = height or H
    img = Image.new("RGB", (W, h), BG)
    draw = ImageDraw.Draw(img)
    draw.rounded_rectangle(
        [16, 16, W - 16, h - 16],
        radius=12,
        fill=PANEL,
        outline=BORDER,
        width=2,
    )
    return img, draw


def draw_lines(draw, lines, y0=36):
    y = y0
    for item in lines:
        if isinstance(item, tuple):
            text, color = item
        else:
            text, color = item, WHITE
        draw.text((PAD + 12, y), text, fill=color, font=F)
        y += LINE_H
    return y


def save(img, name):
    path = OUT / name
    img.save(path, "PNG")
    print("wrote", path.name)


def shot(name, title, lines, height=None):
    img, draw = new_canvas(height)
    draw.text((PAD + 12, 26), title, fill=TITLE, font=F_TITLE)
    draw.line([(PAD + 8, 48), (W - PAD - 8, 48)], fill=BORDER, width=1)
    draw_lines(draw, lines, y0=58)
    save(img, name)


# ---- 01–30 screenshots ----

shot(
    "01-main-banner-menu.png",
    "01  Main banner + full detailed menu",
    [
        "",
        ("  =====================================================================", GREEN),
        ("  ||                                                                 ||", GREEN),
        ("  ||              MATRIX CALCULATOR  PF  (Professional)              ||", GREEN),
        ("  ||                                                                 ||", GREEN),
        ("  ||         Square matrices up to 10 x 10  |  Double precision GE    ||", GREEN),
        ("  ||         Author: Mohammad Rohaan  ·  Roll 22I-2327               ||", GREEN),
        ("  ||                                                                 ||", GREEN),
        ("  =====================================================================", GREEN),
        "",
        ("  ---------------------------------------------------------------------", DIM),
        ("   Status", TITLE),
        ("     Last operation : None", DIM),
        ("  ---------------------------------------------------------------------", DIM),
        "",
        ("   ========================  COURSE MENU (1-8)  ========================", TITLE),
        "",
        "     1.  Display matrix A",
        "     2.  Symmetric check          (also prints transpose)",
        "     3.  Identity check",
        "     4.  Determinant              (Gaussian elimination)",
        "     5.  Adjoint / adjugate",
        "     6.  Inverse                  (double precision)",
        "     7.  Solve linear system      Ax = b",
        "     8.  Exit program",
        "",
        ("   ========================  EXTRA TOOLS (9-19)  ========================", TITLE),
        "",
        "     9.  Transpose of A",
        "    10.  Matrix addition          A + B",
        "    11.  Matrix subtraction       A - B",
        "    12.  Matrix multiplication    A * B",
        "    13.  Scalar multiplication    k * A",
        "    14.  Trace of A",
        "    15.  Rank of A                (Gaussian elimination)",
        "    16.  Load sample matrices     (identity / singular / symmetric...)",
        "    17.  Show banner / about",
        "    18.  Save last result         -> result.txt",
        "    19.  Show last operation history",
        "",
        ("  ---------------------------------------------------------------------", DIM),
        ("   Enter option number (1 to 19)", YELLOW),
        ("  > _", PROMPT),
    ],
    height=920,
)

shot(
    "02-size-input.png",
    "02  Size input (validated 1..10)",
    [
        ("  ENTER THE SIZE OF THE SQUARE MATRIX (1..10): 12", YELLOW),
        ("  [Error] Value must be in [1 .. 10].", RED),
        ("  ENTER THE SIZE OF THE SQUARE MATRIX (1..10): 0", YELLOW),
        ("  [Error] Value must be in [1 .. 10].", RED),
        ("  ENTER THE SIZE OF THE SQUARE MATRIX (1..10): 3", GREEN),
        "",
        "  Accepted. Capacity const int N = 10.",
        "  Working matrix A remains untouched by later operations.",
    ],
)

shot(
    "03-matrix-entry.png",
    "03  Matrix entry (double precision)",
    [
        "========================================",
        "  Enter 9 entries for A (3x3)",
        "========================================",
        ("  A[1,1] >> 1", PROMPT),
        ("  A[1,2] >> 2", PROMPT),
        ("  A[1,3] >> 3", PROMPT),
        ("  A[2,1] >> 0", PROMPT),
        ("  A[2,2] >> 1", PROMPT),
        ("  A[2,3] >> 4", PROMPT),
        ("  A[3,1] >> 5", PROMPT),
        ("  A[3,2] >> 6", PROMPT),
        ("  A[3,3] >> 0", PROMPT),
        "",
        "  All values stored as double.",
    ],
)

shot(
    "04-display-matrix.png",
    "04  Option 1 — Display matrix",
    [
        ("  Choose option [1-19]: 1", PROMPT),
        "",
        "  --- Matrix A (3x3) ---",
        "  |    1.000000    2.000000    3.000000  |",
        "  |    0.000000    1.000000    4.000000  |",
        "  |    5.000000    6.000000    0.000000  |",
        "",
        ("  Last operation : Display matrix", DIM),
    ],
)

shot(
    "05-symmetric-yes.png",
    "05  Option 2 — Symmetric check (full n compare)",
    [
        "  --- Original A ---",
        "  |    2.000000    1.000000    0.000000  |",
        "  |    1.000000    3.000000    4.000000  |",
        "  |    0.000000    4.000000    5.000000  |",
        "",
        "  --- Transpose A^T ---",
        "  |    2.000000    1.000000    0.000000  |",
        "  |    1.000000    3.000000    4.000000  |",
        "  |    0.000000    4.000000    5.000000  |",
        "",
        ("  Result: A is SYMMETRIC (A = A^T).", GREEN),
    ],
)

shot(
    "06-identity-check.png",
    "06  Option 3 — Identity check (flag initialized)",
    [
        "  --- Matrix A ---",
        "  |    1.000000    0.000000    0.000000  |",
        "  |    0.000000    1.000000    0.000000  |",
        "  |    0.000000    0.000000    1.000000  |",
        "",
        ("  Result: A IS an identity matrix.", GREEN),
        "",
        ("  [Fixed] Identity flag is always initialized to true.", YELLOW),
    ],
)

shot(
    "07-determinant-2x2.png",
    "07  Option 4 — Determinant 2x2 (GE)",
    [
        "  A = [[2, 1], [5, 3]]",
        "  --- Matrix A ---",
        "  |    2.000000    1.000000  |",
        "  |    5.000000    3.000000  |",
        "",
        ("  Determinant (Gaussian elimination) = 1.000000", GREEN),
        ("  (stored in lastDet = 1.000000)", DIM),
        "",
        "  Manual check: 2*3 - 1*5 = 1",
    ],
)

shot(
    "08-determinant-3x3.png",
    "08  Option 4 — Determinant 3x3 (GE)",
    [
        "  A = [[1,2,3],[0,1,4],[5,6,0]]",
        "  --- Matrix A ---",
        "  |    1.000000    2.000000    3.000000  |",
        "  |    0.000000    1.000000    4.000000  |",
        "  |    5.000000    6.000000    0.000000  |",
        "",
        ("  Determinant (Gaussian elimination) = 1.000000", GREEN),
        "",
        "  Works for all n in 1..10 via partial-pivoting GE.",
    ],
)

shot(
    "09-adjoint.png",
    "09  Option 5 — Adjoint / adjugate",
    [
        "  A = [[2,1],[5,3]]",
        "  Adjoint via cofactors (course style, n <= 3):",
        "  |    3.000000   -1.000000  |",
        "  |   -5.000000    2.000000  |",
        "",
        "  Adjoint via A^{-1} * det (n <= 10, invertible):",
        "  |    3.000000   -1.000000  |",
        "  |   -5.000000    2.000000  |",
        ("  [OK] Cofactor adjoint matches Inv*det.", GREEN),
    ],
)

shot(
    "10-inverse-double.png",
    "10  Option 6 — Inverse (double, [A|I] GE)",
    [
        "  A = [[2,1],[5,3]]   det = 1",
        "  Inverse A^{-1} (Gaussian elimination on [A|I]):",
        "  |    3.000000   -1.000000  |",
        "  |   -5.000000    2.000000  |",
        "",
        ("  [Fixed] No integer truncation — full double precision.", YELLOW),
        "  Original A is never overwritten.",
    ],
)

shot(
    "11-solve-axb.png",
    "11  Option 7 — Solve Ax = b (correct n×1 input)",
    [
        "  A = [[2,1],[5,3]]",
        "  Enter 2 entries for b (n x 1)",
        ("  b[1] >> 1", PROMPT),
        ("  b[2] >> 1", PROMPT),
        "",
        "  Solution x of Ax = b:",
        ("  x[1] =     2.000000", GREEN),
        ("  x[2] =    -3.000000", GREEN),
        "",
        "  Check: 2(2)+1(-3)=1 ,  5(2)+3(-3)=1",
    ],
)

shot(
    "12-transpose.png",
    "12  Option 9 — Transpose",
    [
        ("  Choose option [1-19]: 9", PROMPT),
        "",
        "  --- Transpose A^T ---",
        "  |    1.000000    0.000000    5.000000  |",
        "  |    2.000000    1.000000    6.000000  |",
        "  |    3.000000    4.000000    0.000000  |",
        "",
        ("  Last operation : Transpose", DIM),
    ],
)

shot(
    "13-multiply.png",
    "13  Option 12 — Matrix multiplication",
    [
        "  A = [[1,2],[3,4]]   B = [[2,0],[1,2]]",
        "  --- A * B ---",
        "  |    4.000000    4.000000  |",
        "  |   10.000000    8.000000  |",
        "",
        "  Also available: 10 addition, 11 subtraction, 13 scalar.",
    ],
)

shot(
    "14-rank-trace.png",
    "14  Options 14–15 — Trace & Rank",
    [
        "  A = [[1,2,3],[0,1,4],[5,6,0]]",
        ("  Trace(A) = 2.000000", GREEN),
        ("  Rank(A) via Gaussian elimination = 3", GREEN),
        "",
        "  Singular sample [[1,2,3],[2,4,6],[1,1,1]]:",
        ("  Rank(A) via Gaussian elimination = 2", YELLOW),
    ],
)

shot(
    "15-singular-no-inverse.png",
    "15  Singular matrix — no inverse",
    [
        "  Loaded singular 3x3 (row2 = 2*row1).",
        "  |    1.000000    2.000000    3.000000  |",
        "  |    2.000000    4.000000    6.000000  |",
        "  |    1.000000    1.000000    1.000000  |",
        "",
        ("  det(A) = 0.000000", YELLOW),
        ("  [Error] Singular matrix (det = 0). Inverse does not exist.", RED),
    ],
)

shot(
    "16-continue-prompt.png",
    "16  Continue prompt (clean loops, no goto)",
    [
        "  ------------------------------------------------------------",
        "  Continue?  Y = same matrix  |  N = new matrix  |  0 = exit",
        ("  >>>>>>> Y", PROMPT),
        "",
        "  Returning to menu with the SAME matrix A.",
        "",
        ("  >>>>>>> N", PROMPT),
        "  Restart: prompt new size + new entries.",
        "",
        ("  >>>>>>> 0", PROMPT),
        ("  Program terminated. Thank you.", GREEN),
    ],
)

shot(
    "17-sample-presets.png",
    "17  Option 16 — Sample presets",
    [
        "  Sample presets:",
        "    1) Identity 3x3",
        "    2) Hilbert-ish integers 3x3",
        "    3) Singular 3x3",
        "    4) Symmetric 3x3",
        "    5) Classic 2x2  [[2,1],[5,3]]  det=1",
        "    6) Classic 3x3  [[1,2,3],[0,1,4],[5,6,0]]",
        ("  Choose sample [1-6]: 5", PROMPT),
        "  Loaded 2x2 [[2,1],[5,3]].",
    ],
)

shot(
    "18-full-session-demo.png",
    "18  Full session demo (walkthrough)",
    [
        ("  size=2 → enter [[2,1],[5,3]] → menu", WHITE),
        ("  [4] det = 1.000000", GREEN),
        ("  [5] adj = [[3,-1],[-5,2]]", GREEN),
        ("  [6] inv = [[3,-1],[-5,2]]", GREEN),
        ("  [7] b=[1,1] → x=[2,-3]", GREEN),
        ("  [14] trace = 5", GREEN),
        ("  [15] rank = 2", GREEN),
        ("  [18] saved result.txt", DIM),
        ("  [Y] continue same matrix → [8] exit", WHITE),
        "",
        ("  Professional PF calculator · 22I-2327", TITLE),
    ],
    height=520,
)

# ---- coverage gaps: arithmetic, asymmetric, 4x4 GE, singular solve,
#      save/history, size error focus, N-path, trace-only, feature map ----

shot(
    "19-matrix-addition.png",
    "19  Option 10 — Matrix addition A + B",
    [
        ("  Choose option [1-19]: 10", PROMPT),
        "",
        "  Enter matrix B (2x2) for A + B:",
        ("  B[1,1] >> 1", PROMPT),
        ("  B[1,2] >> 0", PROMPT),
        ("  B[2,1] >> 0", PROMPT),
        ("  B[2,2] >> 1", PROMPT),
        "",
        "  A = [[2,1],[5,3]]   B = [[1,0],[0,1]]",
        "  --- A + B ---",
        "  |    3.000000    1.000000  |",
        "  |    5.000000    4.000000  |",
        "",
        ("  Last operation : Addition A+B", DIM),
    ],
)

shot(
    "20-matrix-subtraction.png",
    "20  Option 11 — Matrix subtraction A − B",
    [
        ("  Choose option [1-19]: 11", PROMPT),
        "",
        "  Enter matrix B (2x2) for A - B:",
        ("  B[1,1] >> 1", PROMPT),
        ("  B[1,2] >> 1", PROMPT),
        ("  B[2,1] >> 1", PROMPT),
        ("  B[2,2] >> 1", PROMPT),
        "",
        "  A = [[2,1],[5,3]]   B = [[1,1],[1,1]]",
        "  --- A - B ---",
        "  |    1.000000    0.000000  |",
        "  |    4.000000    2.000000  |",
        "",
        ("  Last operation : Subtraction A-B", DIM),
    ],
)

shot(
    "21-scalar-multiply.png",
    "21  Option 13 — Scalar multiplication k · A",
    [
        ("  Choose option [1-19]: 13", PROMPT),
        ("  Enter scalar k >> 2.5", PROMPT),
        "",
        "  A = [[2,1],[5,3]]",
        "  --- k * A ---",
        "  |    5.000000    2.500000  |",
        "  |   12.500000    7.500000  |",
        "",
        ("  Last operation : Scalar multiply", DIM),
        "  Working matrix A is unchanged.",
    ],
)

shot(
    "22-asymmetric-check.png",
    "22  Option 2 — Asymmetric matrix (NOT symmetric)",
    [
        "  --- Original A ---",
        "  |    1.000000    2.000000  |",
        "  |    3.000000    4.000000  |",
        "",
        "  --- Transpose A^T ---",
        "  |    1.000000    3.000000  |",
        "  |    2.000000    4.000000  |",
        "",
        ("  Result: A is NOT symmetric.", RED),
        "",
        "  Full pairwise A[i][j] == A[j][i] for all n (not only 2/3).",
    ],
)

shot(
    "23-det-4x4-ge.png",
    "23  Option 4 — Determinant 4×4 (partial-pivoting GE)",
    [
        "  A (4x4 diagonal) = diag(1, 2, 3, 4)",
        "  --- Matrix A ---",
        "  |    1.000000    0.000000    0.000000    0.000000  |",
        "  |    0.000000    2.000000    0.000000    0.000000  |",
        "  |    0.000000    0.000000    3.000000    0.000000  |",
        "  |    0.000000    0.000000    0.000000    4.000000  |",
        "",
        ("  Determinant (Gaussian elimination) = 24.000000", GREEN),
        ("  (stored in lastDet = 24.000000)", DIM),
        "",
        "  GE + partial pivoting supports all n in 1..10 (not only 2×2 / 3×3).",
    ],
    height=560,
)

shot(
    "24-solve-singular.png",
    "24  Option 7 — Singular / inconsistent Ax = b",
    [
        "  Loaded singular 3x3 (row2 = 2*row1).",
        "  --- Coefficient matrix A ---",
        "  |    1.000000    2.000000    3.000000  |",
        "  |    2.000000    4.000000    6.000000  |",
        "  |    1.000000    1.000000    1.000000  |",
        "",
        "  Enter 3 entries for b (n x 1)",
        ("  b[1] >> 1", PROMPT),
        ("  b[2] >> 2", PROMPT),
        ("  b[3] >> 1", PROMPT),
        "",
        ("  [Error] No unique solution (singular / inconsistent system).", RED),
        "",
        "  Distinct from inverse refuse: solve reports no unique x.",
    ],
)

shot(
    "25-save-result-file.png",
    "25  Option 18 — Save last result to result.txt",
    [
        ("  Choose option [1-19]: 18", PROMPT),
        "",
        ("  Saved last result to result.txt", GREEN),
        ("  Last operation : Save result.txt", DIM),
        "",
        "  --- Contents of result.txt (excerpt) ---",
        ("  MatrixCalculator_PF — last result", YELLOW),
        "  Operation: Inverse",
        "  Matrix 2x2:",
        "         3.00000000   -1.00000000",
        "        -5.00000000    2.00000000",
        "  lastDet = 1.00000000",
        "",
        "  Optional dump of the last stored matrix or vector result.",
    ],
)

shot(
    "26-operation-history.png",
    "26  Option 19 — Operation history",
    [
        ("  Choose option [1-19]: 19", PROMPT),
        "",
        ("  Last operation name: Inverse", GREEN),
        "  Last result was a 2x2 matrix:",
        "  |    3.000000   -1.000000  |",
        "  |   -5.000000    2.000000  |",
        "",
        "  History recalls the last operation name and last numeric result",
        "  (matrix or vector). Does not overwrite working matrix A.",
    ],
)

shot(
    "27-invalid-size-error.png",
    "27  Size validation — out-of-range / non-integer",
    [
        ("  ENTER THE SIZE OF THE SQUARE MATRIX (1..10): -1", YELLOW),
        ("  [Error] Value must be in [1 .. 10].", RED),
        ("  ENTER THE SIZE OF THE SQUARE MATRIX (1..10): 11", YELLOW),
        ("  [Error] Value must be in [1 .. 10].", RED),
        ("  ENTER THE SIZE OF THE SQUARE MATRIX (1..10): abc", YELLOW),
        ("  [Error] Invalid integer. Try again.", RED),
        ("  ENTER THE SIZE OF THE SQUARE MATRIX (1..10): 4", GREEN),
        "",
        "  Loop re-prompts until a valid size in [1 .. 10] is accepted.",
        ("  Capacity: const int N = 10", DIM),
    ],
)

shot(
    "28-new-matrix-prompt.png",
    "28  Continue — N loads a new matrix",
    [
        "  ------------------------------------------------------------",
        "  Continue?  Y = same matrix  |  N = new matrix  |  0 = exit",
        ("  >>>>>>> N", PROMPT),
        "",
        "  Restart: prompt new size + new entries.",
        "",
        ("  ENTER THE SIZE OF THE SQUARE MATRIX (1..10): 2", GREEN),
        "========================================",
        "  Enter 4 entries for A (2x2)",
        "========================================",
        ("  A[1,1] >> 2", PROMPT),
        ("  A[1,2] >> 1", PROMPT),
        ("  A[2,1] >> 5", PROMPT),
        ("  A[2,2] >> 3", PROMPT),
        "",
        "  Y keeps A; N rebuilds A; 0 exits (no goto).",
    ],
)

shot(
    "29-trace-only-demo.png",
    "29  Option 14 — Trace (diagonal sum only)",
    [
        ("  Choose option [1-19]: 14", PROMPT),
        "",
        "  A = [[1,2,3],[0,1,4],[5,6,0]]",
        "  Diagonal: 1 + 1 + 0",
        ("  Trace(A) = 2.000000", GREEN),
        "",
        ("  Last operation : Trace", DIM),
        "  Stored as last scalar result (separate from rank).",
    ],
)

shot(
    "30-complete-feature-map.png",
    "30  Complete feature map (course + extras)",
    [
        ("   COURSE MENU (1-8)", TITLE),
        "",
        "     1. Display matrix A",
        "     2. Symmetric check (+ transpose)",
        "     3. Identity check",
        "     4. Determinant (GE)",
        "     5. Adjoint / adjugate",
        "     6. Inverse (double)",
        "     7. Solve Ax = b",
        "     8. Exit",
        "",
        ("   EXTRA TOOLS (9-19)", TITLE),
        "",
        "     9. Transpose",
        "    10. Addition A + B",
        "    11. Subtraction A - B",
        "    12. Multiplication A * B",
        "    13. Scalar k * A",
        "    14. Trace",
        "    15. Rank (GE)",
        "    16. Sample presets",
        "    17. Banner / about",
        "    18. Save -> result.txt",
        "    19. Operation history",
        "",
        ("   SESSION CONTROLS", TITLE),
        "",
        "     Y  continue with same matrix A",
        "     N  enter a new matrix",
        "     0  exit",
        "",
        ("   A never overwritten · GE for n ≤ 10 · double I/O", GREEN),
        ("   Screenshot gallery: 01–30  ·  22I-2327", GREEN),
    ],
    height=860,
)


print(f"\nDone. {len(list(OUT.glob('*.png')))} PNGs in {OUT}")
