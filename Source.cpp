/*
 * MatrixCalculator_PF — Professional square-matrix console calculator
 * Course DNA (menu 1–8) + real-world extras (9–19)
 * Author: Mohammad Rohaan · 22I-2327
 * Capacity: const int N = 10  |  Double precision for det / inv / solve
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <cstdio>
#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

using namespace std;

const int N = 10;
const double EPS = 1e-9;

/* Read helper results: OK / retry / user cancelled (back). */
enum ReadResult { RR_OK = 1, RR_RETRY = 0, RR_CANCEL = -1 };

/* ---------- console colors (real Windows terminal) ---------- */
enum ConColor {
    C_RESET = 0,
    C_TITLE,
    C_OK,
    C_WARN,
    C_ERR,
    C_DIM,
    C_PROMPT,
    C_HIGH
};

void setColor(ConColor c) {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    switch (c) {
    case C_TITLE:  attr = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
    case C_OK:     attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
    case C_WARN:   attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
    case C_ERR:    attr = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
    case C_DIM:    attr = FOREGROUND_INTENSITY; break;
    case C_PROMPT: attr = FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
    case C_HIGH:   attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
    default:       attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
    }
    SetConsoleTextAttribute(h, attr);
#else
    (void)c;
#endif
}

void enableConsoleUtf8() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(h, &mode))
        SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

#ifdef _WIN32
constexpr SHORT MIN_CONSOLE_COLS = 100;
constexpr SHORT CONSOLE_SCROLLBACK = 400;
constexpr SHORT FONT_CELL_Y = 28;

void snapConsoleToFontGrid(HANDLE hOut) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hOut, &csbi))
        return;
    HWND hwnd = GetConsoleWindow();
    if (!hwnd)
        return;
    RECT rc;
    if (!GetClientRect(hwnd, &rc))
        return;
    int pxW = rc.right - rc.left;
    int pxH = rc.bottom - rc.top;
    if (pxW < 80 || pxH < 40)
        return;

    CONSOLE_FONT_INFOEX cfi;
    ZeroMemory(&cfi, sizeof(cfi));
    cfi.cbSize = sizeof(cfi);
    if (!GetCurrentConsoleFontEx(hOut, FALSE, &cfi))
        return;
    int cellW = cfi.dwFontSize.X > 0 ? cfi.dwFontSize.X : 8;
    int cellH = cfi.dwFontSize.Y > 0 ? cfi.dwFontSize.Y : FONT_CELL_Y;
    if (cellW < 1) cellW = 8;
    if (cellH < 1) cellH = FONT_CELL_Y;

    SHORT cols = (SHORT)(pxW / cellW);
    SHORT rows = (SHORT)(pxH / cellH);
    if (cols < MIN_CONSOLE_COLS) cols = MIN_CONSOLE_COLS;
    if (rows < 20) rows = 20;

    COORD buf = csbi.dwSize;
    if (buf.X < cols) buf.X = cols;
    if (buf.Y < rows + CONSOLE_SCROLLBACK)
        buf.Y = (SHORT)(rows + CONSOLE_SCROLLBACK);
    SetConsoleScreenBufferSize(hOut, buf);

    SMALL_RECT win = { 0, 0, (SHORT)(cols - 1), (SHORT)(rows - 1) };
    SetConsoleWindowInfo(hOut, TRUE, &win);
}

void fillRestOfWindowBlank(HANDLE hOut) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hOut, &csbi))
        return;
    SHORT winW = (SHORT)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
    SHORT winH = (SHORT)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
    SHORT curY = csbi.dwCursorPosition.Y;
    SHORT top = csbi.srWindow.Top;
    if (curY < top)
        return;
    SHORT rowsLeft = (SHORT)(winH - (curY - top));
    if (rowsLeft <= 0)
        return;
    DWORD written = 0;
    COORD start = { 0, curY };
    DWORD cells = (DWORD)winW * (DWORD)rowsLeft;
    FillConsoleOutputCharacterA(hOut, ' ', cells, start, &written);
    FillConsoleOutputAttribute(hOut, csbi.wAttributes, cells, start, &written);
}
#endif

/* Clear visible area + scrollback paint — no buffer swap (avoids ghost / wrap). */
void clearScreen() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE || hOut == NULL)
        return;
    cout.flush();
    snapConsoleToFontGrid(hOut);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hOut, &csbi))
        return;

    DWORD cells = (DWORD)csbi.dwSize.X * (DWORD)csbi.dwSize.Y;
    DWORD written = 0;
    COORD home = { 0, 0 };
    FillConsoleOutputCharacterA(hOut, ' ', cells, home, &written);
    FillConsoleOutputAttribute(hOut, csbi.wAttributes, cells, home, &written);
    SetConsoleCursorPosition(hOut, home);

    SMALL_RECT scroll = csbi.srWindow;
    CHAR_INFO fill;
    fill.Char.AsciiChar = ' ';
    fill.Attributes = csbi.wAttributes;
    ScrollConsoleScreenBufferA(hOut, &scroll, NULL, home, &fill);
    SetConsoleCursorPosition(hOut, home);

    fillRestOfWindowBlank(hOut);
#else
    cout << "\n\n";
#endif
}

void setupConsoleDisplay() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE || hOut == NULL)
        return;

    CONSOLE_FONT_INFOEX cfi;
    ZeroMemory(&cfi, sizeof(cfi));
    cfi.cbSize = sizeof(cfi);
    if (GetCurrentConsoleFontEx(hOut, FALSE, &cfi)) {
        cfi.dwFontSize.X = 0;
        cfi.dwFontSize.Y = FONT_CELL_Y;
        wcscpy_s(cfi.FaceName, L"Consolas");
        SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
    }

    HWND hwnd = GetConsoleWindow();
    if (hwnd)
        ShowWindow(hwnd, SW_MAXIMIZE);

    snapConsoleToFontGrid(hOut);
#endif
}

string g_flashMsg;

void setFlash(const string& msg) { g_flashMsg = msg; }

void printFlashIfAny() {
    if (g_flashMsg.empty())
        return;
    setColor(C_ERR);
    cout << "\n  " << g_flashMsg << "\n";
    setColor(C_RESET);
    g_flashMsg.clear();
}

string toLowerCopy(const string& s) {
    string t;
    t.reserve(s.size());
    for (size_t i = 0; i < s.size(); i++)
        t.push_back((char)tolower((unsigned char)s[i]));
    return t;
}

/* Only 0 goes back / cancel on choice screens. */
bool isCancelToken(const string& s) {
    return toLowerCopy(s) == "0";
}

void printBackHint() {
    setColor(C_DIM);
    cout << "   (Enter 0 to go back)\n";
    setColor(C_RESET);
}

/* ---------- shared session state ---------- */
double lastDet = 0.0;
char lastOpName[64] = "None";
double lastResult[N][N];
int lastResultRows = 0;
int lastResultCols = 0;
bool lastResultIsVector = false;
double lastVector[N];
int lastVectorLen = 0;
bool hasLastResult = false;

void setOpName(const char* name) {
    size_t i = 0;
    while (name[i] != '\0' && i < sizeof(lastOpName) - 1) {
        lastOpName[i] = name[i];
        i++;
    }
    lastOpName[i] = '\0';
}

/* ---------- helpers: copy / print / I/O ---------- */
void copyMatrix(const double src[N][N], double dst[N][N], int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            dst[i][j] = src[i][j];
}

void zeroMatrix(double M[N][N], int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            M[i][j] = 0.0;
}

void writeMatrixGrid(ostream& out, const double M[N][N], int rows, int cols, int prec = 6) {
    out << fixed << setprecision(prec);
    out << "\n        ";
    for (int j = 0; j < cols; j++)
        out << setw(14) << ("Column " + to_string(j + 1));
    out << "\n";
    for (int i = 0; i < rows; i++) {
        out << "  Row " << (i + 1) << "  |";
        for (int j = 0; j < cols; j++)
            out << setw(14) << M[i][j];
        out << "  |\n";
    }
    out << "\n" << defaultfloat;
}

void writeVectorList(ostream& out, const double v[], int n, const char* label, int prec = 6) {
    out << fixed << setprecision(prec);
    out << "\n";
    for (int i = 0; i < n; i++)
        out << "  " << label << " " << (i + 1) << "  =  " << setw(14) << v[i] << "\n";
    out << "\n" << defaultfloat;
}

void printMatrix(const double M[N][N], int rows, int cols, int prec = 6) {
    setColor(C_DIM);
    cout << "\n  Matrix (" << rows << " x " << cols << "):\n";
    cout << "        ";
    for (int j = 0; j < cols; j++)
        cout << setw(14) << ("Column " + to_string(j + 1));
    cout << "\n";
    setColor(C_RESET);

    cout << fixed << setprecision(prec);
    for (int i = 0; i < rows; i++) {
        setColor(C_DIM);
        cout << "  Row " << (i + 1) << "  ";
        setColor(C_HIGH);
        cout << "|";
        for (int j = 0; j < cols; j++)
            cout << setw(14) << M[i][j];
        cout << "  |\n";
    }
    setColor(C_RESET);
    cout << defaultfloat << "\n";
}

void printVector(const double v[], int n, int prec = 6) {
    cout << fixed << setprecision(prec);
    cout << "\n";
    for (int i = 0; i < n; i++) {
        setColor(C_OK);
        cout << "  Entry " << (i + 1) << "  =  ";
        setColor(C_HIGH);
        cout << setw(14) << v[i] << "\n";
    }
    setColor(C_RESET);
    cout << defaultfloat << "\n";
}

void storeLastMatrix(const double M[N][N], int rows, int cols, const char* opName) {
    setOpName(opName);
    lastResultRows = rows;
    lastResultCols = cols;
    lastResultIsVector = false;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            lastResult[i][j] = M[i][j];
    hasLastResult = true;
}

void storeLastVector(const double v[], int n, const char* opName) {
    setOpName(opName);
    lastResultIsVector = true;
    lastVectorLen = n;
    for (int i = 0; i < n; i++)
        lastVector[i] = v[i];
    hasLastResult = true;
}

void storeLastScalar(double value, const char* opName) {
    setOpName(opName);
    lastResultIsVector = false;
    lastResultRows = 1;
    lastResultCols = 1;
    lastResult[0][0] = value;
    hasLastResult = true;
}

/* Returns RR_OK, RR_RETRY, or RR_CANCEL (0 when zeroCancels). */
ReadResult readIntInRange(const char* prompt, int lo, int hi, int& out, bool zeroCancels = false) {
    setColor(C_PROMPT);
    cout << prompt;
    setColor(C_RESET);
    string tok;
    if (!(cin >> tok)) {
        if (cin.eof())
            return RR_CANCEL;
        cin.clear();
        cin.ignore(10000, '\n');
        setColor(C_ERR);
        cout << "  That is not a whole number. Please try again.\n";
        setColor(C_RESET);
        return RR_RETRY;
    }
    if (zeroCancels && isCancelToken(tok))
        return RR_CANCEL;

    char* endp = nullptr;
    long v = strtol(tok.c_str(), &endp, 10);
    if (endp == tok.c_str() || *endp != '\0') {
        setColor(C_ERR);
        cout << "  That is not a whole number. Please try again.\n";
        setColor(C_RESET);
        return RR_RETRY;
    }
    out = (int)v;
    if (out < lo || out > hi) {
        setColor(C_ERR);
        cout << "  Please enter a number from " << lo << " to " << hi;
        if (zeroCancels)
            cout << " (or 0 to go back)";
        cout << ".\n";
        setColor(C_RESET);
        return RR_RETRY;
    }
    setColor(C_OK);
    cout << "  Saved: " << out << "\n";
    setColor(C_RESET);
    return RR_OK;
}

/* Numeric read. "0" is a valid value (no cancel here). */
ReadResult readDouble(const char* prompt, double& out) {
    setColor(C_PROMPT);
    cout << prompt;
    setColor(C_RESET);
    string tok;
    if (!(cin >> tok)) {
        if (cin.eof())
            return RR_CANCEL;
        cin.clear();
        cin.ignore(10000, '\n');
        setColor(C_ERR);
        cout << "  That is not a number. Example: 3.5\n";
        setColor(C_RESET);
        return RR_RETRY;
    }
    char* endp = nullptr;
    double v = strtod(tok.c_str(), &endp);
    if (endp == tok.c_str() || *endp != '\0') {
        setColor(C_ERR);
        cout << "  That is not a number. Example: 3.5\n";
        setColor(C_RESET);
        return RR_RETRY;
    }
    out = v;
    return RR_OK;
}

/* Ask 1=continue / 0=back before a multi-step entry. */
bool confirmOrBack(const char* titleLine) {
    for (;;) {
        clearScreen();
        printFlashIfAny();
        setColor(C_TITLE);
        cout << "\n  " << titleLine << "\n\n";
        setColor(C_HIGH);
        cout << "     1  = Continue\n";
        cout << "     0  = Go back\n";
        setColor(C_PROMPT);
        cout << "  Enter your choice: ";
        setColor(C_RESET);
        string tok;
        if (!(cin >> tok)) {
            if (cin.eof())
                return false;
            cin.clear();
            cin.ignore(10000, '\n');
            setFlash("Please type 1 or 0.");
            continue;
        }
        if (isCancelToken(tok))
            return false;
        if (toLowerCopy(tok) == "1")
            return true;
        setFlash("Wrong choice. Type 1 to continue or 0 to go back.");
    }
}

/* false = user went back. label e.g. "A" or "B". */
bool inputMatrix(double M[N][N], int n, const char* label) {
    char title[96];
    sprintf_s(title, "Enter matrix %s (%d x %d)", label, n, n);
    if (!confirmOrBack(title))
        return false;

    clearScreen();
    setColor(C_TITLE);
    cout << "\n  =====================================================================\n";
    cout << "   INPUT MATRIX " << label << "  (" << n << " x " << n
         << ")   —  total entries: " << (n * n) << "\n";
    cout << "  =====================================================================\n";
    setColor(C_DIM);
    cout << "   Enter one number at a time. Decimals are allowed (example: 2.5).\n";
    cout << "   If a value is wrong, you can type it again for that box.\n\n";
    setColor(C_RESET);

    for (int i = 0; i < n; i++) {
        setColor(C_WARN);
        cout << "  --- Row " << (i + 1) << " of " << n << " ---\n";
        setColor(C_RESET);
        for (int j = 0; j < n; j++) {
            double val = 0.0;
            for (;;) {
                char cellPrompt[96];
                sprintf_s(cellPrompt, "  Enter value for Row %d, Column %d: ", i + 1, j + 1);
                ReadResult rr = readDouble(cellPrompt, val);
                if (rr == RR_OK)
                    break;
            }
            M[i][j] = val;
            setColor(C_OK);
            cout << "    saved: " << fixed << setprecision(4) << val << defaultfloat << "\n";
            setColor(C_RESET);
        }
        cout << "\n";
    }

    setColor(C_OK);
    cout << "  ---------------------------------------------------------------------\n";
    cout << "   Done — preview of matrix " << label << ":\n";
    cout << "  ---------------------------------------------------------------------\n";
    setColor(C_RESET);
    printMatrix(M, n, n);
    cout << "\n";
    return true;
}

/* false = user went back */
bool inputVector(double b[N], int n) {
    if (!confirmOrBack("Enter vector b for the equation A x = b"))
        return false;

    clearScreen();
    setColor(C_TITLE);
    cout << "\n  =====================================================================\n";
    cout << "   INPUT VECTOR b  (" << n << " x 1)\n";
    cout << "  =====================================================================\n\n";
    setColor(C_RESET);
    for (int i = 0; i < n; i++) {
        double val = 0.0;
        for (;;) {
            char cellPrompt[64];
            sprintf_s(cellPrompt, "  Enter value for Entry %d: ", i + 1);
            ReadResult rr = readDouble(cellPrompt, val);
            if (rr == RR_OK)
                break;
        }
        b[i] = val;
        setColor(C_OK);
        cout << "    saved: " << fixed << setprecision(4) << val << defaultfloat << "\n";
        setColor(C_RESET);
    }
    setColor(C_OK);
    cout << "\n   Vector b preview:\n";
    setColor(C_RESET);
    printVector(b, n);
    return true;
}

/* ---------- Gaussian elimination (partial pivoting) ---------- */

/* Determinant via GE on a working copy. Updates lastDet. */
double determinantGE(const double A[N][N], int n) {
    double T[N][N];
    copyMatrix(A, T, n);
    double det = 1.0;
    int sign = 1;

    for (int col = 0; col < n; col++) {
        int pivot = col;
        for (int r = col + 1; r < n; r++) {
            if (fabs(T[r][col]) > fabs(T[pivot][col]))
                pivot = r;
        }
        if (fabs(T[pivot][col]) < EPS) {
            lastDet = 0.0;
            return 0.0;
        }
        if (pivot != col) {
            for (int c = 0; c < n; c++) {
                double tmp = T[col][c];
                T[col][c] = T[pivot][c];
                T[pivot][c] = tmp;
            }
            sign = -sign;
        }
        det *= T[col][col];
        for (int r = col + 1; r < n; r++) {
            double factor = T[r][col] / T[col][col];
            for (int c = col; c < n; c++)
                T[r][c] -= factor * T[col][c];
        }
    }
    lastDet = det * sign;
    return lastDet;
}

/* Inverse via augmented [A | I] GE. Returns false if singular. */
bool inverseGE(const double A[N][N], int n, double Inv[N][N]) {
    double Aug[N][2 * N];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            Aug[i][j] = A[i][j];
        for (int j = 0; j < n; j++)
            Aug[i][n + j] = (i == j) ? 1.0 : 0.0;
    }

    for (int col = 0; col < n; col++) {
        int pivot = col;
        for (int r = col + 1; r < n; r++) {
            if (fabs(Aug[r][col]) > fabs(Aug[pivot][col]))
                pivot = r;
        }
        if (fabs(Aug[pivot][col]) < EPS)
            return false;
        if (pivot != col) {
            for (int c = 0; c < 2 * n; c++) {
                double tmp = Aug[col][c];
                Aug[col][c] = Aug[pivot][c];
                Aug[pivot][c] = tmp;
            }
        }
        double div = Aug[col][col];
        for (int c = 0; c < 2 * n; c++)
            Aug[col][c] /= div;
        for (int r = 0; r < n; r++) {
            if (r == col) continue;
            double factor = Aug[r][col];
            for (int c = 0; c < 2 * n; c++)
                Aug[r][c] -= factor * Aug[col][c];
        }
    }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            Inv[i][j] = Aug[i][n + j];
    return true;
}

/* Solve Ax = b via GE on [A|b]. Returns false if singular / no unique solution. */
bool solveGE(const double A[N][N], const double b[N], int n, double x[N]) {
    double Aug[N][N + 1];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            Aug[i][j] = A[i][j];
        Aug[i][n] = b[i];
    }

    for (int col = 0; col < n; col++) {
        int pivot = col;
        for (int r = col + 1; r < n; r++) {
            if (fabs(Aug[r][col]) > fabs(Aug[pivot][col]))
                pivot = r;
        }
        if (fabs(Aug[pivot][col]) < EPS)
            return false;
        if (pivot != col) {
            for (int c = 0; c <= n; c++) {
                double tmp = Aug[col][c];
                Aug[col][c] = Aug[pivot][c];
                Aug[pivot][c] = tmp;
            }
        }
        double div = Aug[col][col];
        for (int c = 0; c <= n; c++)
            Aug[col][c] /= div;
        for (int r = 0; r < n; r++) {
            if (r == col) continue;
            double factor = Aug[r][col];
            for (int c = 0; c <= n; c++)
                Aug[r][c] -= factor * Aug[col][c];
        }
    }
    for (int i = 0; i < n; i++)
        x[i] = Aug[i][n];
    return true;
}

/* Rank via GE row reduction. */
int rankGE(const double A[N][N], int n) {
    double T[N][N];
    copyMatrix(A, T, n);
    int rank = 0;
    int row = 0;
    for (int col = 0; col < n && row < n; col++) {
        int pivot = row;
        for (int r = row + 1; r < n; r++) {
            if (fabs(T[r][col]) > fabs(T[pivot][col]))
                pivot = r;
        }
        if (fabs(T[pivot][col]) < EPS)
            continue;
        if (pivot != row) {
            for (int c = 0; c < n; c++) {
                double tmp = T[row][c];
                T[row][c] = T[pivot][c];
                T[pivot][c] = tmp;
            }
        }
        double div = T[row][col];
        for (int c = 0; c < n; c++)
            T[row][c] /= div;
        for (int r = 0; r < n; r++) {
            if (r == row) continue;
            double factor = T[r][col];
            for (int c = 0; c < n; c++)
                T[r][c] -= factor * T[row][c];
        }
        rank++;
        row++;
    }
    return rank;
}

/* ---------- cofactor adjoint for n <= 3 (course style) ---------- */
double minor2x2(const double A[N][N], int r0, int c0, int r1, int c1) {
    return A[r0][c0] * A[r1][c1] - A[r0][c1] * A[r1][c0];
}

bool adjointCofactors(const double A[N][N], int n, double Adj[N][N]) {
    if (n == 1) {
        Adj[0][0] = 1.0;
        return true;
    }
    if (n == 2) {
        Adj[0][0] = A[1][1];
        Adj[0][1] = -A[0][1];
        Adj[1][0] = -A[1][0];
        Adj[1][1] = A[0][0];
        return true;
    }
    if (n == 3) {
        double C[3][3];
        C[0][0] = minor2x2(A, 1, 1, 2, 2);
        C[0][1] = -minor2x2(A, 1, 0, 2, 2);
        C[0][2] = minor2x2(A, 1, 0, 2, 1);
        C[1][0] = -minor2x2(A, 0, 1, 2, 2);
        C[1][1] = minor2x2(A, 0, 0, 2, 2);
        C[1][2] = -minor2x2(A, 0, 0, 2, 1);
        C[2][0] = minor2x2(A, 0, 1, 1, 2);
        C[2][1] = -minor2x2(A, 0, 0, 1, 2);
        C[2][2] = minor2x2(A, 0, 0, 1, 1);
        /* adjugate = transpose of cofactor matrix */
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                Adj[i][j] = C[j][i];
        return true;
    }
    return false;
}

/* Adjoint via Inv * det when invertible (works for all n <= 10). */
bool adjointViaInverse(const double A[N][N], int n, double Adj[N][N]) {
    double Inv[N][N];
    double det = determinantGE(A, n);
    if (fabs(det) < EPS)
        return false;
    if (!inverseGE(A, n, Inv))
        return false;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            Adj[i][j] = Inv[i][j] * det;
    return true;
}

/* ---------- UI ---------- */
/* Fixed inner width so the right border stays on one column. */
static const int BANNER_INNER = 65;

void printBannerLine(const string& inner) {
    string s = inner;
    if ((int)s.size() > BANNER_INNER)
        s = s.substr(0, BANNER_INNER);
    while ((int)s.size() < BANNER_INNER)
        s.push_back(' ');
    cout << "  ||" << s << "||\n";
}

void printBanner() {
    setColor(C_OK);
    cout << "\n";
    cout << "  " << string(BANNER_INNER + 4, '=') << "\n";
    printBannerLine("");
    printBannerLine("           MATRIX CALCULATOR  PF");
    printBannerLine("");
    printBannerLine("     Square matrices up to 10 x 10");
    printBannerLine("     Author: Mohammad Rohaan - 22I-2327");
    printBannerLine("");
    cout << "  " << string(BANNER_INNER + 4, '=') << "\n\n";
    setColor(C_RESET);
}

/* Welcome: 1 = start, 0 = exit. Wrong input re-asks (does not continue). */
bool welcomeScreen() {
    for (;;) {
        clearScreen();
        printFlashIfAny();
        printBanner();
        setColor(C_TITLE);
        cout << "   WELCOME\n\n";
        setColor(C_HIGH);
        cout << "     1  = Start calculator\n";
        cout << "     0  = Exit\n";
        setColor(C_PROMPT);
        cout << "  Enter your choice: ";
        setColor(C_RESET);

        string tok;
        if (!(cin >> tok)) {
            if (cin.eof())
                return false;
            cin.clear();
            cin.ignore(10000, '\n');
            setFlash("Please type 1 or 0.");
            continue;
        }
        if (isCancelToken(tok)) {
            clearScreen();
            setColor(C_OK);
            cout << "\n  Goodbye. Thank you for using Matrix Calculator.\n\n";
            setColor(C_RESET);
            return false;
        }
        if (toLowerCopy(tok) == "1")
            return true;

        setFlash("Wrong choice. Type 1 to start or 0 to exit.");
    }
}

void printMenu() {
    clearScreen();
    printFlashIfAny();
    setColor(C_DIM);
    cout << "\n";
    cout << "  ---------------------------------------------------------------------\n";
    setColor(C_TITLE);
    cout << "   Status\n";
    setColor(C_HIGH);
    cout << "     Last step : " << lastOpName << "\n";
    setColor(C_DIM);
    cout << "  ---------------------------------------------------------------------\n\n";

    setColor(C_TITLE);
    cout << "   ========================  MAIN MENU (1-8)  =========================\n\n";
    setColor(C_HIGH);
    cout << "     1.  Show matrix A\n";
    cout << "     2.  Check if A is symmetric\n";
    cout << "     3.  Check if A is identity\n";
    cout << "     4.  Determinant of A\n";
    cout << "     5.  Adjoint of A\n";
    cout << "     6.  Inverse of A\n";
    cout << "     7.  Solve A x = b\n";
    cout << "     8.  Exit program\n\n";

    setColor(C_TITLE);
    cout << "   ========================  MORE TOOLS (9-19)  =======================\n\n";
    setColor(C_HIGH);
    cout << "     9.  Transpose of A\n";
    cout << "    10.  Add matrices            A + B\n";
    cout << "    11.  Subtract matrices       A - B\n";
    cout << "    12.  Multiply matrices       A * B\n";
    cout << "    13.  Multiply by a number    k * A\n";
    cout << "    14.  Trace of A\n";
    cout << "    15.  Rank of A\n";
    cout << "    16.  Load a sample matrix\n";
    cout << "    17.  Show about / title\n";
    cout << "    18.  Save last result        (result.txt)\n";
    cout << "    19.  Show last result\n\n";

    setColor(C_DIM);
    cout << "  ---------------------------------------------------------------------\n";
    setColor(C_WARN);
    cout << "   Choose 1 to 19     |     0 = go back to welcome\n";
    setColor(C_PROMPT);
    cout << "  Enter your choice: ";
    setColor(C_RESET);
}

/* ---------- operations (A is NEVER overwritten) ---------- */

void opDisplay(const double A[N][N], int n) {
    cout << "\n  --- Matrix A (" << n << "x" << n << ") ---\n";
    printMatrix(A, n, n);
    storeLastMatrix(A, n, n, "Display matrix");
}

void opSymmetric(const double A[N][N], int n) {
    cout << "\n  --- Original A ---\n";
    printMatrix(A, n, n);
    double T[N][N];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            T[i][j] = A[j][i];
    cout << "\n  --- Transpose A^T ---\n";
    printMatrix(T, n, n);

    bool sym = true;
    for (int i = 0; i < n && sym; i++)
        for (int j = 0; j < n && sym; j++)
            if (fabs(A[i][j] - A[j][i]) > EPS)
                sym = false;

    if (sym)
        cout << "\n  Result: A is SYMMETRIC (A = A^T).\n";
    else
        cout << "\n  Result: A is NOT symmetric.\n";
    storeLastMatrix(T, n, n, "Symmetric check / transpose");
}

void opIdentity(const double A[N][N], int n) {
    cout << "\n  --- Matrix A ---\n";
    printMatrix(A, n, n);
    bool isId = true; /* MUST initialize */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double expect = (i == j) ? 1.0 : 0.0;
            if (fabs(A[i][j] - expect) > EPS)
                isId = false;
        }
    }
    if (isId)
        cout << "\n  Result: A IS an identity matrix.\n";
    else
        cout << "\n  Result: A is NOT an identity matrix.\n";
    storeLastMatrix(A, n, n, "Identity check");
}

void opDeterminant(const double A[N][N], int n) {
    cout << "\n  --- Matrix A ---\n";
    printMatrix(A, n, n);
    double det = determinantGE(A, n);
    cout << fixed << setprecision(6);
    cout << "\n  Determinant = " << det << "\n";
    cout << defaultfloat;
    storeLastScalar(det, "Determinant");
}

void opAdjoint(const double A[N][N], int n) {
    cout << "\n  --- Matrix A ---\n";
    printMatrix(A, n, n);
    double Adj[N][N];
    zeroMatrix(Adj, n);

    if (n <= 3) {
        adjointCofactors(A, n, Adj);
        cout << "\n  Adjoint of A:\n";
        printMatrix(Adj, n, n);
        storeLastMatrix(Adj, n, n, "Adjoint");
    }

    double Adj2[N][N];
    if (adjointViaInverse(A, n, Adj2)) {
        if (n > 3) {
            cout << "\n  Adjoint of A:\n";
            printMatrix(Adj2, n, n);
            storeLastMatrix(Adj2, n, n, "Adjoint");
        } else {
            bool match = true;
            for (int i = 0; i < n && match; i++)
                for (int j = 0; j < n && match; j++)
                    if (fabs(Adj[i][j] - Adj2[i][j]) > 1e-6)
                        match = false;
            if (!match)
                cout << "  (Tiny rounding differences are normal.)\n";
        }
    } else {
        if (n > 3) {
            cout << "\n  Sorry: this matrix has no adjoint this way\n";
            cout << "  because its determinant is zero.\n";
            cout << "  Tip: try a matrix size 1 to 3, or use a different matrix.\n";
        } else {
            cout << "\n  Note: determinant is zero, but the adjoint above is still valid.\n";
        }
    }
}

void opInverse(const double A[N][N], int n) {
    cout << "\n  --- Matrix A ---\n";
    printMatrix(A, n, n);
    double det = determinantGE(A, n);
    cout << fixed << setprecision(6);
    cout << "  Determinant = " << det << "\n";
    cout << defaultfloat;

    double Inv[N][N];
    if (!inverseGE(A, n, Inv)) {
        cout << "\n  Sorry: inverse does not exist for this matrix\n";
        cout << "  (determinant is zero).\n";
        return;
    }
    cout << "\n  Inverse of A:\n";
    printMatrix(Inv, n, n);
    storeLastMatrix(Inv, n, n, "Inverse");
}

void opSolve(const double A[N][N], int n) {
    cout << "\n  --- Matrix A ---\n";
    printMatrix(A, n, n);
    double b[N], x[N];
    if (!inputVector(b, n))
        return;
    cout << "\n  --- Vector b ---\n";
    printVector(b, n);

    if (!solveGE(A, b, n, x)) {
        cout << "\n  Sorry: there is no single unique answer for this system.\n";
        return;
    }
    cout << "\n  Solution x:\n";
    printVector(x, n);
    storeLastVector(x, n, "Solve Ax=b");
}

void opTranspose(const double A[N][N], int n) {
    double T[N][N];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            T[i][j] = A[j][i];
    cout << "\n  --- Transpose A^T ---\n";
    printMatrix(T, n, n);
    storeLastMatrix(T, n, n, "Transpose");
}

void opAdd(const double A[N][N], int n) {
    double B[N][N], C[N][N];
    cout << "\n  Enter matrix B (" << n << "x" << n << ") for A + B:\n";
    if (!inputMatrix(B, n, "B"))
        return;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
    cout << "\n  --- A + B ---\n";
    printMatrix(C, n, n);
    storeLastMatrix(C, n, n, "Addition A+B");
}

void opSub(const double A[N][N], int n) {
    double B[N][N], C[N][N];
    cout << "\n  Enter matrix B (" << n << "x" << n << ") for A - B:\n";
    if (!inputMatrix(B, n, "B"))
        return;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
    cout << "\n  --- A - B ---\n";
    printMatrix(C, n, n);
    storeLastMatrix(C, n, n, "Subtraction A-B");
}

void opMul(const double A[N][N], int n) {
    double B[N][N], C[N][N];
    cout << "\n  Enter matrix B (" << n << "x" << n << ") for A * B:\n";
    if (!inputMatrix(B, n, "B"))
        return;
    zeroMatrix(C, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                C[i][j] += A[i][k] * B[k][j];
    cout << "\n  --- A * B ---\n";
    printMatrix(C, n, n);
    storeLastMatrix(C, n, n, "Multiplication A*B");
}

void opScalar(const double A[N][N], int n) {
    if (!confirmOrBack("Multiply matrix A by a number k"))
        return;
    clearScreen();
    double k = 0.0;
    for (;;) {
        ReadResult rr = readDouble("  Enter the number k: ", k);
        if (rr == RR_OK)
            break;
    }
    double C[N][N];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = k * A[i][j];
    cout << "\n  --- k * A ---\n";
    printMatrix(C, n, n);
    storeLastMatrix(C, n, n, "Scalar multiply");
}

void opTrace(const double A[N][N], int n) {
    double tr = 0.0;
    for (int i = 0; i < n; i++)
        tr += A[i][i];
    cout << fixed << setprecision(6);
    cout << "\n  Trace(A) = " << tr << "\n";
    cout << defaultfloat;
    storeLastScalar(tr, "Trace");
}

void opRank(const double A[N][N], int n) {
    int r = rankGE(A, n);
    cout << "\n  Rank(A) = " << r << "\n";
    storeLastScalar((double)r, "Rank");
}

void opLoadSample(double A[N][N], int& n) {
    for (;;) {
        clearScreen();
        printFlashIfAny();
        cout << "\n  Sample matrices:\n";
        cout << "    1) Identity 3x3\n";
        cout << "    2) Simple 3x3 numbers\n";
        cout << "    3) No-inverse example 3x3\n";
        cout << "    4) Symmetric 3x3\n";
        cout << "    5) Classic 2x2\n";
        cout << "    6) Classic 3x3\n";
        printBackHint();
        int choice = 0;
        ReadResult rr = readIntInRange("  Enter your choice: ", 1, 6, choice, true);
        if (rr == RR_CANCEL)
            return;
        if (rr != RR_OK) {
            setFlash("Please choose a number from 1 to 6, or 0 to go back.");
            continue;
        }

        zeroMatrix(A, N);
        if (choice == 1) {
            n = 3;
            for (int i = 0; i < 3; i++) A[i][i] = 1.0;
            cout << "  Loaded Identity 3x3.\n";
        } else if (choice == 2) {
            n = 3;
            double raw[3][3] = {
                {1, 2, 3},
                {2, 3, 4},
                {3, 4, 5}
            };
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    A[i][j] = raw[i][j];
            cout << "  Loaded simple 3x3.\n";
        } else if (choice == 3) {
            n = 3;
            double raw[3][3] = {
                {1, 2, 3},
                {2, 4, 6},
                {1, 1, 1}
            };
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    A[i][j] = raw[i][j];
            cout << "  Loaded example with determinant 0.\n";
        } else if (choice == 4) {
            n = 3;
            double raw[3][3] = {
                {2, 1, 0},
                {1, 3, 4},
                {0, 4, 5}
            };
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    A[i][j] = raw[i][j];
            cout << "  Loaded symmetric 3x3.\n";
        } else if (choice == 5) {
            n = 2;
            A[0][0] = 2; A[0][1] = 1;
            A[1][0] = 5; A[1][1] = 3;
            cout << "  Loaded 2x2 [[2,1],[5,3]].\n";
        } else {
            n = 3;
            double raw[3][3] = {
                {1, 2, 3},
                {0, 1, 4},
                {5, 6, 0}
            };
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    A[i][j] = raw[i][j];
            cout << "  Loaded classic 3x3.\n";
        }
        printMatrix(A, n, n);
        storeLastMatrix(A, n, n, "Load sample");
        return;
    }
}

void opSaveResult() {
    if (!hasLastResult) {
        cout << "\n  Nothing to save yet. Run a calculation first.\n";
        return;
    }
    ofstream out("result.txt");
    if (!out) {
        cout << "\n  Could not save the file. Please try again later.\n";
        return;
    }
    out << fixed << setprecision(8);
    out << "==============================================\n";
    out << "  Matrix Calculator PF — Last Result\n";
    out << "==============================================\n\n";
    out << "  Last step : " << lastOpName << "\n";

    if (lastResultIsVector) {
        out << "\n  Result type : Number list (" << lastVectorLen << " entries)\n";
        writeVectorList(out, lastVector, lastVectorLen, "Entry");
    } else if (lastResultRows == 1 && lastResultCols == 1) {
        out << "\n  Result type : Single value\n";
        out << "  Value : " << lastResult[0][0] << "\n\n";
    } else {
        out << "\n  Result type : Matrix (" << lastResultRows << " x " << lastResultCols << ")\n";
        writeMatrixGrid(out, lastResult, lastResultRows, lastResultCols, 8);
    }

    out << "  Last determinant : " << lastDet << "\n";
    out << "\n==============================================\n";
    out.close();
    cout << "\n  Saved last result to result.txt\n";
    setOpName("Save result.txt");
}

void opHistory() {
    cout << "\n  Last step: " << lastOpName << "\n";
    if (hasLastResult) {
        if (lastResultIsVector) {
            cout << "  Last result (number list):\n";
            printVector(lastVector, lastVectorLen);
        } else if (lastResultRows == 1 && lastResultCols == 1) {
            cout << "  Last result (single value):\n";
            setColor(C_OK);
            cout << fixed << setprecision(6);
            cout << "  Value : " << lastResult[0][0] << "\n\n";
            cout << defaultfloat;
            setColor(C_RESET);
        } else {
            cout << "  Last result (matrix " << lastResultRows << " x " << lastResultCols << "):\n";
            printMatrix(lastResult, lastResultRows, lastResultCols);
        }
    } else {
        cout << "  No result saved yet.\n";
    }
}

/* ---------- main: clean loops, no goto ---------- */
int main() {
    enableConsoleUtf8();
    setupConsoleDisplay();

    bool running = true;
    while (running) {
        if (!welcomeScreen()) {
            running = false;
            break;
        }

        double A[N][N];
        zeroMatrix(A, N);
        int n = 0;

        /* size — 0 returns to welcome only */
        bool sizeOk = false;
        while (!sizeOk && running) {
            clearScreen();
            printFlashIfAny();
            setColor(C_TITLE);
            cout << "\n   MATRIX SIZE\n\n";
            setColor(C_HIGH);
            cout << "   Enter the size of the square matrix (1 to 10).\n";
            printBackHint();
            setColor(C_RESET);
            ReadResult rr = readIntInRange(
                "  Enter matrix size: ",
                1, N, n, true);
            if (rr == RR_CANCEL) {
                break; /* back to welcome — welcome clears itself */
            }
            if (rr == RR_OK)
                sizeOk = true;
            else
                setFlash("Please enter a size from 1 to 10, or 0 to go back.");
        }
        if (!sizeOk)
            continue;

        if (!inputMatrix(A, n, "A"))
            continue; /* back to welcome */

        bool sameMatrix = true;
        while (sameMatrix && running) {
            printMenu();
            string optTok;
            if (!(cin >> optTok)) {
                if (cin.eof()) {
                    running = false;
                    sameMatrix = false;
                    break;
                }
                cin.clear();
                cin.ignore(10000, '\n');
                setFlash("Please type a menu number from 1 to 19, or 0 to go back.");
                continue;
            }
            if (isCancelToken(optTok)) {
                sameMatrix = false;
                break; /* welcome only */
            }
            char* endp = nullptr;
            long optLong = strtol(optTok.c_str(), &endp, 10);
            if (endp == optTok.c_str() || *endp != '\0') {
                setFlash("That is not a valid menu choice. Type 1 to 19, or 0 to go back.");
                continue;
            }
            int option = (int)optLong;
            if (option < 1 || option > 19) {
                setFlash("That option is not on the list. Type 1 to 19, or 0 to go back.");
                continue;
            }

            clearScreen();
            switch (option) {
            case 1:  opDisplay(A, n); break;
            case 2:  opSymmetric(A, n); break;
            case 3:  opIdentity(A, n); break;
            case 4:  opDeterminant(A, n); break;
            case 5:  opAdjoint(A, n); break;
            case 6:  opInverse(A, n); break;
            case 7:  opSolve(A, n); break;
            case 8:
                clearScreen();
                cout << "\n  Goodbye. Thank you for using Matrix Calculator.\n\n";
                running = false;
                sameMatrix = false;
                break;
            case 9:  opTranspose(A, n); break;
            case 10: opAdd(A, n); break;
            case 11: opSub(A, n); break;
            case 12: opMul(A, n); break;
            case 13: opScalar(A, n); break;
            case 14: opTrace(A, n); break;
            case 15: opRank(A, n); break;
            case 16: opLoadSample(A, n); break;
            case 17: printBanner();
                     setOpName("Show about");
                     break;
            case 18: opSaveResult(); break;
            case 19: opHistory(); break;
            default:
                setFlash("That option is not on the list. Type 1 to 19, or 0 to go back.");
                continue;
            }

            if (!running || !sameMatrix)
                break;

            /* continue prompt — wrong answer re-asks this screen only */
            bool contAsked = false;
            while (!contAsked && running && sameMatrix) {
                setColor(C_DIM);
                cout << "\n  ---------------------------------------------------------------------\n";
                setColor(C_WARN);
                cout << "  What next?\n";
                setColor(C_HIGH);
                cout << "     Y  = same matrix — show menu again\n";
                cout << "     N  = new matrix (welcome)\n";
                cout << "     0  = exit program\n";
                setColor(C_PROMPT);
                cout << "  Enter your choice: ";
                setColor(C_RESET);
                string cont;
                if (!(cin >> cont)) {
                    if (cin.eof()) {
                        running = false;
                        sameMatrix = false;
                        contAsked = true;
                        break;
                    }
                    cin.clear();
                    cin.ignore(10000, '\n');
                    setColor(C_ERR);
                    cout << "  Please type Y, N, or 0.\n";
                    setColor(C_RESET);
                    continue;
                }
                string ct = toLowerCopy(cont);
                if (ct == "y" || ct == "yes") {
                    contAsked = true;
                    sameMatrix = true;
                } else if (ct == "n" || ct == "no") {
                    contAsked = true;
                    sameMatrix = false;
                } else if (ct == "0") {
                    clearScreen();
                    cout << "\n  Goodbye. Thank you for using Matrix Calculator.\n\n";
                    running = false;
                    sameMatrix = false;
                    contAsked = true;
                } else {
                    setColor(C_ERR);
                    cout << "  Wrong choice. Type Y, N, or 0.\n";
                    setColor(C_RESET);
                }
            }
        }
    }
    return 0;
}
