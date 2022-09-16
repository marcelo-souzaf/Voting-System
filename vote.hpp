#ifndef VOTE_HPP_
#define VOTE_HPP_

#include <ctime>
#include "./date.hpp"

typedef unsigned int uint;

class Vote {
 private:
    Date date;
    uint user_id;
    short candidate_id;
    char abbrev[2];
    Vote* next;

 public:
    Vote() {
        this->user_id = 0;
        this->candidate_id = 0;
        this->abbrev[0] = 0;
        this->abbrev[1] = 0;
        this->next = nullptr;
    }

    Vote(uint user_id, short candidate_id, const char* abbrev) :
        user_id(user_id), candidate_id(candidate_id) {
        this->abbrev[0] = abbrev[0];
        this->abbrev[1] = abbrev[1];
        this->date = Date(std::time(nullptr));
        this->next = nullptr;
    }

    Vote(uint user_id, short candidate_id, const char* abbrev, Date date) :
    user_id(user_id), candidate_id(candidate_id), date(date) {
        this->abbrev[0] = abbrev[0];
        this->abbrev[1] = abbrev[1];
        this->next = nullptr;
    }

    ~Vote() {
        delete this->next;
    }

    uint get_user_id() const {
        return this->user_id;
    }

    short get_candidate_id() const {
        return this->candidate_id;
    }

    Date get_date() const {
        return this->date;
    }

    const char* get_abbrev() const {
        return this->abbrev;
    }

    Vote* get_next() const {
        return this->next;
    }

    void enlist(Vote* vote) {
        if (this->next != nullptr) {
            std::invalid_argument("Voto já possui voto encadeado.");
        }
        this->next = vote;
        std::cout << "Colisão" << std::endl;
    }
};

#endif  // VOTE_HPP_
