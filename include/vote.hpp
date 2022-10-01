#ifndef VOTE_HPP_
#define VOTE_HPP_

#include <ctime>
#include "./date.hpp"

typedef unsigned int uint;

class Vote {
 private:
    Date date;
    uint user_id;
    uint vote_id;
    short candidate_id;
    char abbrev[2];

 public:
    Vote() {
        this->user_id = 0;
    }

    Vote(const Vote& other) {
        this->date = other.date;
        this->user_id = other.user_id;
        this->vote_id = other.vote_id;
        this->candidate_id = other.candidate_id;
        this->abbrev[0] = other.abbrev[0];
        this->abbrev[1] = other.abbrev[1];
    }

    Vote(uint vote_id, uint user_id, short candidate_id, const char* abbrev) :
        user_id(user_id), vote_id(vote_id), candidate_id(candidate_id) {
        this->date = Date(std::time(nullptr));
        this->abbrev[0] = abbrev[0];
        this->abbrev[1] = abbrev[1];
    }

    Vote(uint vote_id, uint user_id, short candidate_id, const char* abbrev,
    time_t timestamp) : user_id(user_id), vote_id(vote_id), candidate_id(candidate_id) {
        this->date = Date(timestamp);
        this->abbrev[0] = abbrev[0];
        this->abbrev[1] = abbrev[1];
    }

    uint get_vote_id() const {
        return this->vote_id;
    }

    uint get_user_id() const {
        return this->user_id;
    }

    void set_null() {
        this->user_id = 0;
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

    void print() const {
        std::cout << "Recibo: " << this->vote_id << std::endl;
        std::cout << "ID do Eleitor: " << this->user_id << std::endl;
        std::cout << "ID do Candidato: " << this->candidate_id << std::endl;
        std::cout << "Estado: " << this->abbrev[0] << this->abbrev[1] << std::endl;
        std::cout << "Data: " << this->date << std::endl;
    }
};

std::ostream& operator<<(std::ostream& out, const Vote& vote) {
    out << vote.get_vote_id() << "," << vote.get_user_id() << "," << vote.get_candidate_id() \
    << "," << vote.get_abbrev()[0] << vote.get_abbrev()[1] << "," << vote.get_date() << '\n';
    return out;
}

#endif  // VOTE_HPP_
