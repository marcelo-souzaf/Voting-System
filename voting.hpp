#ifndef VOTING_HPP_
#define VOTING_HPP_

#include <random>  // Para gerar números aleatórios
#include <string>  // Para melhorar a mensagem de erro

#include "./Vector.hpp"
#include "./vote.hpp"
#include "./stack.hpp"
#define swap(v, i, j) { int temp = v[i]; v[i] = v[j]; v[j] = temp; }

// Laguardia disse que é um bom nome para uma classe
class Roberto {
 private:
    Vote* data;
    Stack deleted;
    uint capacity;
    uint current_idx;
    uint candidate_count = 5; // candidate_id é um short, logo, 32767 valores distintos
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
    Roberto(uint population_size=1000000, uint candidate_count=20000) {
        // Inicializa a máquina de votos com uma capacidade de 8 votos
        this->data = new Vote[6];
        this->capacity = 6;
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
    Vector<Vote> sorted_by_date() {
        Vector<Vote> sorted(this->current_idx - this->deleted.get_size());
        for (uint i = 0; i < this->current_idx; ++i) {
            if (this->data[i].get_user_id() != 0) {
                sorted.push_back(this->data[i]);
            }
        }
        quicksort(sorted.data(), 0, sorted.size() - 1);
        return sorted;
    }

    Vector<uint> topK_candidates(uint k, Date beginning, Date end) {
        Vector<uint> topK(k * 2);
        Vector<uint> counts = votecount_by_date(beginning, end);
        Vector<uint> indices(this->candidate_count);
        for (uint i = 0; i < this->candidate_count; ++i) {
            indices.push_back(i);
        }
        this->quickselect(counts.data(), indices.data(), 0, this->candidate_count - 1, k);
        for (uint i = k; k < this->candidate_count; ++i) {
            topK.push_back(indices[i]);
            topK.push_back(counts[indices[i]]);
        }
        return topK;
    }

    Vector<uint> topK_candidates(int k) {
        Vector<uint> topK(k * 2);
        Vector<uint> counts = votecount_by_date();
        Vector<uint> indices(this->candidate_count);
        for (uint i = 0; i < this->candidate_count; ++i) {
            indices.push_back(i);
        }
        this->quickselect(counts.data(), indices.data(), 0, this->candidate_count - 1, k);
        for (uint i = this->candidate_count - k; i < this->candidate_count; ++i) {
            topK.push_back(indices[i]);
            topK.push_back(counts[indices[i]]);
        }
        return topK;
    }
    
    Vector<uint> votecount_by_date(Date beginning, Date end) {
        Vector<uint> votecount(this->candidate_count);
        for (int i = 0; i < this->candidate_count; ++i) {
            votecount.push_back(0);
        }
        for (uint i = 0; i < this->current_idx; ++i) {
            if (this->data[i].get_user_id() != 0) {
                if (this->data[i].get_date() >= beginning &&
                    this->data[i].get_date() <= end) {
                    ++votecount[this->data[i].get_candidate_id() - 1];
                }
            }
        }
        return votecount;
    }

    Vector<uint> votecount_by_date() const {
        Vector<uint> votecount(this->candidate_count);
        for (uint i = 0; i < this->candidate_count; ++i) {
            votecount.push_back(0);
        }
        for (uint i = 0; i < this->current_idx; ++i) {
            if (this->data[i].get_user_id() != 0) {
                ++votecount[this->data[i].get_candidate_id() - 1];
            }
        }
        return votecount;
    }

 private:
    void quicksort(Vote* data, int left, int right) {
        int i = left, j = right;
        Vote tmp;
        Vote pivot = data[(left + right) / 2];
        /* partition */
        while (i <= j) {
            while (data[i].get_date() < pivot.get_date()) {
                ++i;
            }
            while (data[j].get_date() > pivot.get_date()) {
                --j;
            }
            if (i <= j) {
                Vote tmp = data[i];
                data[i] = data[j];
                data[j] = tmp;
                ++i;
                --j;
            }
        };

        /* recursion */
        if (left < j) {
            this->quicksort(data, left, j);
        }
        if (i < right) {
            this->quicksort(data, i, right);
        }
    }

    uint partition_idx(uint counts[], uint idx[], uint left, uint right) {
        uint pivot = counts[right];
        uint j = left;
        for (uint i = left; i < right; ++i) {
            if (counts[idx[i]] <= pivot) {
                swap(idx, i, j);
                ++j;
            }
        }
        swap(idx, j, right);
        return j;
    }

    void quickselect(uint counts[], uint idx[], uint left, uint right, uint k) {
        if (k > right - left + 1) {
            return;
        }
        uint j = this->partition_idx(counts, idx, left, right);
        uint length = right - j + 1;
        if (length == k) {
            return;
        }
        else if (length > k) {
            this->quickselect(counts, idx, j + 1, right, k);
        }
        else {
            this->quickselect(counts, idx, left, j - 1, k - length);
        }
    }
};

#endif  // VOTING_HPP_
