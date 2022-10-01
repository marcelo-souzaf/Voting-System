#ifndef VOTING_HPP_
#define VOTING_HPP_

#include <random>
#include <string>

#include "./Vector.hpp"
#include "./vote.hpp"
#include "./stack.hpp"
#define swap(v, i, j) { int temp = v[i]; v[i] = v[j]; v[j] = temp; }

// Laguardia disse que é um bom nome para uma classe
class Roberto {
 public:
    struct PairCount {
        short candidate_id;
        uint count;
    }; 

 private:
    Vote* data;
    Stack deleted;
    uint capacity;
    uint current_idx;
    char* user_states;
    uint candidate_count = 32767; // candidate_id é um short, logo, 32767 valores distintos

    void resize() {
        // Duplica a capacidade do array que armazena as informações de votos
        Vote* old_data = this->data;
        this->capacity *= 2;
        this->data = new Vote[this->capacity];
        std::copy(old_data, old_data + this->current_idx, this->data);
        delete[] old_data;
    }


 public:
    Roberto(uint population_size=1000000) {
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
        Vector<Vote> sorted(this->current_idx - this->deleted.get_size());
        for (uint i = 0; i < this->current_idx; ++i) {
            if (this->data[i].get_user_id() != 0) {
                sorted.push_back(this->data[i]);
            }
        }
        quicksort(sorted.data(), 0, sorted.size() - 1);
        return sorted;
    }

    // Função que engloba a seleção dos topK candidatos, retornando
    // uma array de structs que contém o id do candidato e a sua respectiva
    // quantidade de votos, ordenada por essa quantidade
    Roberto::PairCount* topK_sorted(uint k, Date beginning, Date end) {
        uint* counts = this->votecount_by_date(beginning, end);
        short* candidates = this->topK_candidates(k, beginning, end);
        Roberto::PairCount* pairs = new Roberto::PairCount[k];
        for (int i = 0; i < k; i++) {
            pairs[i].candidate_id = candidates[i];
            pairs[i].count = counts[candidates[i]-1];
        }
        // Bubble sort otimizado, bom para esse contexto
        // pois o vetor está próximo de ser ordenado
        // O(k^2) no pior caso, mas como k é uma constante pequena no caso
        // de uso do trabalho (que será k=10), o custo computacional é baixo
        for (int i = 0; i < k; i++) {
            bool flag = false;
            for (int j = 0; j < k-i-1; j++) {
                if (pairs[j].count < pairs[j+1].count) {
                    flag = true;
                    PairCount temp = pairs[j]; 
                    pairs[j] = pairs[j+1];
                    pairs[j+1] = temp;
                }
            }
            if (!flag) {
                break;
            }
        }
        return pairs;
    }

    Roberto::PairCount* topK_sorted(uint k) {
        uint* counts = this->votecount_by_date();
        short* candidates = this->topK_candidates(k);
        Roberto::PairCount* pairs = new Roberto::PairCount[k];
        for (int i = 0; i < k; i++) {
            pairs[i].candidate_id = candidates[i];
            pairs[i].count = counts[candidates[i]-1];
        }
        // Bubble sort otimizado, bom para esse contexto
        // pois o vetor está próximo de ser ordenado
        // O(k^2) no pior caso, mas como k é uma constante pequena no caso
        // de uso do trabalho (que será k=10), o custo computacional é baixo
        for (int i = 0; i < k; i++) {
            bool flag = false;
            for (int j = 0; j < k-i-1; j++) {
                if (pairs[j].count < pairs[j+1].count) {
                    flag = true;
                    PairCount temp = pairs[j]; 
                    pairs[j] = pairs[j+1];
                    pairs[j+1] = temp;
                }
            }
            if (!flag) {
                break;
            }
        }
        return pairs;
    }

    short* topK_candidates(int k, Date beginning, Date end) {
        short* topk = new short[k];
        uint* counts = votecount_by_date(beginning, end); 
        uint temp[this->candidate_count];
        for (uint i = 0; i < this->candidate_count; i++) {
            temp[i] = counts[i];
        }
        int k_inv = this->candidate_count - k;
        int element_kth = quickselect(temp, 0, this->candidate_count - 1, k_inv);
        int aux = 0;
        for (int i = 0; i < candidate_count; i++) {
            if (counts[i] > element_kth) {
                topk[aux++] = i+1; // +1 porque o índice do vetor é o id do candidato - 1
            }
            if (aux == k) {
                break;
            }
        } 
        // Separei em dois loops para lidar com empates (senão, haveria a possi-
        // bilidade de retornar k dos candidatos com a quantidade de votos iguais
        // ao k-ésimo maior, enquanto os valores maiores que o k-ésimo não seriam
        // retornados)
        for (int i = 0; i < candidate_count; i++) {
            if (aux == k) {
                break;
            }
            if (counts[i] == element_kth) {
                topk[aux++] = i+1;
            }
        }
        return topk;
    }

    short* topK_candidates(int k) {
        short* topk = new short[k];
        uint* counts = votecount_by_date(); 
        uint temp[this->candidate_count];
        for (uint i = 0; i < this->candidate_count; i++) {
            temp[i] = counts[i];
        }
        int k_inv = this->candidate_count - k;
        int element_kth = quickselect(temp, 0, this->candidate_count - 1, k_inv);
        int aux = 0;
        for (int i = 0; i < candidate_count; i++) {
            if (counts[i] > element_kth) {
                topk[aux++] = i+1; // +1 porque o índice do vetor é o id do candidato - 1
            }
            if (aux == k) {
                break;
            }
        } 
        // Separei em dois loops para lidar com empates (senão, haveria a possi-
        // bilidade de retornar k dos candidatos com a quantidade de votos iguais
        // ao k-ésimo maior, enquanto os valores maiores que o k-ésimo não seriam
        // retornados)
        for (int i = 0; i < candidate_count; i++) {
            if (aux == k) {
                break;
            }
            if (counts[i] == element_kth) {
                topk[aux++] = i+1;
            }
        }
        return topk;
    }
    
    uint* votecount_by_date(Date beginning, Date end) {
        // Como a quantidade de candidatos únicos é baixa, não há
        // necessidade de usar uma tabela hash complexa
        uint* votecount = new uint[this->candidate_count];
        for (int i = 0; i < this->candidate_count; i++) {
            votecount[i] = 0;
        }
        for (uint i = 0; i < this->current_idx; i++) {
            if (this->data[i].get_user_id() != 0) {
                if (this->data[i].get_date() >= beginning &&
                    this->data[i].get_date() <= end) {
                    votecount[this->data[i].get_candidate_id()-1]++;
                }
            }
        }
        return votecount;
    }

    uint* votecount_by_date() {
        // Como a quantidade de candidatos únicos é baixa, não há
        // necessidade de usar uma tabela hash complexa
        uint* votecount = new uint[this->candidate_count];
        for (int i = 0; i < this->candidate_count; i++) {
            votecount[i] = 0;
        }
        for (uint i = 0; i < this->current_idx; i++) {
            if (this->data[i].get_user_id() != 0) {
                votecount[this->data[i].get_candidate_id()-1]++;
            }
        }
        return votecount;
    }
    

 private:
    void quicksort(Vote* data, int left, int right) {
        int i = left, j = right;
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

    int partition(uint v[], int p, int r) {
        int pivot = v[r];
        int j = p;
        for (int i=p; i < r; i++) {
            if (v[i] <= pivot) {
                swap(v, i, j);
                j++;
            }
        }
        swap(v, j, r);
        return j;
    }

    int quickselect(uint v[], int left, int right, int x) {
        if (x > 0 && x <= right - left + 1) {
            int j = partition(v, left, right);
            if (j - left == x - 1) {
                return v[j];
            }
            if (j - left > x - 1) {
                return quickselect(v, left, j - 1, x);
            }
            return quickselect(v, j + 1, right, x - j + left - 1);
        }
        return -1;
    }
};

#endif  // VOTING_HPP_
