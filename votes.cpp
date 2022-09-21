#include <chrono>
#include <ctime>

#include "./voting.hpp"
#include "./Vetor.hpp"

using std::cout;
using std::endl;

int main() {
    cout << "Bytes da classe de voto: " << sizeof(Vote) << endl;

    // start = std::chrono::system_clock::now();

    // end = std::chrono::system_clock::now();
    // std::chrono::duration<double> diff = end - start;
    // std::cout << diff.count() << std::endl;
    
    // // Gerador de números aleatórios, talvez seja útil depois
    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::uniform_int_distribution<int> distrib(0, 10000);
    // int i = distrib(gen);

    // time_t current_seconds = std::time(nullptr);
    // tm* _time = std::gmtime(&current_seconds);
    // _time->tm_mday = 29;
    // _time->tm_mon = 1;
    // _time->tm_year = 120;
    // Date::FieldDate date_test(Date(std::mktime(_time)));
    // cout << date_test << endl;
    // cout << std::asctime(_time) << endl;

    Roberto machine = Roberto();
    Vetor<int> votes = Vetor<int>(32);
    votes.push_back(machine.vote(1, 1, "SP"));
    votes.push_back(machine.vote(2, 1, "MG"));
    votes.push_back(machine.vote(3, 2, "RJ"));
    votes.push_back(machine.vote(1, 2, "SP"));
    votes.push_back(machine.vote(2, 2, "MG"));
    votes.push_back(machine.vote(3, 3, "RJ"));
    votes.push_back(machine.vote(4, 1, "AC"));
    votes.push_back(machine.vote(5, 1, "AM"));
    votes.push_back(machine.vote(6, 1, "AP"));
    votes.remove(3);
    votes.push_back(machine.vote(7, 1, "BA"));
    for (uint i = 0; i < votes.size(); ++i) {
        machine.search(votes[i]).print();
        cout << endl;
    }
    return 0;
}
