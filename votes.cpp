#include <chrono>
#include <ctime>
#include <random>

#include "./voting.hpp"

using std::cout;
using std::endl;

int main() {
    cout << "Bytes da classe de voto: " << sizeof(Vote) << endl;
    const char states[27][3] = {"AC", "AL", "AP", "AM", "BA", "CE", "DF",
                                "ES", "GO", "MA", "MT", "MS", "MG", "PA",
                                "PB", "PR", "PE", "PI", "RJ", "RN", "RS",
                                "RO", "RR", "SC", "SP", "SE", "TO"};


    // Aleatoriza os votos com base no tempo atual
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint> user_distrib(1U, 10'000'000U);
    std::uniform_int_distribution<short> candidate_distrib(0, 5000);
    std::uniform_int_distribution<int> state_distrib(0, 26);
    std::uniform_int_distribution<int> time_distrib(-5 * Time::DAY, 5 * Time::DAY);

    std::ofstream file;
    file.open("votes.csv");

    auto start = std::chrono::system_clock::now();
    try {
        Roberto machine = Roberto();
        // Vetor<int> votes = Vetor<int>(32);
        // votes.push_back(machine.vote(1, 1, "SP"));
        // votes.push_back(machine.vote(2, 1, "MG"));
        // votes.push_back(machine.vote(3, 2, "RJ"));
        // votes.push_back(machine.vote(1, 2, "SP"));
        // votes.push_back(machine.vote(2, 2, "MG"));
        // votes.push_back(machine.vote(3, 3, "RJ"));
        // votes.push_back(machine.vote(4, 1, "AC"));
        // votes.push_back(machine.vote(5, 1, "AM"));
        // votes.push_back(machine.vote(6, 1, "AP"));
        // machine.remove(3);
        // votes.push_back(machine.vote(7, 1, "BA"));
        // for (uint i = 0; i < votes.size(); ++i) {
        //     machine.search(votes[i]).print();
        //     cout << endl;
        // }

        // Cuidado para não passar de 100 milhões
        const int number_of_votes = 100000;
        for (int i = 0; i < number_of_votes; ++i) {
            machine.vote(user_distrib(gen), candidate_distrib(gen),
                states[state_distrib(gen)], std::time(nullptr) + time_distrib(gen));
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

int main2() {

    Roberto machine = Roberto();
    machine.vote(1, 1, "SP", std::time(nullptr)+3600);
    machine.vote(2, 1, "MG", std::time(nullptr));
    machine.vote(3, 2, "RJ", std::time(nullptr)+7200);

    machine.describe();
    Vote* resultados = machine.sorted_by_data();
    for (int i = 0; i < 3; i++) {
        resultados[i].print();
        cout << endl;
    }

    return 0;
}
