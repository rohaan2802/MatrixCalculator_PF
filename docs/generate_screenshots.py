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
    "01  Welcome screen (home)",
    [
        "",
        ("  =====================================================================", GREEN),
        ("  ||                                                                 ||", GREEN),
        ("  ||           MATRIX CALCULATOR  PF                                 ||", GREEN),
        ("  ||                                                                 ||", GREEN),
        ("  ||     Square matrices up to 10 x 10                               ||", GREEN),
        ("  ||     Author: Mohammad Rohaan - 22I-2327                          ||", GREEN),
        ("  ||                                                                 ||", GREEN),
        ("  =====================================================================", GREEN),
        "",
        ("   WELCOME", TITLE),
        "",
        "     1  = Start calculator",
        "     0  = Exit",
        "",
        ("  Enter your choice: _", PROMPT),
    ],
    height=520,
)

shot(
    "02-size-input.png",
    "02  Size input (validated 1..10)",
    [
        ("   MATRIX SIZE", TITLE),
        "",
        "   Enter the size of the square matrix (1 to 10).",
        ("   (Enter 0 to go back)", DIM),
        "",
        ("  Enter matrix size: 12", YELLOW),
        ("  Please enter a number from 1 to 10 (or 0 to go back).", RED),
        ("  Enter matrix size: 3", GREEN),
        ("  Saved: 3", GREEN),
    ],
)

shot(
    "03-matrix-entry.png",
    "03  Matrix entry",
    [
        "  Enter matrix A (3 x 3)",
        "",
        ("  Enter value for Row 1, Column 1: 1", PROMPT),
        ("    saved: 1.0000", GREEN),
        ("  Enter value for Row 1, Column 2: 2", PROMPT),
        ("    saved: 2.0000", GREEN),
        ("  Enter value for Row 1, Column 3: 3", PROMPT),
        ("    saved: 3.0000", GREEN),
        "",
        ("  ... continue for each Row and Column ...", DIM),
    ],
)

shot(
    "04-display-matrix.png",
    "04  Option 1 — Show matrix (block form)",
    [
        ("  Enter your choice: 1", PROMPT),
        "",
        "  --- Matrix A (3x3) ---",
        ("  Matrix (3 x 3) — one block per value:", DIM),
        "",
        ("  +------------------------------------------+", TITLE),
        "  |  Row 1, Column 1",
        ("  |  Value : 1.000000", GREEN),
        ("  +------------------------------------------+", TITLE),
        "",
        ("  +------------------------------------------+", TITLE),
        "  |  Row 1, Column 2",
        ("  |  Value : 2.000000", GREEN),
        ("  +------------------------------------------+", TITLE),
        "",
        ("  +------------------------------------------+", TITLE),
        "  |  Row 1, Column 3",
        ("  |  Value : 3.000000", GREEN),
        ("  +------------------------------------------+", TITLE),
        "",
        ("  ... more open blocks for remaining cells ...", DIM),
        "",
        ("  Last step : Display matrix", DIM),
    ],
    height=720,
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
        ("  Enter your choice: 18", PROMPT),
        "",
        ("  Saved last result to result.txt (block format).", GREEN),
        "",
        "  --- Contents of result.txt (excerpt) ---",
        ("  ==============================================", YELLOW),
        "    Matrix Calculator PF — Last Result",
        ("  ==============================================", YELLOW),
        "    Last step : Inverse",
        "    Result type : Matrix (2 x 2)",
        "",
        "    +------------------------------------------+",
        "    |  Row 1, Column 1",
        "    |  Value : 3.00000000",
        "    +------------------------------------------+",
        "",
        "    +------------------------------------------+",
        "    |  Row 1, Column 2",
        "    |  Value : -1.00000000",
        "    +------------------------------------------+",
        "",
        ("  ... more open blocks for each cell ...", DIM),
    ],
    height=720,
)

shot(
    "26-operation-history.png",
    "26  Option 19 — Show last result (blocks)",
    [
        ("  Enter your choice: 19", PROMPT),
        "",
        ("  Last step: Inverse", GREEN),
        "  Last result (matrix 2 x 2):",
        "",
        "  +------------------------------------------+",
        "  |  Row 1, Column 1",
        ("  |  Value : 3.000000", GREEN),
        "  +------------------------------------------+",
        "",
        "  +------------------------------------------+",
        "  |  Row 1, Column 2",
        ("  |  Value : -1.000000", GREEN),
        "  +------------------------------------------+",
        "",
        ("  ... open blocks continue for every cell ...", DIM),
    ],
    height=640,
)

shot(
    "27-invalid-size-error.png",
    "27  Size validation — wrong input",
    [
        ("   MATRIX SIZE", TITLE),
        ("  Enter matrix size: -1", YELLOW),
        ("  Please enter a number from 1 to 10 (or 0 to go back).", RED),
        ("  Enter matrix size: 11", YELLOW),
        ("  Please enter a number from 1 to 10 (or 0 to go back).", RED),
        ("  Enter matrix size: abc", YELLOW),
        ("  That is not a whole number. Please try again.", RED),
        ("  Enter matrix size: 2", GREEN),
        ("  Saved: 2", GREEN),
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
        "     4. Determinant  —  Gaussian Elimination (GE)",
        "     5. Adjoint / adjugate",
        "     6. Inverse      —  Gaussian Elimination on [A | I]",
        "     7. Solve Ax=b   —  Gaussian Elimination on [A | b]",
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
        "    15. Rank        —  Gaussian Elimination (GE)",
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
        ("   NUMERICAL ENGINE", TITLE),
        "",
        ("   GE = Gaussian Elimination  (with partial pivoting)", GREEN),
        ("   Supported for all square sizes n = 1 .. 10  (n ≤ 10)", GREEN),
        ("   Used for: Determinant · Inverse · Ax = b · Rank", GREEN),
        ("   A never overwritten · double-precision I/O", GREEN),
        "",
        ("   Screenshot gallery: 01–30  ·  22I-2327", DIM),
    ],
    height=980,
)


print(f"\nDone. {len(list(OUT.glob('*.png')))} PNGs in {OUT}")
