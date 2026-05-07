#include <bits/stdc++.h>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

// ===================== BUBBLE SORT =====================

// Sequential Bubble Sort
void sequentialBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ✅ Correct Parallel Bubble Sort (Odd-Even Transposition)
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n; i++) {

        // Even phase
        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }

        // Odd phase
        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ===================== MERGE SORT =====================

// Merge function
vector<int> mergeVectors(vector<int>& left, vector<int>& right) {
    vector<int> result;
    int i = 0, j = 0;

    while (i < left.size() && j < right.size()) {
        if (left[i] <= right[j]) {
            result.push_back(left[i++]);
        } else {
            result.push_back(right[j++]);
        }
    }

    while (i < left.size()) result.push_back(left[i++]);
    while (j < right.size()) result.push_back(right[j++]);

    return result;
}

// Sequential Merge Sort
vector<int> sequentialMergeSort(vector<int> arr) {
    if (arr.size() <= 1) return arr;

    int mid = arr.size() / 2;

    vector<int> left(arr.begin(), arr.begin() + mid);
    vector<int> right(arr.begin() + mid, arr.end());

    left = sequentialMergeSort(left);
    right = sequentialMergeSort(right);

    return mergeVectors(left, right);
}

// Parallel Merge Sort
vector<int> parallelMergeSort(vector<int> arr) {
    // Threshold to avoid too many threads
    if (arr.size() <= 1000)
        return sequentialMergeSort(arr);

    int mid = arr.size() / 2;

    vector<int> left(arr.begin(), arr.begin() + mid);
    vector<int> right(arr.begin() + mid, arr.end());

    #pragma omp parallel sections
    {
        #pragma omp section
        left = parallelMergeSort(left);

        #pragma omp section
        right = parallelMergeSort(right);
    }

    return mergeVectors(left, right);
}

// ===================== MAIN =====================

int main() {
    int n = 100000;

    // Generate SAME input for fair comparison
    vector<int> base(n);
    for (int i = 0; i < n; i++) base[i] = rand() % 1000;

    // ---------------- BUBBLE SORT ----------------
    cout << "=== BUBBLE SORT ===\n";

    vector<int> arr1 = base;
    vector<int> arr2 = base;

    auto start = high_resolution_clock::now();
    sequentialBubbleSort(arr1);
    auto end = high_resolution_clock::now();
    auto duration1 = duration_cast<milliseconds>(end - start);
    cout << "Sequential: " << duration1.count() << " ms\n";

    start = high_resolution_clock::now();
    parallelBubbleSort(arr2);
    end = high_resolution_clock::now();
    auto duration2 = duration_cast<milliseconds>(end - start);
    cout << "Parallel:   " << duration2.count() << " ms\n";

    if (duration2.count() > 0)
        cout << "Speedup: " << (double)duration1.count() / duration2.count() << "x\n\n";

    // ---------------- MERGE SORT ----------------
    cout << "=== MERGE SORT ===\n";

    vector<int> arr3 = base;
    vector<int> arr4 = base;

    start = high_resolution_clock::now();
    arr3 = sequentialMergeSort(arr3);
    end = high_resolution_clock::now();
    duration1 = duration_cast<milliseconds>(end - start);
    cout << "Sequential: " << duration1.count() << " ms\n";

    start = high_resolution_clock::now();
    arr4 = parallelMergeSort(arr4);
    end = high_resolution_clock::now();
    duration2 = duration_cast<milliseconds>(end - start);
    cout << "Parallel:   " << duration2.count() << " ms\n";

    if (duration2.count() > 0)
        cout << "Speedup: " << (double)duration1.count() / duration2.count() << "x\n";

    return 0;
}