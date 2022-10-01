#ifndef TIME_MEASUREMENT_HPP_
#define TIME_MEASUREMENT_HPP_

#include <vector>
#include <chrono>
#include <ctime>
#include <random>
#include "./voting.hpp"

using std::vector;

// Função genérica que retorna o tempo de execução de uma função
vector<double> measureTime (Roberto* machine, int max, int step, void (*function)(Roberto*, int)) {
    vector<double> times;
    // até o máximo de iterações
    for (int i = 0; i < max; i++) {
        auto start = std::chrono::system_clock::now();

        // executa a função com o número de iterações especificado
        function(machine, step);

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = end - start;
        times.push_back(diff.count());
    }
    return times;
}

// Função para medição de tempo de inserção - Tempo esperado O(1)
void insertNVotes (Roberto* machine, int n) {
    // Aleatoriza os votos com base no tempo atual
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint> user_distrib(1U, 1000000U);
    std::uniform_int_distribution<short> candidate_distrib(1, 20000);
    std::uniform_int_distribution<int> time_distrib(-5 * Time::DAY, 5 * Time::DAY);


    for (int i = 0; i < n; ++i) {
        machine->vote(user_distrib(gen), candidate_distrib(gen),
                std::time(nullptr) + time_distrib(gen));
    }
}

// Função para medição de tempo de ordenação - Tempo esperado O(n log n)
void sortVotes (Roberto* machine, int n) {
    insertNVotes(machine, n);
    machine->sorted_by_date();
}

#endif  // TIME_MEASUREMENT_HPP_
