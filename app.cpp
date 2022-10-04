#include <iostream>
#include <ctime>

#include "include/voting.hpp"

using std::cout;
using std::cin;
using std::endl;

// Função para limpar o terminal baseado no OS em que está.
void clear()
{
#if defined _WIN32
    system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#elif defined (__APPLE__)
    system("clear");
#endif
}

// Função para congelar a execução do programa (em ms)
// based on https://man7.org/linux/man-pages/man2/nanosleep.2.html
int wait(long miliseconds)
{
   struct timespec rem;
   struct timespec req= {
       (int)(miliseconds / 1000),     
       (miliseconds % 1000) * 1000000
   };

   return nanosleep(&req , &rem);
}

// Função para tratar qualquer tipo de erro vindo de um cin
bool handle_invalid_input(){
    if (cin.fail()) {
        cin.clear();
        cin.ignore(80, '\n');

        cout << "Opção inválida! Operação cancelada. Aguarde." << endl;
        wait(2000); // waits 2 secs
        return true;
    } else {
        return false;
    }
}

// Função para inserção do voto.
int vote(System* machine){
    int id, candidate;
    cout << "Digite seu ID: ";
    cin >> id;
    if (handle_invalid_input() || id <= 0){
        clear();
        return -1;
    }
    cout << "Digite o número do candidato em que deseja votar: ";
    cin >> candidate;
    if (handle_invalid_input() || id <= 0){
        clear();
        return -1;
    }
    clear();

    int code = machine->vote(id, candidate, std::time(nullptr));
    return code;
}

// Função para controlar as escolhar do admin.
int admin(int choice, System* machine){
    Vector<Vote> results;
    Vector<System::PairCount> topid;
    char state[2];
    switch (choice)
    {
    case 0:
        choice = -1;
        break;
    
    case 1:
        // MARCELO. 
        // cout << "Digite um estado para filtar o relatório, ou vazio caso não deseje filtrar." << endl;
        // cin >> state;
        // handle_invalid_input();

        results = machine->sorted_by_date();
        for (int i = 0; i < results.size(); i++){
            results[i].print();
        }

        wait(10000); // waits 10 secs
        clear();

        break;
    
    case 2:
        // Marcelo
        // topid = machine->topK_candidates(10);
        // cout << "Top 10 candidatos: " << endl;
        // for (int i = 0; i < 10; i++) {
        //     cout << "ID: " << topid[i].candidate_id << endl;
        //     cout << "Contagem: " << topid[i].count << endl;
        // }
        

        // wait(10000); // waits 10 secs

        clear();
        break;

    default:
        break;
    }
    return choice;
}


int main(){
    clear();
    // Declarando e inicializando algumas variáveis.
    int choice = -1;
    int code;
    Vote received;
    System machine = System();
    // Loop principal
    while (choice != 0)
    {   
        // Primeira tela de escolhas.
        cout << "Bem-vindo às eleições da FGV\nEscolha a opção desejada: " << endl;
        cout << "1. Votar em um candidato." << endl;
        cout << "2. Consultar seu voto." << endl;
        cout << "3. Operações de administrador." << endl;
        cout << "0. Encerrar sessão." << endl;
        cout << "Opção: ";
        cin >> choice;
        handle_invalid_input();
        clear();
        switch (choice)
        {
        case 0:
            break;

        case 1:
            // Computa o voto
            code = vote(&machine);

            if (code != -1){ // Se o voto foi válido
                cout << "Seu recibo é: " << code << endl;
                wait(2000); // Aguarda 2 segundos.
            }

            clear();
            break;

        case 2:
            // Retorna o voto.
            cout << "Digite o número do seu recibo: ";
            cin >> code;
            if (handle_invalid_input()){
                clear();
                break;
            }
            received = machine.search(code);
            clear();

            received.print();

            wait(5000); // Aguarda 5 segundos.

            clear();
            break;

        case 3:
            choice = -1;
            cout << "1. Exibir relatório de todos os votos computados." << endl;
            cout << "2. Exibir relatório dos 10 candidatos mais votados." << endl;
            cout << "0. Retornar.\nOpção:" << endl;
            cin >> choice;
            handle_invalid_input();
            clear();

            choice = admin(choice, &machine);

            break;

        default:
            cout << "Opção inválida! Insira uma das opções listadas. Aguarde." << endl;
            wait(2000); // Aguarda 2 segundos.
            clear();
            break;
        }
    }
}