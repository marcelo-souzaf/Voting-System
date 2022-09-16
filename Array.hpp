// Copyright (c) 2022 - Marcelo Amaral
#ifndef ARRAY_HPP_  // NOLINT
#define ARRAY_HPP_

#include <algorithm>
#include <cmath>
#include <exception>
#include <iostream>
#include <limits>
#include <random>
#include <string>

// #define inf std::numeric_limits<uint>::max()
#define swap(i, j) { T temp = data[i]; data[i] = data[j]; data[j] = temp; }

typedef unsigned int uint;

struct Point;

template<class T>
class Array {
    uint _capacity;
    uint _size;
    T* data;
    const static uint inf = std::numeric_limits<uint>::max();

    int partition(int start, int end) {
        uint count = 0;
        T pivot = this->data[start];
        for (int i = start + 1; i <= end; ++i) {
            if (this->data[i] <= pivot) {
                ++count;
            }
        }
        // Obtém o índice correto do primeiro elemento, que é o pivô
        int pivot_index = start + count;
        // Troca os valores colocando o pivô na posição correta
        T aux = this->data[pivot_index];
        this->data[pivot_index] = pivot;
        this->data[start] = aux;
        // Ordena os elementos antes e depois do pivô
        int i = start;
        int j = end;
        while (i < pivot_index && j > pivot_index) {
            // Encontra o primeiro elemento maior que o pivô à sua esquerda
            while (this->data[i] <= pivot) {
                ++i;
            }
            // Encontra o primeiro elemento menor que o pivô à sua direita
            while (this->data[j] > pivot) {
                --j;
            }
            // Troca os valores se os índices não passaram pelo pivô
            if (i < pivot_index && j > pivot_index) {
                // cppcheck-suppress shadowVariable
                T aux = this->data[i];
                this->data[i++] = this->data[j];
                this->data[j--] = aux;
            }
        }
        return pivot_index;
    }

    void quick_sort(int start, int end) {
        if (start >= end) {
            return;
        }
        // Encontra o índice da partição do array
        int p = this->partition(start, end);
        // Ordenando a partição esquerda
        this->quick_sort(start, p - 1);
        // Ordenando a partição direita
        this->quick_sort(p + 1, end);
    }

    // Recebe uma função que será aplicada a elementos do
    // Array com base em elementos do Array de entrada
    Array<T> operation(const Array<T>& arr, T func(const T&, const T&)) {
        if (this->_size % arr.size() == 0) {
            Array<T> result(this->_size);
            result.set_size(this->_size);
            // for (uint i = 0; i < this->_size; i += arr.size()) {
            //     for (uint j = 0; j < arr.size(); ++j) {
            //         result[i + j] = func(this->data[i + j], arr[j]);
            //     }
            // }

            uint i = 0;
            for (uint j = 0; j < arr.size(); ++j) {
                for (uint k = 0; k < this->_size / arr.size(); ++k) {
                    result[i++] = func(this->data[i], arr[j]);
                }
            }
            return result;
        } else {
            throw std::invalid_argument(
                "Broadcast não foi possível na operação. Tamanhos " \
                + std::to_string(this->_size) + " e " + \
                std::to_string(arr.size()) + " não são múltiplos.");
        }
    }

    void operation_inplace(const Array<T>& arr, T func(const T&, const T&)) {
        if (this->_size % arr.size() == 0) {
            uint i = 0;
            for (uint j = 0; j < arr.size(); ++j) {
                for (uint k = 0; k < this->_size / arr.size(); ++k) {
                    this->data[i++] = func(this->data[i], arr[j]);
                }
            }
        } else {
            throw std::invalid_argument(
                "Broadcast não foi possível na operação. Tamanhos " \
                + std::to_string(this->_size) + " e " + \
                std::to_string(arr.size()) + " não são múltiplos.");
        }
    }

    Array<bool> comparison(const Array<T> arr, bool func(const T&, const T&)) {
        if (this->_size % arr.size() == 0) {
            Array<bool> result(this->_size);
            result.set_size(this->_size);
            uint i = 0;
            for (uint j = 0; j < arr.size(); ++j) {
                for (uint k = 0; k < this->_size / arr.size(); ++k) {
                    result[i++] = func(this->data[i], arr[j]);
                }
            }
            return result;
        } else {
            throw std::invalid_argument(
                "Broadcast não foi possível na operação. Tamanhos " \
                + std::to_string(this->_size) + " e " + \
                std::to_string(arr.size()) + " não são múltiplos.");
        }
    }

    uint ideal_size(uint size) {
        int i = 2;
        while (i < size) {
            i *= 2;
        }
        return i;
    }

 public:
    // Paulo Artur Silvério Von Knoblauch

    // Construtor padrão para Array vazio
    Array() {
        this->_capacity = 0;
        this->_size = 0;
        this->data = nullptr;
    }

    // Construtor para Array de capacidade especificada
    explicit Array(uint capacity, bool optimize_size = false) {
        if (optimize_size) {
            this->_capacity = this->ideal_size(capacity);
        } else {
            this->_capacity = capacity;
        }
        this->_size = 0;
        this->data = reinterpret_cast<T*>(operator new(_capacity * sizeof(T)));
    }

    // Inicializa a partir de um array padrão da linguagem
    Array(const T* data, uint size, bool optimize_size = false) {
        if (optimize_size) {
            this->_capacity = this->ideal_size(size);
        } else {
            this->_capacity = size;
        }
        this->_size = size;
        this->data = reinterpret_cast<T*>(operator new(_capacity * sizeof(T)));
        std::copy(data, data + size, this->data);
    }

    // Contrutor de Array que copia os valores de outro, [index, limit)
    Array(const Array<T>& other, uint capacity = 1,
          uint index = 0, uint limit = inf, bool optimize_size = false) {
        if (limit > other._size) {
            // Usa o tamanho do outro Array se não for especificado
            limit = other._size;
        }
        if (capacity < limit - index) {
            if (optimize_size) {
                this->_capacity = this->ideal_size(limit - index);
            } else {
                this->_capacity = limit - index;
            }
        }
        this->_size = limit - index;
        this->data = reinterpret_cast<T*>(operator new(_size * sizeof(T)));
        std::copy(other.data + index, other.data + limit, this->data);
    }

    // Permite inicialização do tipo Array = {1, 2, 3}
    // cppcheck-suppress noExplicitConstructor
    Array(const std::initializer_list<T>& l, bool optimize_size = false) {
        this->_size = l.size();
        if (optimize_size) {
            this->_capacity = this->ideal_size(this->_size);
        } else {
            this->_capacity = this->_size;
        }
        this->data = reinterpret_cast<T*>(operator new(_capacity * sizeof(T)));
        std::copy(l.begin(), l.end(), this->data);
    }

    // Inicializa o Array com um tamanho dado de elementos com o valor passado
    Array(uint size, const T& value, bool optimize_size = false) {
        this->_size = size;
        if (optimize_size) {
            this->_capacity = this->ideal_size(this->_size);
        } else {
            this->_capacity = this->_size;
        }
        this->data = reinterpret_cast<T*>(operator new(size * sizeof(T)));
        for (uint i = 0; i < size; ++i) {
            this->data[i] = value;
        }
    }

    // Construtor de "move", toma os dados do outro Array
    Array(Array<T>&& other) {
        this->_size = other._size;
        this->_capacity = other._capacity;
        this->data = other.data;
        other.data = nullptr;
    }

    // Destrutor que libera a memória alocada dinamicamente
    ~Array() {
        delete[] this->data;
    }

    uint size() const {
        return this->_size;
    }

    uint capacity() const {
        return this->_capacity;
    }

    // Define o tamanho do Array dado que este seja menor que
    // capacidade, sem fazer alterações no array interno
    void set_size(uint size) {
        if (size <= this->_capacity) {
            this->_size = size;
        } else {
            throw std::out_of_range(
                "Tamanho "+ std::to_string(size) +"excede capacidade máxima " \
                + std::to_string(this->_capacity) + ".");
        }
    }

    // Duplica a capacidade do Array quando uma inserção vai excedê-la
    void extend(uint multiplier = 2) {
        if (multiplier == 0) {
            this->clear();
            return;
        }
        if (multiplier == 1) {
            return;
        }
        if (this->_capacity == 0) {
            this->_capacity = 8;
        } else {
            this->_capacity *= multiplier;
        }
        T* new_data = reinterpret_cast<T*>(
            operator new(this->_capacity * sizeof(T)));
        std::copy(this->data, this->data + this->_size, new_data);
        delete[] this->data;
        this->data = new_data;
    }

    // Aumenta a capacidade máxima do Array, reserva mais memória
    void reserve(uint size) {
        if (size > this->_capacity) {
            this->_capacity = size;
            T* new_data = reinterpret_cast<T*>(
                operator new(this->_capacity * sizeof(T)));
            std::copy(this->data, this->data + this->_size, new_data);
            delete[] this->data;
            this->data = new_data;
        }
    }

    // Encolhe o tamanho do Array para o especificado ou não
    // faz nada se esse tamanho for maior ou igual ao atual
    void shrink(uint size) {
        if (size < this->_capacity) {
            this->_capacity = size;
            T* new_data = reinterpret_cast<T*>(
            operator new(this->_capacity * sizeof(T)));
            std::copy(this->data, this->data + size, new_data);
            delete[] this->data;
            this->data = new_data;
        }
    }

    // Define o tamanho como 0, equivalente a apagar todos os elementos
    void clear(bool destruct = true) {
        this->_size = 0;
        if (destruct) {
            delete[] this->data;
        }
    }

    Array<T> copy() const {
        return *this;
    }

    // Binary search: Tem complexidade O(log n) e encontra o índice da chave
    // procurada ou o índice do primeiro elemento maior que a chave procurada
    uint find(const T& key) {
        int s = 0;
        int e = this->_size;
        if (e == 0) {
            throw std::out_of_range("Array vazio.");
        } else if (e == 1) {
            return key <= this->data[0] ? 0 : 1;
        }
        int mid = 0;
        while (s < e) {
            mid = (s + e) / 2;
            if (this->data[mid] == key) {
                return mid;
            } else if (this->data[mid] > key) {
                e = mid - 1;
            } else if (this->data[mid] < key) {
                s = mid + 1;
            }
        }
        // Corrige a posição do índice que pode estar errada devido às divisões
        // de números ímpares por 2 e à busca de elementos não presentes
        if (this->data[mid] > key) {
            --mid;
        } else if (this->data[mid] < key) {
            ++mid;
        }
        // Corrige a correção...
        if (this->data[mid] < key && mid != this->_size) {
            ++mid;
        }
        return mid;
    }

    uint new_find(const T& key) {
        int s = 0;
        int e = this->_size;
        if (e == 0) {
            throw std::out_of_range("Array vazio.");
        } else if (e == 1) {
            return key <= this->data[0] ? 0 : 1;
        }
        int mid = 0;
        while (s < e) {
            mid = (s + e) / 2;
            if (this->data[mid] == key) {
                return mid;
            } else if (this->data[mid + 1] >= key) {
                return mid + 1;
            } else if (this->data[mid] - 1 < key) {
                return mid;
            } else if (this->data[mid] - 1 == key) {
                return mid - 1;
            } else if (this->data[mid] - 1 > key) {
                e = mid - 1;
            } else if (this->data[mid] - 1 < key) {
                s = mid + 1;
            }
        }
        return 100;
    }

    // Encontra apenas chaves existentes no Array, senão retorna -1
    int contains(const T& key) {
        int s = 0;
        int e = this->_size;
        if (e == 0) {
            throw std::out_of_range("Array vazio.");
        } else if (e == 1) {
            return key == this->data[0] ? 0 : -1;
        }
        int mid = 0;
        while (s < e) {
            mid = (s + e) / 2;
            if (this->data[mid] == key) {
                return mid;
            } else if (this->data[mid] > key) {
                e = mid - 1;
            } else if (this->data[mid] < key) {
                s = mid + 1;
            }
        }
        if (this->data[mid] > key) {
            --mid;
        } else if (this->data[mid] < key) {
            ++mid;
        }
        if (this->data[mid] == key) {
            return mid;
        }
        return -1;
    }

    // Valor no índice index é perdido, limit fica duplicado
    void rotate_left(uint index, uint limit) {
        for (uint i = index; i < limit; ++i) {
            this->data[i] = this->data[i + 1];
        }
    }

    // Valor no índice limit é perdido, index fica duplicado
    void rotate_right(uint index, uint limit) {
        for (uint i = limit; i > index; --i) {
            this->data[i] = this->data[i - 1];
        }
    }

    // Recebe valor a ser inserido, índice da inserção e o índice máximo
    // em que o Array será atualizado
    void insert(const T& value, uint index, uint limit = inf) {
        if (index > this->_capacity) {
            throw std::out_of_range("Índice " + std::to_string(index) \
                                    + " fora de alcance.");
        } else {
            // Restringe a atualização ao tamanho atual do Array
            if (limit > this->_size) {
                limit = this->_size;
            }
            if (this->_size == this->_capacity) {
                T* new_data = reinterpret_cast<T*>(
                    operator new(this->_capacity * sizeof(T)));
                std::copy(this->data, this->data + index, new_data);
                new_data[index] = value;
                std::copy(this->data + index, this->data + limit, new_data + index + 1);
                delete[] this->data;
                this->data = new_data;
            } else {
                this->rotate_right(index, limit);
                this->data[index] = value;
            }
            ++this->_size;
        }
    }

    // Copia elementos do Array atual e insere vários valores de uma vez
    void insert(const Array<uint>& indices, const Array<T>& values) {
        if (indices.size() != values.size()) {
            throw std::invalid_argument(
                "Índices e valores devem ter o mesmo tamanho.");
        }
        uint multiplier = 1;
        while (this->_capacity * multiplier < this->_size + indices.size()) {
            multiplier *= 2;
        }
        // Multiplica o tamanho do Array por 2 até que ele seja
        // suficiente para comportar todos os novos elementos
        T* new_data = reinterpret_cast<T*>(
            operator new(this->_capacity * multiplier * sizeof(T)));
        uint insert_idx = indices.size() - 1;
        uint new_size = this->_size + insert_idx;
        // Percorre o Array atual inserindo os valores
        for (uint i = new_size; i >= 0; --i) {
            // Se o índice atual é igual ao índice de inserção de novos
            // elementos e se o Array desses elementos ainda não chegou ao fim,
            // insere os elementos do Array de valores. O índice de inserção é
            // deslocado para cada inserção feita antes dele
            if (insert_idx >= 0 && i == indices[insert_idx] + insert_idx) {
                new_data[i] = values[insert_idx--];
            // Caso contrário, copia o elemento como era antes
            } else {
                new_data[i] = this->data[--this->_size];
            }
        }
        delete[] this->data;
        this->data = new_data;
        this->_size = new_size + 1;
    }

    // "Apaga" um elemento ao mover cada um dos elementos
    // de índice maior que ele para um índice menor.
    T remove(uint index, uint limit = inf) {
        if (index >= this->_size) {
            throw std::out_of_range("Índice " + std::to_string(index) \
                                    + " fora de alcance.");
        }
        if (limit >= this->_size) {
            limit = this->_size - 1;
        }
        T removed = this->data[index];
        this->rotate_left(index, limit);
        --this->_size;
        return removed;
    }

    // Se o último índice é maior que o tamanho
    // do Array, o último elemento é removido
    void remove(const Array<uint>& indices) {
        uint removal_size = indices.size();
        // Índice do Array em que uma mudança de elemento ocorrerá
        uint i = indices[0];
        // Índice do Array de índices
        uint j = 0;
        for (uint old_idx = indices[0]; old_idx < this->_size; ++old_idx) {
            while (j < removal_size && old_idx == indices[j]) {
                ++old_idx;
                ++j;
            }
            this->data[i++] = this->data[old_idx];
        }
        this->_size -= removal_size;
    }

    // Atualiza valores no Array atual com valores de um outro, com base
    // em um Array de índices onde esses valores serão atualizados
    // TODO: Implementar broadcasting
    void update(const Array<uint>& indices, const Array<T>& values) {
        if (indices.size() != values.size()) {
            throw std::invalid_argument(
                "Índices e valores devem ter o mesmo tamanho.");
        }
        uint j = 0;
        for (uint i = 0; i < indices.size(); ++i) {
            this->data[indices[i]] = values[j++];
        }
    }

    // Copia valores de outro Array a partir do índice i para o
    // atual no índice j, parando no índice definido por limit
    void merge(const Array<T>& other, uint i = 0,
               uint j = inf, uint limit = inf) {
        if (limit >= other._size) {
            limit = other._size - 1;
        }
        uint increase = limit - i;
        if (this->_size + increase > this->_capacity) {
            this->reserve(this->_size + increase);
        }
        if (j > this->_size) {
            j = this->_size;
        }
        this->_size += increase;
        for (; i < limit; ++i) {
            this->data[j++] = other.data[i];
        }
    }

    void push_back(const T& value) {
        if (this->_size == this->_capacity) {
            this->extend();
        }
        this->data[this->_size++] = value;
    }

    T pop_back() {
        if (this->_size == 0) {
            throw std::out_of_range("Array está vazio.");
        }
        return this->data[--this->_size];
    }

    T& back() {
        if (this->_size == 0) {
            throw std::out_of_range("Array está vazio.");
        }
        return this->data[this->_size - 1];
    }

    T back() const {
        if (this->_size == 0) {
            throw std::out_of_range("Array está vazio.");
        }
        return this->data[this->_size - 1];
    }

    void sort() {
        this->quick_sort(0, this->_size - 1);
    }

    // Operador para acessar os dados no array interno
    T& operator[](int index) {
        if (index < 0) {
            index += this->_size;
        }
        if (index >= this->_size || index < 0) {
            throw std::out_of_range("Índice " + std::to_string(index) + \
            " fora de alcance, tamanho do Array é " + \
            std::to_string(this->_size) + ".");
        }
        return this->data[index];
    }

    // Mesmo operador anterior, mas para quando o Array não pode ser alterado
    T operator[](int index) const {
        if (index < 0) {
            index += this->_size;
        }
        if (index >= this->_size || index < 0) {
            throw std::out_of_range("Índice " + std::to_string(index) + \
            " fora de alcance, tamanho do Array é " + \
            std::to_string(this->_size) + ".");
        }
        return this->data[index];
    }

    Array<T> operator[](const Array<uint>& indices) const {
        Array<T> result(indices.size());
        result.set_size(indices.size());
        int j = 0;
        for (uint i = 0; i < indices.size(); ++i) {
            result[j++] = this->data[indices[i]];
        }
        return result;
    }

    Array<T> operator[](const Array<bool>& indices) const {
        if (this->_size != indices.size()) {
            throw std::invalid_argument(
                "Arrays devem possuir o mesmo tamanho. Tamanho " + \
                std::to_string(this->_size) + " != " + \
                std::to_string(indices.size()) + ".");
        }
        Array<T> result(10);
        for (int i = 0; i < indices.size(); ++i) {
            if (indices[i]) {
                result.push_back(this->data[i]);
            }
        }
        return result;
    }

    explicit operator bool() const {
        return this->_size > 0;
    }

    // Alternativa ao operador [] para acessar elementos com um método
    T& at(uint index) {
        if (index >= this->_size) {
            throw std::out_of_range("Índice " + std::to_string(index) + \
            " fora de alcance, tamanho do Array é " + \
            std::to_string(this->_size) + ".");
        }
        return this->data[index];
    }

    T at(uint index) const {
        if (index >= this->_size) {
            throw std::out_of_range("Índice " + std::to_string(index) + \
            " fora de alcance, tamanho do Array é " + \
            std::to_string(this->_size) + ".");
        }
        return this->data[index];
    }

    Array<T> at(uint i, uint j) const {
        if (i > j || j > this->_size) {
            throw std::out_of_range("Índices " + std::to_string(i) + \
            " e/ou " + std::to_string(j) + " fora de alcance.");
        }
        Array<T> result = Array<T>(*this, 0, i, j);
        return result;
    }

    void set(uint index, const T& value) {
        if (index >= this->_size || index < 0) {
            throw std::out_of_range("Índice " + std::to_string(index) + \
            " fora de alcance, tamanho do Array é " + \
            std::to_string(this->_size) + ".");
        }
        this->data[index] = value;
    }

    // Operadores matemáticos

    // Soma inplace um valor a cada elemento do Array
    // Os próximos operadores seguem a mesma ideia
    Array<T>& operator+=(const T& value) {
        for (int i = 0; i < this->_size; ++i) {
            this->data[i] += value;
        }
        return *this;
    }

    Array<T>& operator-=(const T& value) {
        for (int i = 0; i < this->_size; ++i) {
            this->data[i] -= value;
        }
        return *this;
    }

    Array<T>& operator*=(const T& value) {
        for (int i = 0; i < this->_size; ++i) {
            this->data[i] *= value;
        }
        return *this;
    }

    Array<T>& operator/=(const T& value) {
        for (int i = 0; i < this->_size; ++i) {
            this->data[i] /= value;
        }
        return *this;
    }

    Array<T>& operator+=(const Array<T>& arr) {
        this->operation_inplace(arr, [](
            const T& a, const T& b) { return a + b; });
        return *this;
    }

    Array<T>& operator-=(const Array<T>& arr) {
        this->operation_inplace(arr, [](
            const T& a, const T& b) { return a - b; });
        return *this;
    }

    Array<T>& operator*=(const Array<T>& arr) {
        this->operation_inplace(arr, [](
            const T& a, const T& b) { return a * b; });
        return *this;
    }

    Array<T>& operator/=(const Array<T>& arr) {
        this->operation_inplace(arr, [](
            const T& a, const T& b) { return a / b; });
        return *this;
    }

    Array<T> operator+(const T& value) {
        Array<T> result(*this);
        for (int i = 0; i < this->_size; ++i) {
            result[i] += value;
        }
        return result;
    }

    Array<T> operator-(const T& value) {
        Array<T> result(*this);
        for (int i = 0; i < this->_size; ++i) {
            result[i] -= value;
        }
        return result;
    }

    Array<T> operator*(const T& value) {
        Array<T> result(*this);
        for (int i = 0; i < this->_size; ++i) {
            result[i] *= value;
        }
        return result;
    }

    Array<T> operator/(const T& value) {
        Array<T> result(*this);
        for (int i = 0; i < this->_size; ++i) {
            result[i] /= value;
        }
        return result;
    }

    Array<T> operator%(const T& value) {
        Array<T> result(*this);
        for (int i = 0; i < this->_size; ++i) {
            result[i] %= value;
        }
        return result;
    }

    Array<T> operator+(const Array<T>& arr) {
        return this->operation(arr, [](
            const T& a, const T& b) { return a + b; });
    }

    Array<T> operator-(const Array<T>& arr) {
        return this->operation(arr, [](
            const T& a, const T& b) { return a - b; });
    }

    Array<T> operator*(const Array<T>& arr) {
        return this->operation(arr, [](
            const T& a, const T& b) { return a * b; });
    }

    Array<T> operator/(const Array<T>& arr) {
        return this->operation(arr, [](
            const T& a, const T& b) { return a / b; });
    }

    Array<T> operator%(const Array<T>& arr) {
        return this->operation(arr, [](
            const T& a, const T& b) { return a % b; });
    }

    // Operadores de comparação

    Array<T>& operator=(const Array<T>& other) {
        this->_size = other._size;
        this->_capacity = other._capacity;
        this->data = other.data;
        return *this;
    }

    Array<T>& operator=(Array<T>&& other) {
        this->_size = other._size;
        this->_capacity = other._capacity;
        delete[] this->data;
        this->data = other.data;
        other.data = nullptr;
        return *this;
    }

    Array<bool> operator==(const T& value) {
        Array<bool> result(this->_size);
        result.set_size(this->_size);
        for (int i = 0; i < this->_size; ++i) {
            result[i] = this->data[i] == value;
        }
        return result;
    }

    Array<bool> operator!=(const T& value) {
        Array<bool> result(this->_size);
        result.set_size(this->_size);
        for (int i = 0; i < this->_size; ++i) {
            result[i] = this->data[i] != value;
        }
        return result;
    }

    Array<bool> operator<(const T& value) {
        Array<bool> result(this->_size);
        result.set_size(this->_size);
        for (int i = 0; i < this->_size; ++i) {
            result[i] = this->data[i] < value;
        }
        return result;
    }

    Array<bool> operator>(const T& value) {
        Array<bool> result(this->_size);
        result.set_size(this->_size);
        for (int i = 0; i < this->_size; ++i) {
            result[i] = this->data[i] > value;
        }
        return result;
    }

    Array<bool> operator<=(const T& value) {
        Array<bool> result(this->_size);
        result.set_size(this->_size);
        for (int i = 0; i < this->_size; ++i) {
            result[i] = this->data[i] <= value;
        }
        return result;
    }

    Array<bool> operator>=(const T& value) {
        Array<bool> result(this->_size);
        result.set_size(this->_size);
        for (int i = 0; i < this->_size; ++i) {
            result[i] = this->data[i] >= value;
        }
        return result;
    }

    Array<bool> operator==(const Array<T>& arr) {
        return this->comparison(arr, [](
            const T& a, const T& b) { return a == b; });
    }

    Array<bool> operator!=(const Array<T>& arr) {
        return this->comparison(arr, [](
            const T& a, const T& b) { return a != b; });
    }

    Array<bool> operator<(const Array<T>& arr) {
        return this->comparison(arr, [](
            const T& a, const T& b) { return a < b; });
    }

    Array<bool> operator>(const Array<T>& arr) {
        return this->comparison(arr, [](
            const T& a, const T& b) { return a > b; });
    }

    Array<bool> operator<=(const Array<T>& arr) {
        return this->comparison(arr, [](
            const T& a, const T& b) { return a <= b; });
    }

    Array<bool> operator>=(const Array<T>& arr) {
        return this->comparison(arr, [](
            const T& a, const T& b) { return a >= b; });
    }

    void print() const {
        std::cout << "[ ";
        for (int i = 0; i < this->_size - 1; ++i) {
            std::cout << this->data[i] << ", ";
        }
        if (this->_size > 0) {
            std::cout << this->data[this->_size - 1] << " ]" << std::endl;
        } else {
            std::cout << ']' << std::endl;
        }
    }

    void repr() const {
        for (uint i = 0; i < this->_size; ++i) {
            std::cout << this->data[i];
        }
        std::cout << std::endl;
    }

    class Iterator {
        T* ptr;

     public:
        explicit Iterator(T* ptr) {
            this->ptr = ptr;
        }

        Iterator& operator++() {
            this->ptr++;
            return *this;
        }

        Iterator operator++(int) {
            Iterator old = *this;
            this->ptr++;
            return old;
        }

        Iterator& operator--() {
            this->ptr--;
            return *this;
        }

        Iterator operator--(int) {
            Iterator old = *this;
            this->ptr--;
            return old;
        }

        T* operator->() {
            return this->ptr;
        }

        T operator*() const {
            return *this->ptr;
        }

        T& operator*() {
            return *this->ptr;
        }

        inline bool operator!=(const Iterator& other) const {
            return this->ptr != other.ptr;
        }
    };

    Iterator begin() {
        return Iterator(this->data);
    }

    Iterator end() {
        return Iterator(this->data + this->_size);
    }
};

// Cria Array a partir de cstring
Array<char> operator""_A(const char* str, size_t size) {
    Array<char> arr(str, size);
    return arr;
}

// Cria Array a partir de cstring com tamanho múltiplo de 2 para otimização
Array<char> operator""_AO(const char* str, size_t size) {
    Array<char> arr(str, size, true);
    return arr;
}

std::ostream& operator<<(std::ostream& os, const Array<char>& arr) {
    for (int i = 0; i < arr.size(); ++i) {
        os << arr[i];
    }
    return os;
}

Array<int> arange(int start, int stop, int step = 1) {
    Array<int> result = Array<int>();
    if (step > 0 && stop > start) {
        result = Array<int>(((stop - start - 1) / step) + 1);
        for (int i = start; i < stop; i += step) {
            result.push_back(i);
        }
    } else if (step < 0 && stop < start) {
        result = Array<int>((start - stop - 1) / -step) + 1;
        for (int i = start; i > stop; i += step) {
            result.push_back(i);
        }
    }
    return result;
}

Array<float> arange(float start, float stop, float step = 1.0) {
    Array<float> result = Array<float>();
    if (step > 0 && stop > start) {
        result = Array<float>(((stop - start - 1) / step) + 1);
        for (float i = start; i < stop; i += step) {
            result.push_back(i);
        }
    } else if (step < 0 && stop < start) {
        result = Array<float>((start - stop - 1) / -step) + 1;
        for (float i = start; i > stop; i += step) {
            result.push_back(i);
        }
    }
    return result;
}

Array<int> random_array(uint size, int start, int end) {
    Array<int> result = Array<int>(size);
    result.set_size(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(start, end);
    for (uint i = 0; i < size; ++i) {
        result[i] = distrib(gen);
    }
    return result;
}

Array<float> random_array(uint size, float start, float end) {
    Array<float> result = Array<float>(size);
    result.set_size(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distrib(start, end);
    for (uint i = 0; i < size; ++i) {
        result[i] = distrib(gen);
    }
    return result;
}

Array<int> abs(const Array<int>& arr) {
    Array<int> result(arr.size());
    result.set_size(arr.size());
    for (int i = 0; i < arr.size(); ++i) {
        result[i] = abs(arr[i]);
    }
    return result;
}

Array<float> abs(const Array<float>& arr) {
    Array<float> result(arr.size());
    result.set_size(arr.size());
    for (int i = 0; i < arr.size(); ++i) {
        result[i] = abs(arr[i]);
    }
    return result;
}

Array<int> power(const Array<int>& arr, int value) {
    Array<int> result(arr.size());
    result.set_size(arr.size());
    for (int i = 0; i < arr.size(); ++i) {
        result[i] = std::pow(arr[i], value);
    }
    return result;
}

Array<float> power(const Array<float>& arr, float value) {
    Array<float> result(arr.size());
    result.set_size(arr.size());
    for (int i = 0; i < arr.size(); ++i) {
        result[i] = std::pow(arr[i], value);
    }
    return result;
}

Array<int> neg(const Array<int>& arr) {
    Array<int> result(arr.size());
    result.set_size(arr.size());
    for (int i = 0; i < arr.size(); ++i) {
        result[i] = -arr[i];
    }
    return result;
}

Array<float> neg(const Array<float>& arr) {
    Array<float> result(arr.size());
    result.set_size(arr.size());
    for (int i = 0; i < arr.size(); ++i) {
        result[i] = -arr[i];
    }
    return result;
}

Array<bool> _and(const Array<bool>& arr, const Array<bool>& arr2) {
    Array<bool> result(arr.size());
    result.set_size(arr.size());
    for (int i = 0; i < arr.size(); ++i) {
        result[i] = arr[i] && arr2[i];
    }
    return result;
}

Array<bool> _or(const Array<bool>& arr, const Array<bool>& arr2) {
    Array<bool> result(arr.size());
    result.set_size(arr.size());
    for (int i = 0; i < arr.size(); ++i) {
        result[i] = arr[i] || arr2[i];
    }
    return result;
}

bool _all(const Array<bool>& arr) {
    for (int i = 0; i < arr.size(); ++i) {
        if (!arr[i]) {
            return false;
        }
    }
    return true;
}

bool _any(const Array<bool>& arr) {
    for (int i = 0; i < arr.size(); ++i) {
        if (arr[i]) {
            return true;
        }
    }
    return false;
}

Array<bool> _invert(const Array<bool>& arr) {
    Array<bool> result(arr.size());
    result.set_size(arr.size());
    for (int i = 0; i < arr.size(); ++i) {
        result[i] = !arr[i];
    }
    return result;
}

uint _count(const Array<bool>& arr) {
    int result = 0;
    for (int i = 0; i < arr.size(); ++i) {
        if (arr[i]) {
            ++result;
        }
    }
    return result;
}

#endif  // ARRAY_HPP_  // NOLINT
