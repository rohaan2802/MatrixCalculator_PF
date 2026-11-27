#define MATRIX_CALCULATOR_NO_MAIN
#include "../src/matrix_calculator.cpp"
#include <stdexcept>
#include <limits>

int checks = 0;
void check(bool condition, const string& label) {
    if (!condition) throw runtime_error(label);
    checks++;
    cout << "PASS " << label << "\n";
}
bool nearValue(double actual, double expected, double tolerance = 1e-8) {
    return fabs(actual - expected) <= tolerance * max(1.0, fabs(expected));
}
double inverseResidual(const double A[N][N], const double inverse[N][N], int n) {
    double residual = 0;
    for (int row = 0; row < n; row++)
        for (int col = 0; col < n; col++) {
            double value = 0;
            for (int k = 0; k < n; k++) value += A[row][k] * inverse[k][col];
            residual = max(residual, fabs(value - (row == col ? 1.0 : 0.0)));
        }
    return residual;
}
ReadResult numericToken(const string& token, double& value) {
    istringstream input(token);
    ostringstream output;
    auto* savedInput = cin.rdbuf(input.rdbuf());
    auto* savedOutput = cout.rdbuf(output.rdbuf());
    cin.clear();
    ReadResult result = readDouble("", value);
    cin.rdbuf(savedInput);
    cout.rdbuf(savedOutput);
    cin.clear();
    return result;
}
int main() {
    try {
        double A[N][N] = {{4, 7}, {2, 6}};
        double original[N][N] = {};
        copyMatrix(A, original, 2);
        check(nearValue(determinantGE(A, 2), 10), "2x2 determinant");
        double inverse[N][N] = {};
        check(inverseGE(A, 2, inverse), "invertible matrix accepted");
        check(inverseResidual(A, inverse, 2) < 1e-12, "inverse residual");
        check(A[0][0] == original[0][0] && A[1][1] == original[1][1], "working matrix preserved");
        double b[N] = {18, 14}, x[N] = {};
        check(solveGE(A, b, 2, x), "unique system accepted");
        check(nearValue(x[0], 1) && nearValue(x[1], 2), "known solution");
        double pivot[N][N] = {{0, 2}, {3, 4}};
        check(nearValue(determinantGE(pivot, 2), -6), "row-swap determinant sign");
        check(inverseGE(pivot, 2, inverse) && inverseResidual(pivot, inverse, 2) < 1e-12, "zero first pivot handled");
        double singular[N][N] = {{1, 2}, {2, 4}};
        check(!inverseGE(singular, 2, inverse), "singular inverse rejected");
        check(!solveGE(singular, b, 2, x), "nonunique system rejected");
        check(rankGE(singular, 2) == 1, "dependent rows rank");
        double zero[N][N] = {};
        check(rankGE(zero, 3) == 0, "zero matrix rank");
        double tiny[N][N] = {{1e-12, 0}, {0, 2e-12}};
        check(fabs(determinantGE(tiny, 2) - 2e-24) < 1e-35, "small-scale determinant retained");
        check(inverseGE(tiny, 2, inverse) && inverseResidual(tiny, inverse, 2) < 1e-12, "small-scale inverse retained");
        check(rankGE(tiny, 2) == 2, "small-scale rank");
        double mixed[N][N] = {{1e12, 0}, {0, 1e-12}};
        check(inverseGE(mixed, 2, inverse) && inverseResidual(mixed, inverse, 2) < 1e-12, "different row units");
        double balanced[N][N] = {{1e-200,0,0,0},{0,1e-200,0,0},{0,0,1e200,0},{0,0,0,1e200}};
        check(nearValue(determinantGE(balanced, 4), 1), "balanced extreme-scale determinant");
        double adj[N][N] = {};
        check(adjointViaInverse(tiny, 2, adj) && fabs(adj[0][0] - 2e-12) < 1e-23, "small determinant adjugate");
        double singular4[N][N] = {{1,0,0,0},{0,2,0,0},{0,0,3,0},{0,0,0,0}};
        lastDet = 7.0;
        check(adjointCofactors(singular4, 4, adj) && nearValue(adj[3][3], 6), "singular 4x4 adjugate");
        check(lastDet == 7.0, "minor calculation preserves determinant history");
        double diagonal[N][N] = {};
        for (int i = 0; i < N; i++) diagonal[i][i] = i + 1;
        check(nearValue(determinantGE(diagonal, N), 3628800), "maximum 10x10 determinant");
        check(inverseGE(diagonal, N, inverse) && inverseResidual(diagonal, inverse, N) < 1e-12, "maximum 10x10 inverse");
        double value = 5;
        for (const string token : {"nan", "inf", "-inf", "1e999", "abc"})
            check(numericToken(token, value) == RR_RETRY && value == 5, "reject token " + token);
        check(numericToken("-2.5e-3", value) == RR_OK && nearValue(value, -0.0025), "scientific input");
        check(numericToken("5e-324", value) == RR_OK && value > 0, "representable subnormal input");
        check(formatNumber(1e-12, 6).find("e-12") != string::npos, "tiny-value formatting");
        {
            istringstream input("4294967297");
            ostringstream output;
            auto* savedInput = cin.rdbuf(input.rdbuf());
            auto* savedOutput = cout.rdbuf(output.rdbuf());
            int number = 5;
            ReadResult result = readIntInRange("", 1, N, number);
            cin.rdbuf(savedInput);
            cout.rdbuf(savedOutput);
            cin.clear();
            check(result == RR_RETRY && number == 5, "integer range checked before narrowing");
        }
        storeLastMatrix(A, 2, 2, "Transpose");
        {
            ostringstream ignored;
            auto* saved = cout.rdbuf(ignored.rdbuf());
            storeLastScalar(numeric_limits<double>::infinity(), "Overflow");
            cout.rdbuf(saved);
            check(lastResultName == "Transpose", "nonfinite result preserves stored calculation");
        }
        setOpName("Show about");
        ostringstream output;
        auto* saved = cout.rdbuf(output.rdbuf());
        opSaveResult();
        cout.rdbuf(saved);
        ifstream report("result.txt");
        string reportText((istreambuf_iterator<char>(report)), istreambuf_iterator<char>());
        check(reportText.find("Result operation : Transpose") != string::npos, "save labels actual result operation");
        cout << "CHECKS " << checks << " PASSED\n";
    } catch (const exception& error) {
        cerr << "FAIL " << error.what() << "\n";
        return 1;
    }
}
