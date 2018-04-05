#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <math.h>
#include <vector>
#include <ctime>
#include <chrono>

long double integral = 0.0;
std::mutex answer_mutex;
int64_t iterations = 12000000;

long double func(long double x) {
    return sin(x);
}

void integrate_function(long double l, long double r, int64_t iters) {
    long double result = 0.0;
    for (int i = 0; i < iters; ++i) {
        result += func(r * static_cast<long double>(i)/iters + l * static_cast<long double>(iters - i)/iters);
    }
    result /= iters;
    std::lock_guard<std::mutex> lock(answer_mutex);
    integral += result * (r - l);
}

int main() {
    std::cout << std::setprecision(15);
    std::cout << std::fixed;
    long double l = 0.0;
    long double r = 10.0;

    for (int thread_count = 1; thread_count <= 4; ++thread_count) {
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        integral = 0.0;
        std::vector<std::unique_ptr<std::thread>> threads;
        for (int thread_id = 0; thread_id < thread_count; ++thread_id) {
            long double current_l = (r * thread_id + l * (thread_count - thread_id)) / thread_count;
            long double current_r = (r * (thread_id + 1) + l * (thread_count - thread_id - 1)) / thread_count;
            threads.push_back(std::unique_ptr<std::thread>(new std::thread(integrate_function,
                                                                           current_l,
                                                                           current_r,
                                                                           iterations / thread_count)));
        }

        for (auto& thread : threads) {
            thread->join();
        }

        std::cout << "threads = " << thread_count << "\n";
        std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start);
        std::cerr << "time taken = " << time_span.count() << "\n";
        std::cout << "integral = " << integral << "\n\n\n\n";
    }


}