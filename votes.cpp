#include <chrono>
#include <ctime>
#include <random>

#include "./voting.hpp"

using std::cout;
using std::endl;

int main() {
    cout << "Bytes da classe de voto: " << sizeof(Vote) << endl;

    // Aleatoriza os votos com base no tempo atual
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint> user_distrib(1U, 1'000'000U);
    std::uniform_int_distribution<short> candidate_distrib(0, 5000);
    std::uniform_int_distribution<int> state_distrib(0, 26);
    std::uniform_int_distribution<int> time_distrib(-5 * Time::DAY, 5 * Time::DAY);

    std::ofstream file;
    file.open("votes.csv");

    auto start = std::chrono::system_clock::now();
    try {
        Roberto machine = Roberto();

        // Cuidado para não passar de 100 milhões
        const int number_of_votes = 100000;
        for (int i = 0; i < number_of_votes; ++i) {
            machine.vote(user_distrib(gen), candidate_distrib(gen),
                std::time(nullptr) + time_distrib(gen));
        }
        // Passou perto de esgotar minha RAM e salvou um arquivo de 4,6 GB
        for (int i = 0; i < number_of_votes; ++i) {
            file << machine.search(i);
        }
        machine.describe();

        Date beginning = Date(std::time(nullptr));
        Date end = Date(std::time(nullptr)+1000);

        uint* counts = machine.votecount_by_date(beginning, end);
        short* candidates = machine.topK_candidates(10, beginning, end);
        cout << "Top 10 candidatos: " << endl;
        for (int i = 0; i < 10; i++) {
            cout << "ID: " << candidates[i] << endl;
            cout << "Contagem: " << counts[candidates[i]] << endl;
        }


        counts = machine.votecount_by_date();
        candidates = machine.topK_candidates(10);
        cout << "Top 10 candidatos: " << endl;
        for (int i = 0; i < 10; i++) {
            cout << "ID: " << candidates[i] << endl;
            cout << "Contagem: " << counts[candidates[i]] << endl;
        }

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
