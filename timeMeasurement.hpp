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
    const char states[27][3] = {"AC", "AL", "AP", "AM", "BA", "CE", "DF",
                                "ES", "GO", "MA", "MT", "MS", "MG", "PA",
                                "PB", "PR", "PE", "PI", "RJ", "RN", "RS",
                                "RO", "RR", "SC", "SP", "SE", "TO"};
    // Aleatoriza os votos com base no tempo atual
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint> user_distrib(1U, 10'000'000U);
    std::uniform_int_distribution<short> candidate_distrib(0, 1000);
    std::uniform_int_distribution<int> state_distrib(0, 26);
    std::uniform_int_distribution<int> time_distrib(-5 * Time::DAY, 5 * Time::DAY);


    for (int i = 0; i < n; i++) {
        machine->vote(user_distrib(gen), candidate_distrib(gen),
                states[state_distrib(gen)], std::time(nullptr) + time_distrib(gen));
    }
}

// Função para medição de tempo de ordenação - Tempo esperado O(n log n)
void sortVotes (Roberto* machine, int n) {
    insertNVotes(machine, n);
    machine->sorted_by_data();
}

// Função para medição de tempo de seleção - Tempo esperado O(n) (Não garantido no pior caso)
void topK (Roberto* machine, int k) {
    insertNVotes(machine, k);
    machine->topK_sorted(10);
}