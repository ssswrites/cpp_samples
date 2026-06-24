#include <iostream>
#include <vector>
#include <future>
#include <numeric>
#include <chrono>

// A heavy, CPU-bound workload function
long long computeSquareSum(int start, int end) {
    long long sum = 0;
    for (int i = start; i < end; ++i) {
        sum += (static_cast<long long>(i) * i);
    }
    return sum;
}

int main() {
    const int numElements = 100'000'000;
    const int midPoint = numElements / 2;

    auto startTime = std::chrono::high_resolution_clock::now();

    // Launch Process/Task 1 asynchronously (forces it onto a separate CPU core if available)
    std::future<long long> worker1 = std::async(std::launch::async, computeSquareSum, 0, midPoint);

    // Launch Process/Task 2 on the main core simultaneously 
    std::future<long long> worker2 = std::async(std::launch::async, computeSquareSum, midPoint, numElements);

    // .get() blocks the main thread until each independent core finishes its math
    long long totalSum = worker1.get() + worker2.get();

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = endTime - startTime;

    std::cout << "Parallel Calculation Result: " << totalSum << std::endl;
    std::cout << "Execution time: " << duration.count() << " ms" << std::endl;

    return 0;
}
