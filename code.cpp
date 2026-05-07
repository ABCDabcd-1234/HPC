#include <bits/stdc++.h>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

int main() {
    int N = 800; // matrix size (adjust if system is slow)

    vector<vector<double>> A(N, vector<double>(N));
    vector<vector<double>> B(N, vector<double>(N));
    vector<vector<double>> C_seq(N, vector<double>(N, 0));
    vector<vector<double>> C_par(N, vector<double>(N, 0));

    // Initialize matrices
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
        }
    }

    // ================= SEQUENTIAL =================
    auto start = high_resolution_clock::now();

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C_seq[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    auto end = high_resolution_clock::now();
    auto duration_seq = duration_cast<milliseconds>(end - start);
    cout << "Sequential Time: " << duration_seq.count() << " ms\n";

    // ================= PARALLEL =================
    start = high_resolution_clock::now();

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C_par[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    end = high_resolution_clock::now();
    auto duration_par = duration_cast<milliseconds>(end - start);
    cout << "Parallel Time:   " << duration_par.count() << " ms\n";

    // Speedup
    cout << "Speedup: " << (double)duration_seq.count() / duration_par.count() << "x\n";

    // Verify correctness (optional)
    bool correct = true;
    for (int i = 0; i < 5 && correct; i++) {
        for (int j = 0; j < 5 && correct; j++) {
            if (abs(C_seq[i][j] - C_par[i][j]) > 1e-6) {
                correct = false;
            }
        }
    }

    if (correct) cout << "Result Verified ✅\n";
    else cout << "Mismatch ❌\n";

    return 0;
}