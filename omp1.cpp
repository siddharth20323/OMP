
#include <iostream>
#include <omp.h>

using namespace std;

const int n = 3; // Define the size of the matrix

void matrix_mul(int a[n][n], int b[n][n], int c[n][n], int num_threads) {
     #pragma omp_set_num_threads(num_threads); // Set number of threads

    #pragma omp parallel for collapse(2)
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            c[i][j] = 0; // Initialize result matrix element to 0
            for(int k = 0; k < n; k++) {
                c[i][j] += a[i][k] * b[k][j]; // Matrix multiplication
            }
        }
    }
}

int main() {
    int num_threads = 4; // Number of threads
    int a[n][n] = {{2, 4, 6}, {4, 3, 2}, {9, 7, 8}}; // Matrix A
    int b[n][n] = {{2, 42, 6}, {44, 3, 2}, {9, 71, 8}}; // Matrix B
    int c[n][n] = {0}; // Result matrix initialized to zero

    matrix_mul(a, b, c, num_threads); // Call the matrix multiplication function

    cout << "FINAL result is:" << endl;
    for(int i = 0; i < n; i++) { // Print the resulting matrix
        for(int j = 0; j < n; j++) {
            cout << c[i][j] << " "; // Output each element in the matrix
        }
        cout << endl;
    }

    return 0;
}
FINAL result is:
234 522 68 
158 319 46 
398 967 132 
