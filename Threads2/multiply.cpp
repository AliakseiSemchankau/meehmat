#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <math.h>
#include <vector>
#include <ctime>
#include <chrono>
#include <random>

long double integral = 0.0;
std::mutex answer_mutex;

long double func(long double x) {
    return sin(x);
}

std::vector<std::vector<int64_t>> generate_matrix(int64_t sz, int64_t mod) {
    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(0, mod - 1); // guaranteed unbiased
    auto random_integer = uni(rng);

    std::vector<std::vector<int64_t>> matrix(sz, std::vector<int64_t>(sz));
    for (int i = 0; i < sz; ++i) {
        for (int j = 0; j < sz; ++j) {
            matrix[i][j] = uni(rng) % mod;
        }
    }

    return matrix;
}

void multiply_function(int l1, int l2, int r1, int r2, int64_t mod,
                        const std::vector<std::vector<int64_t>>& fmatrix,
                        const std::vector<std::vector<int64_t>>& smatrix,
                        std::vector<std::vector<int64_t>>& tmatrix) {
    int sz = static_cast<int>(fmatrix.size());

    for (int i = l1; i < l2; ++i) {
        for (int j = r1; j < r2; ++j) {
            for (int k = 0; k < sz; ++k) {
                tmatrix[i][j] += fmatrix[i][k] * smatrix[j][k];
                tmatrix[i][j] %= mod;
            }
        }
    }
}

int main() {
    std::cout << std::setprecision(15);
    std::cout << std::fixed;
    
    int64_t mod = 5;
    int64_t sz = 600;

    auto fmatrix = generate_matrix(sz, mod);
    auto smatrix = generate_matrix(sz, mod);
    std::vector<std::vector<int64_t>> basematrix(sz, std::vector<int64_t>(sz, 0));
    multiply_function(0, sz, 0, sz, mod, fmatrix, smatrix, basematrix);

    for (int thread_count = 1; thread_count <= 4; ++thread_count) {
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        std::vector<std::vector<int64_t>> tmatrix(sz, std::vector<int64_t>(sz, 0));
        int delta = sz / thread_count;
        std::vector<std::unique_ptr<std::thread>> threads;
        for (int l1 = 0; l1 < sz; l1 += delta) {
            for (int r1 = 0; r1 < sz; r1 += delta) {
                int l2 = l1 + delta;
                int r2 = r1 + delta;

                threads.push_back(std::unique_ptr<std::thread>(new std::thread(multiply_function,
                                                                               l1, l2,
                                                                               r1, r2,
                                                                               mod,
                                                                               std::ref(fmatrix),
                                                                               std::ref(smatrix),
                                                                               std::ref(tmatrix))));
            }
        }
        
        for (auto& thread : threads) {
            thread->join();
        }

        std::cout << "threads = " << thread_count*thread_count << "\n";
        std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start);
        std::cout << "time taken = " << time_span.count() << "\n";
        if (tmatrix != basematrix) {
            std::cout << "result is wrong:(\n";
        } 

    }
       

    return 0;

}
