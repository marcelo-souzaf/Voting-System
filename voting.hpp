#ifndef VOTING_HPP_
#define VOTING_HPP_

#include "./vote.hpp"
#include "./stack.hpp"

// Laguardia disse que é um bom nome para uma classe
class Roberto {
 private:
    Vote* data;
    Stack deleted;
    uint capacity;
    uint current_idx;

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

    void remove(uint vote_id) {
        this->data[vote_id].set_null();
        this->deleted.push(vote_id);
    }

    Vote search(uint vote_id) {
        if (this->data[vote_id].get_user_id() == 0) {
            throw std::invalid_argument("Voto não encontrado.");
        }
        return this->data[vote_id];
    }
};

#endif  // VOTING_HPP_
