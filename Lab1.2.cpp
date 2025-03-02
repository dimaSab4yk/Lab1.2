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

void mirrorRightHalfParallel(vector<vector<int>>& matrix, int n, int numThreads)
{
    vector<thread> threads;
    for (int i = 0; i < n; ++i)
    {
        threads.push_back(thread(mirrorRow, ref(matrix), i, n));

        if (threads.size() >= numThreads)
        {
            for (auto& t : threads)
                t.join();
            threads.clear();
        }
    }

    for (auto& t : threads)
    {
        t.join();
    }
}

int main()
{
    srand(time(0));

    int n = 6;  
    vector<vector<int>> matrix(n, vector<int>(n));

    unsigned int logicalCores = thread::hardware_concurrency();
    unsigned int physicalCores = logicalCores / 2; 

    cout << "Logical cores: " << logicalCores << endl;
    cout << "Physical cores: " << physicalCores << endl;

    for (unsigned int numThreads : {physicalCores / 2, physicalCores, logicalCores, 2 * logicalCores, 4 * logicalCores, 8 * logicalCores, 16 * logicalCores})
    {
        auto start = high_resolution_clock::now();

        fillMatrix(matrix, n);
        mirrorRightHalfParallel(matrix, n, numThreads);

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(end - start).count();
        double seconds = duration * 1e-9;

        cout << "Threads: " << numThreads << " - Time: " << fixed << seconds << " seconds" << endl;
    }

    return 0;
}
