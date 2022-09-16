#include <chrono>
#include <ctime>

#include "./voting.hpp"

using std::cout;
using std::endl;

int main() {
    // Alguns testes de datas
    // Ainda não achei um algoritmo de dia da semana que funciona...
    time_t current_seconds = std::time(nullptr);
    tm* _time = std::gmtime(&current_seconds);
    _time->tm_mday = 29;
    _time->tm_mon = 1;
    _time->tm_year = 121;
    Date date_test(std::mktime(_time));
    cout << date_test << endl;
    cout << std::asctime(_time) << endl;

    _time->tm_mday = 13;
    _time->tm_mon = 1;
    _time->tm_year = 120;
    date_test = Date(mktime(_time));
    cout << date_test << endl;
    cout << std::asctime(_time) << endl;

    _time->tm_mday = 32;
    _time->tm_mon = 11;
    _time->tm_year = 120;
    date_test = Date(mktime(_time));
    cout << date_test << endl;
    cout << std::asctime(_time) << endl;


    auto start = std::chrono::system_clock::now();

    Date date = Date(current_seconds);
    cout << date.get_absolute_seconds() << endl;
    cout << date << endl;

    Date::FieldDate converted(date);
    date = Date(converted);
    cout << date << endl;
    cout << date.get_absolute_seconds() << endl;

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    cout << diff.count() << endl;


    cout << "Bytes da classe de voto: " << sizeof(Vote) << endl;
    cout << "Armazenamento esperado: " << 100'000'000 * sizeof(Vote) * (4.0f / 3) / 1024.0 / 1024 / 1024 << " GB" << endl;

    // start = std::chrono::system_clock::now();

    // end = std::chrono::system_clock::now();
    // diff = end - start;
    // std::cout << diff.count() << std::endl;
    
    // // Gerador de números aleatórios, talvez seja útil depois
    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::uniform_int_distribution<int> distrib(0, 10000);
    // int i = distrib(gen);

    Roberto machine = Roberto();
    machine.votar(1, 1, "SP");
    return 0;
}
