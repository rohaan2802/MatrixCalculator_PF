#!/usr/bin/env python3
"""
Generate console-style dark-panel screenshots for MatrixCalculator_PF README.
Requires: Pillow  (pip install Pillow)
Output: docs/screenshots/01-...png through 18-...png
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


# ---- 18 screenshots ----

shot(
    "01-main-banner-menu.png",
    "01  Main banner + numbered menu",
    [
        ("  ============================================================", GREEN),
        ("  |          MATRIX CALCULATOR  PF  (Professional)           |", GREEN),
        ("  |              Square matrices up to 10 x 10                 |", GREEN),
        ("  |              Author: 22I-2327  ·  Double GE              |", GREEN),
        ("  ============================================================", GREEN),
        "",
        ("  Last operation : None", DIM),
        "   COURSE MENU",
        "    1. Display matrix A",
        "    2. Symmetric check  (+ show transpose)",
        "    3. Identity check",
        "    4. Determinant",
        "    5. Adjoint / adjugate",
        "    6. Inverse",
        "    7. Solve Ax = b",
        "    8. Exit",
        "   EXTRAS  9..19  (transpose, +,-,*, scalar, trace, rank, samples...)",
        ("  Choose option [1-19]: _", PROMPT),
    ],
    height=620,
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


print(f"\nDone. {len(list(OUT.glob('*.png')))} PNGs in {OUT}")
