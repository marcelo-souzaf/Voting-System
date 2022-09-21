#ifndef STACK_HPP_
#define STACK_HPP_

#include <iostream>

typedef unsigned int uint;

class Stack {
 private:
    uint* data;
    uint capacity;
    uint size;

    void resize() {
        uint* old_data = this->data;
        this->capacity *= 2;
        this->data = new uint[this->capacity];
        std::copy(old_data, old_data + this->size, this->data);
        delete[] old_data;
    }

 public:
    Stack() {
        this->data = new uint[8];
        this->capacity = 8;
        this->size = 0;
    }

    ~Stack() {
        delete[] this->data;
    }

    void push(uint index) {
        if (this->size == this->capacity) {
            this->resize();
        }
        this->data[this->size++] = index;
    }

    uint pop() {
        return this->data[--this->size];
    }

    explicit operator bool() {
        return this->size > 0;
    }
};

#endif  // STACK_HPP_
