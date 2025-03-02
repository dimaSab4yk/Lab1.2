#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread> 

using namespace std;
using namespace std::chrono;

void fillMatrix(vector<vector<int>>& matrix, int n)
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            matrix[i][j] = rand() % 100;
        }
    }
}

void mirrorRow(vector<vector<int>>& matrix, int i, int n)
{
    for (int j = n / 2; j < n; ++j)
    {
        matrix[i][n - j - 1] = matrix[i][j];
    }
}

void mirrorRightHalfParallel(vector<vector<int>>& matrix, int n)
{
    vector<thread> threads;
    for (int i = 0; i < n; ++i)
    {
        threads.push_back(thread(mirrorRow, ref(matrix), i, n));
    }

    for (auto& t : threads)
    {
        t.join();
    }
}

void printMatrix(const vector<vector<int>>& matrix, int n)
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    srand(time(0));

    int n = 6;  
    vector<vector<int>> matrix(n, vector<int>(n));

    auto start = high_resolution_clock::now();

    fillMatrix(matrix, n);

    cout << "Matrix before modification:" << endl;
    printMatrix(matrix, n);

    mirrorRightHalfParallel(matrix, n);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(end - start).count();
    double seconds = duration * 1e-9;

    cout << "\nModified matrix:" << endl;
    printMatrix(matrix, n);

    cout << "\nWork time with parallelization: " << fixed << seconds << " seconds" << endl;

    return 0;
}

