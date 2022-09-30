#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <iostream>

template <typename value_type>
class Vector {
 private:
    std::size_t _size;
    std::size_t _capacity;
    value_type* _data;

    void resize() {
        value_type* old_data = this->_data;
        if (this->_capacity > 0) this->_capacity *= 2;
        else this->_capacity = 8;
        this->_data = reinterpret_cast<value_type*>(
            operator new(this->_capacity * sizeof(value_type)));
        std::copy(old_data, old_data + this->_size, this->_data);
        delete[] old_data;
    }

 public:
    Vector() {
        this->_size = 0;
        this->_capacity = 0;
        this->_data = nullptr;
    }

    explicit Vector(size_t capacity) {
        this->_size = 0;
        this->_capacity = capacity;
        this->_data = reinterpret_cast<value_type*>(
            operator new(this->_capacity * sizeof(value_type)));
    }

    Vector(const Vector& other) {
        this->_size = other._size;
        this->_capacity = other._capacity;
        this->_data = reinterpret_cast<value_type*>(
            operator new(this->_capacity * sizeof(value_type)));
        std::copy(other._data, other._data + this->_size, this->_data);
    }

    Vector(const std::initializer_list<value_type>& list) {
        this->_size = list.size();
        this->_capacity = list.size();
        this->_data = reinterpret_cast<value_type*>(
            operator new(this->_size * sizeof(value_type)));
        std::copy(list.begin(), list.end(), this->_data);
    }

    Vector(Vector&& temporary) {
        this->_size = temporary._size;
        this->_capacity = temporary._capacity;
        this->_data = temporary._data;
        temporary._data = nullptr;
    }

    ~Vector() {
        delete[] this->_data;
    }

    Vector<value_type>& operator=(const Vector<value_type>& other) {
        this->_capacity = other._capacity;
        this->_size = other._size;
        delete[] this->_data;
        this->_data = reinterpret_cast<value_type*>(
            operator new(other.capacity() * sizeof(value_type)));
        std::copy(other.data(), other.data() + other.capacity(), this->_data);
        return *this;
    }

    Vector<value_type>& operator=(Vector<value_type>&& temporary) {
        this->_capacity = temporary._capacity;
        this->_size = temporary._size;
        delete[] this->_data;
        this->_data = temporary._data;
        temporary._data = nullptr;
        return *this;
    }

    value_type* data() {
        return this->_data;
    }

    size_t size() const {
        return this->_size;
    }

    size_t capacity() const {
        return this->_capacity;
    }

    void push_back(const value_type& value) {
        if (this->_size == this->_capacity) {
            this->resize();
        }
        this->_data[this->_size++] = value;
    }

    void pop_back() {
        if (this->_size == 0) {
            throw std::out_of_range("Vetor vazio.");
        }
        this->_data[this->_size--].~value_type();
    }

    value_type& at(std::size_t index) {
        if (index >= this->_size) {
            throw std::out_of_range("Índice fora de alcance.");
        }
        return this->_data[index];
    }

    value_type& operator[](std::size_t index) {
        return this->at(index);
    }

    value_type operator[](std::size_t index) const {
        if (index >= this->_size) {
            throw std::out_of_range("Índice fora de alcance.");
        }
        return this->_data[index];
    }

    value_type* begin() {
        return this->_data;
    }

    value_type* end() {
        return this->_data + this->_size - 1;
    }
};

template <typename value_type>
std::ostream& operator<<(std::ostream& out, const Vector<value_type>& v) {
    if (v.size() == 0) {
        out << "[ ]";
        return out;
    }
    out << "[ ";
    for (std::size_t i = 0; i < v.size() - 1; ++i) {
        out << v[i] << ", ";
    }
    out << v[v.size() - 1] << " ]";
    return out;
}

#endif  // VECTOR_HPP_
