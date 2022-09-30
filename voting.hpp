#ifndef VOTING_HPP_
#define VOTING_HPP_

#include <random>
#include <string>

#include "./Vector.hpp"
#include "./vote.hpp"
#include "./stack.hpp"

// Laguardia disse que é um bom nome para uma classe
class Roberto {
 private:
    Vote* data;
    Stack deleted;
    uint capacity;
    uint current_idx;
    char* user_states;

    void resize() {
        // Duplica a capacidade do array que armazena as informações de votos
        Vote* old_data = this->data;
        this->capacity *= 2;
        this->data = new Vote[this->capacity];
        std::copy(old_data, old_data + this->current_idx, this->data);
        delete[] old_data;
    }

 public:
    Roberto(uint population_size=1'000'000) {
        // Inicializa a máquina de votos com uma capacidade de 8 votos
        this->data = new Vote[8];
        this->capacity = 8;
        this->current_idx = 0;
        const char states[27][3] = {"AC", "AL", "AP", "AM", "BA", "CE", "DF",
                                    "ES", "GO", "MA", "MT", "MS", "MG", "PA",
                                    "PB", "PR", "PE", "PI", "RJ", "RN", "RS",
                                    "RO", "RR", "SC", "SP", "SE", "TO"};
        // Inicializa as informações dos usuários, no caso, o estado
        const uint size = population_size * 2 + 2;
        this->user_states = new char[size];
        this->user_states[0] = '\0';
        this->user_states[1] = '\0';
        // Gerador de números aleatórios que ajuda a definir um estado para cada usuário
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint> distrib(0, 26);
        uint state_index;
        for (uint i = 2; i < size; ++i) {
            state_index = distrib(gen);
            this->user_states[i++] = states[state_index][0];
            this->user_states[i] = states[state_index][1];
        }
    }

    ~Roberto() {
        delete[] this->data;
        delete[] this->user_states;
    }

    // Cria um voto a partir de um id de usuário e de um candidato, usando o horário atual
    uint vote(uint user_id, short candidate_id) {
        if (this->current_idx == this->capacity - 1 && !this->deleted) {
            this->resize();
        }
        uint index = this->deleted ? this->deleted.pop() : this->current_idx++;
        this->data[index] = Vote(index, user_id, candidate_id, this->user_states + (user_id * 2));
        return index;
    }

    // Cria um voto a partir de ids e um horário especificados
    uint vote(uint user_id, short candidate_id, time_t timestamp) {
        if (this->current_idx == this->capacity - 1 && !this->deleted) {
            this->resize();
        }
        uint index = this->deleted ? this->deleted.pop() : this->current_idx++;
        this->data[index] = Vote(index, user_id, candidate_id,
            this->user_states + (user_id * 2), timestamp);
        return index;
    }

    // "Remove" um voto invalidando seu identificador de usuário e adiciona o recibo
    // desse voto a uma pilha para que ele possa ser reutilizado
    void remove(uint vote_id) {
        this->data[vote_id].set_null();
        this->deleted.push(vote_id);
    }

    // Busca o voto a partir de um recibo, que é um id numérico
    Vote& search(uint vote_id) {
        if (this->data[vote_id].get_user_id() == 0) {
            throw std::invalid_argument("Voto de ID " + std::to_string(vote_id) + " não encontrado.");
        }
        return this->data[vote_id];
    }

    // Apenas para testes
    void describe() {
        std::cout << "Capacidade atual: " << this->capacity << std::endl;
        std::cout << "Maior contagem de votos: " << this->current_idx << std::endl;
        std::cout << "Quantidade de votos apagados: " << this->deleted.get_size() << std::endl;
    }

    // Cria uma cópia de todos os votos válidos atualmente e os ordena
    Vector<Vote> sorted_by_data() {
        Vector<Vote> result(this->current_idx - this->deleted.get_size());
        Vote* sorted = result.data();
        uint sorted_idx = 0;
        for (uint i = 0; i < this->current_idx; ++i) {
            if (this->data[i].get_user_id() != 0) {
                sorted[sorted_idx++] = this->data[i];
            }
        }
        quick_sort(sorted, 0, sorted_idx - 1);
        return result;
    }

 private:
    void quick_sort(Vote* data, uint left, uint right) {
        uint i = left, j = right;
        Vote tmp;
        Vote pivot = data[(left + right) / 2];

        /* partition */
        while (i <= j) {
            while (data[i].get_date() < pivot.get_date()) {
                ++i;
            }
            while (data[j].get_date() > pivot.get_date()) {
                ++j;
            }
            if (i <= j) {
                tmp = data[i];
                data[i] = data[j];
                data[j] = tmp;
                ++i;
                --j;
            }
        };

        /* recursion */
        if (left < j) {
            this->quick_sort(data, left, j);
        }
        if (i < right) {
            this->quick_sort(data, i, right);
        }
    }
};

#endif  // VOTING_HPP_
