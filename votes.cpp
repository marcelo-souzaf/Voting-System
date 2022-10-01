#include <chrono>
#include <ctime>
#include <random>

#include "include/voting.hpp"

using std::cout;
using std::endl;

int main() {
    cout << "Bytes da classe de voto: " << sizeof(Vote) << endl;
    const uint population_size = 1000000U;  // 1000000U
    const uint candidate_count = 20000U;  // 20000U
    const uint vote_count = 30U;  // 100000000U

    // Aleatoriza os votos com base no tempo atual
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint> user_distrib(1U, population_size);
    std::uniform_int_distribution<short> candidate_distrib(1, 5);
    std::uniform_int_distribution<int> state_distrib(0, 26);
    std::uniform_int_distribution<int> time_distrib(-5 * Time::DAY, 5 * Time::DAY);

    std::ofstream file;
    file.open("votes.csv");

    auto start = std::chrono::system_clock::now();
    try {
        System machine = System(population_size, candidate_count);

        // Cuidado para não passar de 100 milhões
        for (int i = 0; i < vote_count; ++i) {
            machine.vote(user_distrib(gen), candidate_distrib(gen),
                std::time(nullptr) + time_distrib(gen));
        }
        // Passou perto de esgotar minha RAM e salvou um arquivo de 4,6 GB
        for (int i = 0; i < vote_count; ++i) {
            file << machine.search(i);
        }
        machine.describe();
        // Vector<Vote> results = machine.sorted_by_date();
        // for (int i = 0; i < results.size(); i++){
        //     results[i].print();
        // }

        Date beginning = Date(std::time(nullptr));
        Date end = Date(std::time(nullptr) + 1000);

        // Vector<uint> counts = machine.votecount_by_date(beginning, end);
        // Vector<uint> id_count = machine.topK_candidates(10, beginning, end);
        // cout << "Top 10 candidatos: " << endl;
        // for (uint i = 0; i < id_count.size(); ++i) {
        //     cout << "ID: " << id_count[i++] << endl;
        //     cout << "Contagem: " << id_count[i] << endl;
        // }

        Vector<uint> counts = machine.votecount_by_date();
        for (uint i : counts) cout << i << endl;
        Vector<System::PairCount> id_count = machine.topK_sorted(3);

        file.close();
    } catch (std::exception& e) {
        cout << e.what() << endl;
        file.close();
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Tempo passado: " << diff.count() << std::endl;
    return 0;
}
