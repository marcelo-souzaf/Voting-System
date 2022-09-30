#ifndef VOTING_HPP_
#define VOTING_HPP_

#include <string>

#include "./vote.hpp"
#include "./stack.hpp"
#define swap(v, i, j) { int temp = v[i]; v[i] = v[j]; v[j] = temp; }

// Laguardia disse que é um bom nome para uma classe
class Roberto {
 public:
    Vote* data;
 private:
    Stack deleted;
    uint capacity;
    uint current_idx;
    uint candidate_count = 32767; // candidate_id é um short, logo, 32767 valores distintos

    void resize() {
        Vote* old_data = this->data;
        this->capacity *= 2;
        this->data = new Vote[this->capacity];
        std::copy(old_data, old_data + this->current_idx, this->data);
        delete[] old_data;
    }

 public:
    Roberto() {
        this->data = new Vote[8];
        this->capacity = 8;
        this->current_idx = 0;
    }

    ~Roberto() {
        delete[] this->data;
    }

    uint vote(uint user_id, short candidate_id, const char* abbrev) {
        if (this->current_idx == this->capacity - 1 && !this->deleted) {
            this->resize();
        }
        uint index = this->deleted ? this->deleted.pop() : this->current_idx++;
        this->data[index] = Vote(index, user_id, candidate_id, abbrev);
        return index;
    }

    uint vote(uint user_id, short candidate_id, const char* abbrev, time_t timestamp) {
        if (this->current_idx == this->capacity - 1 && !this->deleted) {
            this->resize();
        }
        uint index = this->deleted ? this->deleted.pop() : this->current_idx++;
        this->data[index] = Vote(index, user_id, candidate_id, abbrev, timestamp);
        return index;
    }

    void remove(uint vote_id) {
        this->data[vote_id].set_null();
        this->deleted.push(vote_id);
    }

    Vote& search(uint vote_id) {
        if (this->data[vote_id].get_user_id() == 0) {
            throw std::invalid_argument("Voto de ID " + std::to_string(vote_id) + " não encontrado.");
        }
        return this->data[vote_id];
    }

    void describe() {
        std::cout << "Highest vote count: " << this->current_idx << std::endl;
        std::cout << "Current capacity: " << this->capacity << std::endl;
        std::cout << "Deleted votes: " << this->deleted.get_size() << std::endl;
    }

    Vote* sorted_by_data() {
        Vote* sorted = new Vote[this->current_idx - this->deleted.get_size()];
        uint sorted_idx = 0;
        for (uint i = 0; i < this->current_idx; i++) {
            if (this->data[i].get_user_id() != 0) {
                sorted[sorted_idx++] = this->data[i];
            }
        }
        quick_sort(sorted, 0, sorted_idx - 1);
        return sorted;
    }

    short* topK_candidates(int k, Date beginning, Date end) {
        short* top10 = new short[10];
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
                top10[aux++] = i;
            }
            if (aux == 10) {
                break;
            }
        } 
        // Separei em dois loops para lidar com empates (senão, haveria a possi-
        // bilidade de retornar 10 dos candidatos com a quantidade de votos iguais
        // ao k-ésimo maior, enquanto os valores maiores que o k-ésimo não seriam
        // retornados)
        for (int i = 0; i < candidate_count; i++) {
            if (aux == 10) {
                break;
            }
            if (counts[i] == element_kth) {
                top10[aux++] = i;
            }
        }
        return top10;
    }

    short* topK_candidates(int k) {
        short* top10 = new short[10];
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
                top10[aux++] = i;
            }
            if (aux == 10) {
                break;
            }
        } 
        // Separei em dois loops para lidar com empates (senão, haveria a possi-
        // bilidade de retornar 10 dos candidatos com a quantidade de votos iguais
        // ao k-ésimo maior, enquanto os valores maiores que o k-ésimo não seriam
        // retornados)
        for (int i = 0; i < candidate_count; i++) {
            if (aux == 10) {
                break;
            }
            if (counts[i] == element_kth) {
                top10[aux++] = i;
            }
        }
        return top10;
    }
    
    uint* votecount_by_date(Date beginning, Date end) {
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
    void quick_sort(Vote* data, uint left, uint right) {
        uint i = left, j = right;
        Vote tmp;
        Vote pivot = data[(left + right) / 2];

        /* partition */
        while (i <= j) {
            while (data[i].get_date() < pivot.get_date()) {
                i++;
            }
            while (data[j].get_date() > pivot.get_date()) {
                j--;
            }
            if (i <= j) {
                tmp = data[i];
                data[i] = data[j];
                data[j] = tmp;
                i++;
                j--;
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
