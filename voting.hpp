#ifndef VOTING_HPP_
#define VOTING_HPP_

#include "./vote.hpp"

// Laguardia disse que é um bom nome para uma classe
class Roberto {
 private:
    Vote* data;
    uint size;
    uint capacity;

    // Função que definitivamente depende de vários dados do voto
    uint hash(const Vote& vote) const {
        return vote.get_user_id() % this->capacity;
    }

    void create_vote(uint user_id, short candidate_id, const char* abbrev) {
        Vote new_vote = Vote(user_id, candidate_id, abbrev);
        this->insert(this->hash(new_vote), new_vote);
    }

    void resize() {
        this->capacity *= 1.5;
        Vote* old_data = this->data;
        this->data = new Vote[this->capacity];
        for (uint i = 0; i < this->size; ++i) {
            if (this->data[i].get_user_id() != 0) {
                Vote* current = &this->data[i];
                do {
                    this->insert(this->hash(*current), *current);
                    current = current->get_next();
                } while (current != nullptr);
            }
        }
        delete[] old_data;
    }

 public:
    Roberto() {
        this->data = new Vote[10];
        this->capacity = 10;
        this->size = 0;
    }

    ~Roberto() {
        delete[] this->data;
    }

    void insert(uint index, const Vote& vote) {
        if (this->size + 1 > (3 * this->capacity / 4)) {
            this->resize();
        }
        if (this->data[index].get_user_id() == 0) {
            this->data[index] = vote;
        } else {
            Vote* current = &this->data[index];
            while (current->get_next() != nullptr) {
                current = current->get_next();
            }
            current->enlist(new Vote(vote));
        }
        ++this->size;
    }

    uint votar(uint user_id, short candidate_id, const char* abbrev) {
        Vote new_vote = Vote(user_id, candidate_id, abbrev);
        uint index = this->hash(new_vote);
        this->insert(index, new_vote);
        return index;
    }
};

#endif  // ROBERTO_HPP_
