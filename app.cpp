#include <iostream>
#include <ctime>
//nanosleep()

#include "include/voting.hpp"

using std::cout;
using std::cin;
using std::endl;

// function for clearing the console based on which console you are in.
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

// function for freezing excecution for miliseconds.
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

// function for handling when there's any error on a cin input.
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

// function for voting interface
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
    if (handle_invalid_input() || candidate <= 0){
        clear();
        return -1;
    }
    clear();

    int code = machine->vote(id, candidate, std::time(nullptr));
    return code;
}

// function for dealing with admin choices
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
        cout << "Digite um estado para filtar o relatório, ou vazio caso não deseje filtrar." << endl;
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
        topid = machine->topK_sorted(10);

        wait(10000); // waits 10 secs

        clear();
        break;

    default:
        break;
    }
    return choice;
}


int main(){
    clear();
    // declaring and initializing some vars
    int choice = -1;
    int code;
    Vote received;
    System machine = System();
    // main loop
    while (choice != 0)
    {   
        // first option screen
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
            // computes voting
            code = vote(&machine);

            if (code != -1){ // if vote has been valid
                cout << "Seu recibo é: " << code << endl;
                wait(2000); // waits 2 secs
            }

            clear();
            break;

        case 2:
            // returns a vote
            cout << "Digite o número do seu recibo: ";
            cin >> code;
            if (handle_invalid_input()){
                clear();
                break;
            }
            received = machine.search(code);
            clear();

            received.print();

            wait(5000); // waits 5 secs

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
            wait(2000); // waits 2 secs
            clear();
            break;
        }
    }
}