#include "include/matplotlibcpp.h"
#include "include/voting.hpp"

#include <chrono>
#include <ctime>
#include <random>
#include <vector>  // Obrigatório para o matplotlib

namespace plt = matplotlibcpp;
using std::cout;
using std::endl;
using std::vector;

int main() {
    const int number_of_votes = 100000000;
    const int number_of_users = 1000000;
    const int number_of_candidates = 20000;
    const int batch_size = 1000000;
    vector<double> n;
    n.reserve(number_of_votes / batch_size);
    for (int i = 1; i <= number_of_votes / batch_size; ++i) {
        n.push_back(i * batch_size);
    }

    // Aleatoriza os votos com base no tempo atual
    std::random_device rd;
    std::mt19937 gen(rd());
    // IDs válidos variam de 1 ao máximo definido na classe
    std::uniform_int_distribution<uint> user_distrib(1, number_of_users);
    std::uniform_int_distribution<short> candidate_distrib(1, number_of_candidates);
    // Acrescenta desvio de até 10 dias do horário atual
    std::uniform_int_distribution<int> time_distrib(-10 * Time::DAY, 10 * Time::DAY);

    // 100 milhões de votos, 1 milhão de eleitores e 20 mil candidatos
    System machine = System(number_of_users, number_of_candidates);

    vector<double> insertion_times;
    insertion_times.reserve(n.size());
    vector<double> sorting_times;
    sorting_times.reserve(n.size());
    vector<double> selection_times;
    selection_times.reserve(n.size());
    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    
    for (int j = 0; j < n.size(); ++j) {
        start = std::chrono::system_clock::now();
        for (int i = 0; i < batch_size; ++i) {
            machine.vote(user_distrib(gen), candidate_distrib(gen),
                    std::time(nullptr) + time_distrib(gen));
        }
        end = std::chrono::system_clock::now();
        insertion_times.push_back(diff.count());

        start = std::chrono::system_clock::now();
        machine.sorted_by_date();
        end = std::chrono::system_clock::now();
        diff = end - start;
        sorting_times.push_back(diff.count());

        start = std::chrono::system_clock::now();
        machine.topK_sorted(10);
        end = std::chrono::system_clock::now();
        diff = end - start;
        selection_times.push_back(diff.count());
    }
    plt::plot(n, insertion_times);
    plt::show();
    plt::plot(n, sorting_times);
    plt::show();
    plt::plot(n, selection_times);
    plt::show();

    return 0;
}
