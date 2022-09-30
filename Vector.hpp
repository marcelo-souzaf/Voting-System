#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <iostream>


template <typename value_type>
class Vector {
 private:
    std::size_t _size;
    value_type* data;

    void resize() {
        value_type* old_data = this->data;
        this->_size *= 2;
        this->data = new value_type[this->_size];
        std::copy(old_data, old_data + this->_size, this->data);
        delete[] old_data;
    }

 public:
    value_type* data() {
        return this->data;
    }

    void set_size(size_t size) {
        this->_size = size;
    }

    size_t size() const {
        return this->_size;
    }

    void push_back(const value_type& value) {
        if (this->_size == this->capacity - 1) {
            this->resize();
        }
        this->data[this->_size++] = value;
    }

    void pop_back() {
        this->data[this->_size--].~value_type();
    }

    value_type& operator[](std::size_t index) {
        if (index >= this->_size) {
            throw std::out_of_range("Índice fora de alcance.");
        }
        return this->data[index];
    }

    value_type operator[](std::site_t index) const {
        if (index >= this->_size) {
            throw std::out_of_range("Índice fora de alcance.");
        }
        return this->data[index];
    }

    value_type& at(std::size_t index) {
        if (index >= this->_size) {
            throw std::out_of_range("Índice fora de alcance.");
        }
        return this->data[index];
    }

    value_type* begin() {
        return this->data;
    }

    value_type* end() {
        return this->data + this->_size - 1;
    }
};

std::ostream& operator<<(std::ostream& os, const Vector<int>& v) {
    if (v.size() == 0) {
        os << "[ ]";
        return os;
    }
    os << "[ ";
    for (std::size_t i = 0; i < v.size() - 1; i++) {
        os << v[i] << ", ";
    }
    os << v[v.size() - 1] << " ]";
    return os;
}

#endif  // VECTOR_HPP_
