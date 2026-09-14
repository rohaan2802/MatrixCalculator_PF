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
#include <cstring>

using namespace std;

const int N = 10;
const double EPS = 1e-9;

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

void printMatrix(const double M[N][N], int rows, int cols, int prec = 6) {
    cout << fixed << setprecision(prec);
    for (int i = 0; i < rows; i++) {
        cout << "  |";
        for (int j = 0; j < cols; j++) {
            cout << setw(12) << M[i][j];
        }
        cout << "  |\n";
    }
    cout << defaultfloat;
}

void printVector(const double v[], int n, int prec = 6) {
    cout << fixed << setprecision(prec);
    for (int i = 0; i < n; i++)
        cout << "  x[" << (i + 1) << "] = " << setw(12) << v[i] << "\n";
    cout << defaultfloat;
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

bool readIntInRange(const char* prompt, int lo, int hi, int& out) {
    cout << prompt;
    if (!(cin >> out)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "  [Error] Invalid integer. Try again.\n";
        return false;
    }
    if (out < lo || out > hi) {
        cout << "  [Error] Value must be in [" << lo << " .. " << hi << "].\n";
        return false;
    }
    return true;
}

bool readDouble(const char* prompt, double& out) {
    cout << prompt;
    if (!(cin >> out)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "  [Error] Invalid number. Try again.\n";
        return false;
    }
    return true;
}

void inputMatrix(double A[N][N], int n) {
    cout << "\n========================================\n";
    cout << "  Enter " << (n * n) << " entries for A (" << n << "x" << n << ")\n";
    cout << "========================================\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double val;
            cout << "  A[" << (i + 1) << "," << (j + 1) << "] >> ";
            while (!(cin >> val)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "  [Error] Invalid number. Try again.\n";
                cout << "  A[" << (i + 1) << "," << (j + 1) << "] >> ";
            }
            A[i][j] = val;
        }
    }
}

void inputVector(double b[N], int n) {
    cout << "\n========================================\n";
    cout << "  Enter " << n << " entries for b (n x 1)\n";
    cout << "========================================\n";
    for (int i = 0; i < n; i++) {
        double val;
        cout << "  b[" << (i + 1) << "] >> ";
        while (!(cin >> val)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "  [Error] Invalid number. Try again.\n";
            cout << "  b[" << (i + 1) << "] >> ";
        }
        b[i] = val;
    }
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
void printBanner() {
    cout << "\n\n";
    cout << "  =====================================================================\n";
    cout << "  ||                                                                 ||\n";
    cout << "  ||              MATRIX CALCULATOR  PF  (Professional)              ||\n";
    cout << "  ||                                                                 ||\n";
    cout << "  ||         Square matrices up to " << N << " x " << N
         << "  |  Double precision GE          ||\n";
    cout << "  ||         Author: Mohammad Rohaan  ·  Roll 22I-2327                ||\n";
    cout << "  ||                                                                 ||\n";
    cout << "  =====================================================================\n\n";
}

void printMenu() {
    cout << "\n";
    cout << "  ---------------------------------------------------------------------\n";
    cout << "   Status\n";
    cout << "     Last operation : " << lastOpName << "\n";
    cout << "  ---------------------------------------------------------------------\n\n";

    cout << "   ========================  COURSE MENU (1-8)  ========================\n\n";
    cout << "     1.  Display matrix A\n";
    cout << "     2.  Symmetric check          (also prints transpose)\n";
    cout << "     3.  Identity check\n";
    cout << "     4.  Determinant              (Gaussian elimination)\n";
    cout << "     5.  Adjoint / adjugate\n";
    cout << "     6.  Inverse                  (double precision)\n";
    cout << "     7.  Solve linear system      Ax = b\n";
    cout << "     8.  Exit program\n\n";

    cout << "   ========================  EXTRA TOOLS (9-19)  ========================\n\n";
    cout << "     9.  Transpose of A\n";
    cout << "    10.  Matrix addition          A + B\n";
    cout << "    11.  Matrix subtraction       A - B\n";
    cout << "    12.  Matrix multiplication    A * B\n";
    cout << "    13.  Scalar multiplication    k * A\n";
    cout << "    14.  Trace of A\n";
    cout << "    15.  Rank of A                (Gaussian elimination)\n";
    cout << "    16.  Load sample matrices     (identity / singular / symmetric...)\n";
    cout << "    17.  Show banner / about\n";
    cout << "    18.  Save last result         -> result.txt\n";
    cout << "    19.  Show last operation history\n\n";

    cout << "  ---------------------------------------------------------------------\n";
    cout << "   Enter option number (1 to 19)\n";
    cout << "  > ";
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
    cout << "\n  Determinant (Gaussian elimination) = " << det << "\n";
    cout << "  (stored in lastDet = " << lastDet << ")\n";
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
        cout << "\n  Adjoint via cofactors (course style, n <= 3):\n";
        printMatrix(Adj, n, n);
        storeLastMatrix(Adj, n, n, "Adjoint (cofactors)");
    }

    double Adj2[N][N];
    if (adjointViaInverse(A, n, Adj2)) {
        cout << "\n  Adjoint via A^{-1} * det (n <= 10, invertible):\n";
        printMatrix(Adj2, n, n);
        if (n > 3)
            storeLastMatrix(Adj2, n, n, "Adjoint (Inv*det)");
        else {
            /* consistency note for small n */
            bool match = true;
            for (int i = 0; i < n && match; i++)
                for (int j = 0; j < n && match; j++)
                    if (fabs(Adj[i][j] - Adj2[i][j]) > 1e-6)
                        match = false;
            if (match)
                cout << "  [OK] Cofactor adjoint matches Inv*det.\n";
            else
                cout << "  [Note] Minor floating differences may appear.\n";
        }
    } else {
        if (n > 3) {
            cout << "\n  [Error] Matrix is singular (det ~ 0). Cannot form adjoint via Inv*det.\n";
            cout << "  For singular matrices, use cofactor method (supported for n <= 3).\n";
        } else {
            cout << "\n  Note: Matrix is singular; cofactor adjoint above is still valid.\n";
        }
    }
}

void opInverse(const double A[N][N], int n) {
    cout << "\n  --- Matrix A ---\n";
    printMatrix(A, n, n);
    double det = determinantGE(A, n);
    cout << fixed << setprecision(6);
    cout << "  det(A) = " << det << "\n";
    cout << defaultfloat;

    double Inv[N][N];
    if (!inverseGE(A, n, Inv)) {
        cout << "\n  [Error] Singular matrix (det = 0). Inverse does not exist.\n";
        return;
    }
    cout << "\n  Inverse A^{-1} (Gaussian elimination on [A|I]):\n";
    printMatrix(Inv, n, n);
    storeLastMatrix(Inv, n, n, "Inverse");
}

void opSolve(const double A[N][N], int n) {
    cout << "\n  --- Coefficient matrix A ---\n";
    printMatrix(A, n, n);
    double b[N], x[N];
    inputVector(b, n);
    cout << "\n  --- Vector b ---\n";
    for (int i = 0; i < n; i++)
        cout << "  | " << setw(12) << fixed << setprecision(6) << b[i] << " |\n";
    cout << defaultfloat;

    if (!solveGE(A, b, n, x)) {
        cout << "\n  [Error] No unique solution (singular / inconsistent system).\n";
        return;
    }
    cout << "\n  Solution x of Ax = b:\n";
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
    inputMatrix(B, n);
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
    inputMatrix(B, n);
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
    inputMatrix(B, n);
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
    double k;
    while (!readDouble("  Enter scalar k >> ", k)) { /* retry */ }
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
    cout << "\n  Rank(A) via Gaussian elimination = " << r << "\n";
    storeLastScalar((double)r, "Rank");
}

void opLoadSample(double A[N][N], int& n) {
    cout << "\n  Sample presets:\n";
    cout << "    1) Identity 3x3\n";
    cout << "    2) Hilbert-ish integers 3x3\n";
    cout << "    3) Singular 3x3\n";
    cout << "    4) Symmetric 3x3\n";
    cout << "    5) Classic 2x2  [[2,1],[5,3]]  det=1\n";
    cout << "    6) Classic 3x3  [[1,2,3],[0,1,4],[5,6,0]]\n";
    int choice;
    while (!readIntInRange("  Choose sample [1-6]: ", 1, 6, choice)) { /* retry */ }

    zeroMatrix(A, N);
    if (choice == 1) {
        n = 3;
        for (int i = 0; i < 3; i++) A[i][i] = 1.0;
        cout << "  Loaded Identity 3x3.\n";
    } else if (choice == 2) {
        n = 3;
        /* Hilbert-ish integers: H_ij ~ 1/(i+j+1) scaled */
        double raw[3][3] = {
            {1, 2, 3},
            {2, 3, 4},
            {3, 4, 5}
        };
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                A[i][j] = raw[i][j];
        cout << "  Loaded Hilbert-ish integer 3x3.\n";
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
        cout << "  Loaded singular 3x3 (row2 = 2*row1).\n";
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
}

void opSaveResult() {
    if (!hasLastResult) {
        cout << "\n  [Error] No result to save yet. Run an operation first.\n";
        return;
    }
    ofstream out("result.txt");
    if (!out) {
        cout << "\n  [Error] Could not open result.txt for writing.\n";
        return;
    }
    out << fixed << setprecision(8);
    out << "MatrixCalculator_PF — last result\n";
    out << "Operation: " << lastOpName << "\n";
    if (lastResultIsVector) {
        out << "Vector (" << lastVectorLen << "):\n";
        for (int i = 0; i < lastVectorLen; i++)
            out << lastVector[i] << "\n";
    } else {
        out << "Matrix " << lastResultRows << "x" << lastResultCols << ":\n";
        for (int i = 0; i < lastResultRows; i++) {
            for (int j = 0; j < lastResultCols; j++) {
                out << setw(14) << lastResult[i][j];
            }
            out << "\n";
        }
    }
    out << "lastDet = " << lastDet << "\n";
    out.close();
    cout << "\n  Saved last result to result.txt\n";
    setOpName("Save result.txt");
}

void opHistory() {
    cout << "\n  Last operation name: " << lastOpName << "\n";
    if (hasLastResult) {
        if (lastResultIsVector) {
            cout << "  Last result was a vector of length " << lastVectorLen << ":\n";
            printVector(lastVector, lastVectorLen);
        } else {
            cout << "  Last result was a " << lastResultRows << "x" << lastResultCols << " matrix:\n";
            printMatrix(lastResult, lastResultRows, lastResultCols);
        }
    } else {
        cout << "  (No numeric result stored yet.)\n";
    }
}

/* ---------- main: clean loops, no goto ---------- */
int main() {
    printBanner();

    bool running = true;
    while (running) {
        double A[N][N];
        zeroMatrix(A, N);
        int n = 0;

        /* size validation 1..10 */
        while (!readIntInRange(
            "\n  ENTER THE SIZE OF THE SQUARE MATRIX (1..10): ",
            1, N, n)) {
            /* keep asking */
        }

        inputMatrix(A, n);

        bool sameMatrix = true;
        while (sameMatrix && running) {
            printMenu();
            int option;
            if (!(cin >> option)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "  [Error] Please enter a number 1-19.\n";
                continue;
            }

            switch (option) {
            case 1:  opDisplay(A, n); break;
            case 2:  opSymmetric(A, n); break;
            case 3:  opIdentity(A, n); break;
            case 4:  opDeterminant(A, n); break;
            case 5:  opAdjoint(A, n); break;
            case 6:  opInverse(A, n); break;
            case 7:  opSolve(A, n); break;
            case 8:
                cout << "\n  Program terminated. Thank you.\n\n";
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
                     setOpName("Show banner");
                     break;
            case 18: opSaveResult(); break;
            case 19: opHistory(); break;
            default:
                cout << "  [Error] Invalid option. Choose 1-19.\n";
                break;
            }

            if (!running || !sameMatrix)
                break;

            /* continue prompt: Y same / N new / 0 exit */
            cout << "\n  ------------------------------------------------------------\n";
            cout << "  Continue?  Y = same matrix  |  N = new matrix  |  0 = exit\n";
            cout << "  >>>>>>> ";
            char ch;
            if (!(cin >> ch)) {
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }
            if (ch == 'Y' || ch == 'y') {
                sameMatrix = true;
            } else if (ch == 'N' || ch == 'n') {
                sameMatrix = false;
            } else if (ch == '0') {
                cout << "\n  Program terminated. Thank you.\n\n";
                running = false;
                sameMatrix = false;
            } else {
                cout << "  [Warn] Unrecognized input; staying with same matrix.\n";
                sameMatrix = true;
            }
        }
    }
    return 0;
}
