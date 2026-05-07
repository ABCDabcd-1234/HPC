#include<bits/stdc++.h>
#include<chrono>
#include<omp.h>
using namespace std;
using namespace std::chrono;

int main(){
    int n = 10000000; // safer size for lab
    vector<int> arr(n);

    srand(time(0));

    // Generate random array
    for(int i = 0; i < n; i++){
        arr[i] = rand() % 10000;
    }

    // ===== Sequential Min =====
    auto start = high_resolution_clock::now();
    int min_seq = arr[0];
    for(int i = 1; i < n; i++){
        min_seq = min(min_seq, arr[i]);
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Sequential Min: " << min_seq << " Time: " << duration.count() << " ms\n";

    // ===== Parallel Min =====
    start = high_resolution_clock::now();
    int min_par = INT_MAX;
    #pragma omp parallel for reduction(min:min_par)
    for(int i = 0; i < n; i++){
        min_par = min(min_par, arr[i]);
    }
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "Parallel Min:   " << min_par << " Time: " << duration.count() << " ms\n\n";

    // ===== Sequential Max =====
    start = high_resolution_clock::now();
    int max_seq = arr[0];
    for(int i = 1; i < n; i++){
        max_seq = max(max_seq, arr[i]);
    }
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "Sequential Max: " << max_seq << " Time: " << duration.count() << " ms\n";

    // ===== Parallel Max =====
    start = high_resolution_clock::now();
    int max_par = INT_MIN;
    #pragma omp parallel for reduction(max:max_par)
    for(int i = 0; i < n; i++){
        max_par = max(max_par, arr[i]);
    }
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "Parallel Max:   " << max_par << " Time: " << duration.count() << " ms\n\n";

    // ===== Sequential Sum =====
    start = high_resolution_clock::now();
    long long sum_seq = 0;
    for(int i = 0; i < n; i++){
        sum_seq += arr[i];
    }
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "Sequential Sum: " << sum_seq << " Time: " << duration.count() << " ms\n";

    // ===== Parallel Sum =====
    start = high_resolution_clock::now();
    long long sum_par = 0;
    #pragma omp parallel for reduction(+:sum_par)
    for(int i = 0; i < n; i++){
        sum_par += arr[i];
    }
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "Parallel Sum:   " << sum_par << " Time: " << duration.count() << " ms\n\n";

    // ===== Sequential Average =====
    start = high_resolution_clock::now();
    long long sum_seq_avg = 0;
    for(int i = 0; i < n; i++){
        sum_seq_avg += arr[i];
    }
    double avg_seq = (double)sum_seq_avg / n;
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "Sequential Avg: " << avg_seq << " Time: " << duration.count() << " ms\n";

    // ===== Parallel Average =====
    start = high_resolution_clock::now();
    long long sum_par_avg = 0;
    #pragma omp parallel for reduction(+:sum_par_avg)
    for(int i = 0; i < n; i++){
        sum_par_avg += arr[i];
    }
    double avg_par = (double)sum_par_avg / n;
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "Parallel Avg:   " << avg_par << " Time: " << duration.count() << " ms\n";

    return 0;
}