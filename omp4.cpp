#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <omp.h>

using namespace std;

const int MAX_VAL = 100; // Maximum value for histogram range
const int N = 12;        // Number of elements

void histogram_sort(int arr[], int size) {
    int histogram[MAX_VAL] = {0};

    // Compute histogram in parallel
    #pragma omp parallel for
    for (int i = 0; i < size; i++) {
        #pragma omp atomic
        histogram[arr[i]]++;
    }

    // Reconstruct sorted array
    int sorted_arr[N];  // Local copy to avoid conflicts

    #pragma omp parallel
    {
        int local_index = 0;

        for (int i = 0; i < MAX_VAL; i++) {
            for (int j = 0; j < histogram[i]; j++) {
                sorted_arr[local_index++] = i;
            }
        }
    }

    // Copy back to original array
    for (int i = 0; i < size; i++) {
        arr[i] = sorted_arr[i];
    }
}

int main() {
    int data[N] = {42, 25, 18, 42, 10, 18, 25, 42, 10, 5, 5, 5};

    cout << "Before Sorting: ";
    for (int i = 0; i < N; i++) cout << data[i] << " ";
    cout << endl;

    histogram_sort(data, N);

    cout << "After Sorting: ";
    for (int i = 0; i < N; i++) cout << data[i] << " ";
    cout << endl;

    return 0;
}

// Before Sorting: 42 25 18 42 10 18 25 42 10 5 5 5 
// After Sorting: 5 5 5 10 10 18 18 25 25 42 42 42 
