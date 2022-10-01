// Copyright (c) 2022 - Marcelo Amaral
#ifndef VETOR_HPP_
#define VETOR_HPP_

#include <cmath>
#include <exception>
#include <iostream>
#include <random>
#include <string>

#define swap(i, j) { temp = data[i]; data[i] = data[j]; data[j] = temp; }

typedef unsigned int uint;

struct Point;

struct Index {
    int first;
    int second;
    Index() {}
    Index(int first, int second) : first(first), second(second) {}
    void set(int first, int second) {
        this->first = first;
        this->second = second;
    }
};

template <typename T>
class Vetor {
    uint _capacity;
    uint _size;
    T* data;
    constexpr static uint inf = 0U - 1U;

    // Binary search: Tem complexidade O(log n) e encontra o índice da chave
    // procurada ou o índice do primeiro elemento maior que a chave procurada
    uint binary_search(const T& key, uint l, uint h) const {
        uint mid = 0;
        while (l < h) {
            mid = (l + h) / 2;
            if (key == this->data[mid]) {
                return mid;
            } else if (key < this->data[mid]) {
                h = mid - 1;
            } else if (key > this->data[mid]) {
                l = mid + 1;
            }
        }
        // Corrige a posição do índice que pode estar errada devido às divisões
        // de números ímpares por 2 e à busca de elementos não presentes
        if (key < this->data[mid]) {
            --mid;
        } else if (key > this->data[mid]) {
            ++mid;
        }
        // Corrige a correção...
        if (key > this->data[mid] && mid != this->_size) {
            ++mid;
        }
        return mid;
    }

    uint quick_median(uint l, uint r) {
        uint mid = (l + r) / 2;
        if (this->data[l] > this->data[r]) {
            if (this->data[mid] > l) {
                return l;
            } else if (this->data[mid] > r) {
                return mid;
            } else {
                return r;
            }
        } else {
            if (this->data[mid] > r) {
                return r;
            } else if (this->data[mid] > l) {
                return mid;
            } else {
                return l;
            }
        }
        return mid;
    }

    uint partition(uint l, uint h) {
        T temp;
        swap(this->quick_median(l, h), h);
        T pivot = this->data[h];
        uint j = l;
        for (uint i = l; i < h; ++i) {
            if (this->data[i] <= pivot) {
                swap(i, j);
                ++j;
            }
        }
        swap(j, h);
        return j;
    }

    void quick_sort(int l, int h) {
        if (l < h) {
            uint j = this->partition(l, h);
            this->quick_sort(l, j - 1);
            this->quick_sort(j + 1, h);
        }
    }

    void quick_sort_random(int l, int h) {
        if (l < h) {
            T temp;
            swap(l + rand() % (h - l + 1), h);
            int j = this->partition(l, h);
            this->quick_sort_random(l, j - 1);
            this->quick_sort_random(j + 1, h);
        }
    }

    // Quicksort iterativo
    void quicksort() {
        int idx = 1;
        int j = 2;
        while (j < this->_size) {
            j *= 2;
            ++idx;
        }
        Index indices[2 * idx];
        idx = 0;
        indices[0].set(0, this->_size - 1);
        int l;
        int h;
        while (idx >= 0) {
            l = indices[idx].first;
            h = indices[idx].second;
            j = this->partition(l, h);
            if (l < j - 1) {
                // indices[idx++] = Index(l, j - 1);
                // indices[idx++].set(l, j - 1);
                indices[idx].set(l, j - 1);
                ++idx;
            }
            if (j + 1 < h) {
                // indices[idx++] = Index(j + 1, h);
                // indices[idx++].set(j + 1, h);
                indices[idx].set(j + 1, h);
                ++idx;
            }
            --idx;
        }
    }

    void quick_select(uint l, uint h, uint k) {
        if (k > h - l + 1) {
            return;
        }
        uint j = this->partition(l, h);
        uint length = j - l + 1;
        if (length == k) {
            return;
        } else if (length > k) {
            this->quick_select(l, j - 1, k);
        } else {
            this->quick_select(j + 1, h, k - length);
        }
    }

    void selection_sort(uint l, uint r, uint index) {
        T min = this->data[l];
        T temp;
        uint argmin;
        index += l;
        for (; l <= index; ++l) {
            min = this->data[l];
            argmin = l;
            for (uint i = l + 1; i <= r; ++i) {
                if (this->data[i] < min) {
                    min = this->data[i];
                    argmin = i;
                }
            }
            if (l < argmin) swap(l, argmin);
        }
    }

    void insertion_sort(int l, int h) {
        int j;
        for (int i = l + 1; i <= h; ++i) {
            T value = this->data[i];
            for (j = i - 1; j >= l && this->data[j] > value; --j) {
                this->data[j + 1] = this->data[j];
            }
            this->data[j + 1] = value;
        }
    }

    void insertion_sort_idx(Vetor<uint>* idx, int l, int h) {
        int j;
        for (int i = l + 1; i <= h; ++i) {
            T value = this->data[i];
            // TODO: Resolver quando j = 0, idx->at(j) recebe 0U - 1
            for (j = i - 1; j >= l && this->data[idx->at(j)] > value; --j) {
                idx->at(j + 1) = idx->at(j);
            }
            idx->at(j + 1) = i;
        }
    }

    void _heapify(uint i, uint n) {
        uint idx = i;
        uint left_idx = 2 * i + 1;
        uint right_idx = 2 * i + 2;
        T temp;
        if ((left_idx < n) && (data[left_idx] > data[idx])) {
            idx = left_idx;
        } else if ((right_idx < n) && (data[right_idx] > data[idx])) {
            idx = right_idx;
        }
        if (idx != i) {
            swap(i, idx);
            _heapify(idx, n);
        }
    }

    void heap_sort() {
        T temp;
        for (int i = this->_size / 2 - 1; i >= 0; --i) {
            this->_heapify(i, this->_size);
        }
        for (int i = this->_size - 1; i > 0; --i) {
            swap(0, i);
            this->_heapify(0, i);
        }
    }

    // Recebe uma função que será aplicada a elementos do
    // Vetor com base em elementos do Vetor de entrada
    Vetor<T> operation(const Vetor<T>& vec, T func(const T&, const T&)) const {
        if (this->_size % vec.size() == 0) {
            Vetor<T> result(this->_size);
            result.set_size(this->_size);
            // for (uint i = 0; i < this->_size; i += vec.size()) {
            //     for (uint j = 0; j < vec.size(); ++j) {
            //         result.at(i + j) = func(this->data[i + j], vec.at(j));
            //     }
            // }

            uint i = 0;
            for (uint j = 0; j < vec.size(); ++j) {
                for (uint k = 0; k < this->_size / vec.size(); ++k) {
                    result.at(i++) = func(this->data[i], vec.at(j));
                }
            }
            return result;
        } else {
            throw std::invalid_argument(
                "Broadcast não foi possível na operação. Tamanhos " \
                + std::to_string(this->_size) + " e " + \
                std::to_string(vec.size()) + " não são múltiplos.");
        }
    }

    void operation_inplace(const Vetor<T>& vec, T func(const T&, const T&)) {
        if (this->_size % vec.size() == 0) {
            uint i = 0;
            for (uint j = 0; j < vec.size(); ++j) {
                for (uint k = 0; k < this->_size / vec.size(); ++k) {
                    this->data.at(i++) = func(this->data[i], vec.at(j));
                }
            }
        } else {
            throw std::invalid_argument(
                "Broadcast não foi possível na operação. Tamanhos " \
                + std::to_string(this->_size) + " e " + \
                std::to_string(vec.size()) + " não são múltiplos.");
        }
    }

    Vetor<bool> comparison(const Vetor<T> vec, bool func(const T&, const T&)) const {
        if (this->_size % vec.size() == 0) {
            Vetor<bool> result(this->_size);
            result.set_size(this->_size);
            uint i = 0;
            for (uint j = 0; j < vec.size(); ++j) {
                for (uint k = 0; k < this->_size / vec.size(); ++k) {
                    result.at(i++) = func(this->data[i], vec.at(j));
                }
            }
            return result;
        } else {
            throw std::invalid_argument(
                "Broadcast não foi possível na operação. Tamanhos " \
                + std::to_string(this->_size) + " e " + \
                std::to_string(vec.size()) + " não são múltiplos.");
        }
    }

    static uint ideal_size(uint size) {
        uint i = 2;
        while (i < size) {
            i *= 2;
        }
        return i;
    }

 public:
    // Paulo Artur Silvério Von Knoblauch

    // Construtor padrão para Vetor vazio
    Vetor() {
        this->_capacity = 0;
        this->_size = 0;
        this->data = nullptr;
    }

    // Construtor para Vetor de capacidade especificada
    explicit Vetor(uint capacity, bool optimize = false) {
        if (optimize) {
            this->_capacity = this->ideal_size(capacity);
        } else {
            this->_capacity = capacity;
        }
        this->_size = 0;
        this->data = reinterpret_cast<T*>(operator new(_capacity * sizeof(T)));
    }

    // Inicializa a partir de um array padrão da linguagem
    Vetor(const T* data, uint size, bool optimize = false) {
        if (optimize) {
            this->_capacity = this->ideal_size(size);
        } else {
            this->_capacity = size;
        }
        this->_size = size;
        this->data = reinterpret_cast<T*>(operator new(_capacity * sizeof(T)));
        std::copy(data, data + size, this->data);
    }

    // Contrutor de Vetor que copia os valores de outro, [index, limit)
    Vetor(const Vetor<T>& other, uint capacity = 1,
          uint index = 0, uint limit = inf, bool optimize = false) {
        if (limit > other._size) {
            // Usa o tamanho do outro Vetor se não for especificado
            limit = other._size;
        }
        if (capacity < limit - index) {
            if (optimize) {
                this->_capacity = this->ideal_size(limit - index);
            } else {
                this->_capacity = limit - index;
            }
        }
        this->_size = limit - index;
        this->data = reinterpret_cast<T*>(operator new(_size * sizeof(T)));
        std::copy(other.data + index, other.data + limit, this->data);
    }

    // Permite inicialização do tipo Vetor = {1, 2, 3}
    // cppcheck-suppress noExplicitConstructor
    Vetor(const std::initializer_list<T>& l, bool optimize = false) {
        this->_size = l.size();
        if (optimize) {
            this->_capacity = this->ideal_size(this->_size);
        } else {
            this->_capacity = this->_size;
        }
        this->data = reinterpret_cast<T*>(operator new(_capacity * sizeof(T)));
        std::copy(l.begin(), l.end(), this->data);
    }

    // Inicializa o Vetor com um tamanho dado de elementos com o valor passado
    Vetor(uint size, const T& value, bool optimize = false) {
        if (optimize) {
            this->_capacity = this->ideal_size(this->_size);
        } else {
            this->_capacity = this->_size;
        }
        this->_size = size;
        this->data = reinterpret_cast<T*>(operator new(size * sizeof(T)));
        for (uint i = 0; i < size; ++i) {
            this->data[i] = value;
        }
    }

    // Construtor de "move", toma os dados do outro Vetor
    Vetor(Vetor<T>&& other) {
        this->_capacity = other._capacity;
        this->_size = other._size;
        this->data = other.data;
        other.data = nullptr;
    }

    // Destrutor que libera a memória alocada dinamicamente
    ~Vetor() {
        delete[] this->data;
    }

    uint size() const {
        return this->_size;
    }

    uint capacity() const {
        return this->_capacity;
    }

    T* raw_data() const {
        return this->data;
    }

    // Define o tamanho do Vetor dado que este seja menor que
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

    // Duplica a capacidade do Vetor quando uma inserção vai excedê-la
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

    // Aumenta a capacidade máxima do Vetor, reserva mais memória
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

    // Encolhe o tamanho do Vetor para o especificado ou não
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
            this->data = reinterpret_cast<T*>(
                operator new(this->_capacity * sizeof(T)));
        }
    }

    Vetor<T> copy() const {
        return *this;
    }

    uint where(const T& key) {
        return this->binary_search(key, 0, this->_size);
    }

    // Encontra apenas chaves existentes no Vetor, senão retorna -1
    uint contains(const T& key) const {
        uint s = 0;
        uint e = this->_size;
        if (e == 0) {
            throw std::out_of_range("Vetor vazio.");
        } else if (e == 1) {
            return key == this->data[0] ? 0 : -1;
        }
        uint mid;
        while (s < e) {
            mid = (s + e) / 2;
            if (key == this->data[mid]) {
                return mid;
            } else if (key < this->data[mid]) {
                e = mid - 1;
            } else if (key > this->data[mid]) {
                s = mid + 1;
            }
        }
        if (key < this->data[mid] && mid > 0 && key == this->data[mid - 1]) {
            return mid - 1;
        } else if (key > this->data[mid] && key == this->data[mid + 1]) {
            return mid + 1;
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
    // em que o Vetor será atualizado
    void insert(const T& value, uint index, uint limit = inf) {
        if (index > this->_capacity) {
            throw std::out_of_range("Índice " + std::to_string(index) \
                                    + " fora de alcance.");
        } else {
            // Restringe a atualização ao tamanho atual do Vetor
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

    // Copia elementos do Vetor atual e insere vários valores de uma vez
    void insert(const Vetor<uint>& indices, const Vetor<T>& values) {
        if (indices.size() != values.size()) {
            throw std::invalid_argument(
                "Índices e valores devem ter o mesmo tamanho.");
        }
        uint multiplier = 1;
        while (this->_capacity * multiplier < this->_size + indices.size()) {
            multiplier *= 2;
        }
        // Multiplica o tamanho do Vetor por 2 até que ele seja
        // suficiente para comportar todos os novos elementos
        T* new_data = reinterpret_cast<T*>(
            operator new(this->_capacity * multiplier * sizeof(T)));
        uint insert_idx = indices.size() - 1;
        uint new_size = this->_size + insert_idx;
        // Percorre o Vetor atual inserindo os valores
        for (uint i = new_size; i != inf; --i) {
            // Se o índice atual é igual ao índice de inserção de novos
            // elementos e se o Vetor desses elementos ainda não chegou ao fim,
            // insere os elementos do Vetor de valores. O índice de inserção é
            // deslocado para cada inserção feita antes dele
            if (i == indices.at(insert_idx) + insert_idx) {
                new_data[i] = values.at(insert_idx--);
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
    T pop(uint index, uint limit = inf) {
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
    // do Vetor, o último elemento é removido
    void pop(const Vetor<uint>& indices) {
        uint removal_size = indices.size();
        // Índice do Vetor em que uma mudança de elemento ocorrerá
        uint i = indices.at(0);
        // Índice do Vetor de índices
        uint j = 0;
        for (uint old_idx = indices.at(0); old_idx < this->_size; ++old_idx) {
            while (j < removal_size && old_idx == indices.at(j)) {
                ++old_idx;
                ++j;
            }
            this->data[i++] = this->data[old_idx];
        }
        this->_size -= removal_size;
    }

    void keep_only(const Vetor<uint>& indices) {
        // Índice do Vetor de índices
        uint j = 0;
        for (uint i = 0; i < this->_size && j < indices.size(); ++i) {
            if (i == indices.at(j)) {
                this->data[j++] = this->data[i];
            }
        }
        this->_size = indices.size();
    }

    // Atualiza valores no Vetor atual com valores de um outro, com base
    // em um Vetor de índices onde esses valores serão atualizados
    // TODO: Implementar broadcasting
    void update(const Vetor<uint>& indices, const Vetor<T>& values) {
        if (indices.size() != values.size()) {
            throw std::invalid_argument(
                "Índices e valores devem ter o mesmo tamanho.");
        }
        uint j = 0;
        for (uint i = 0; i < indices.size(); ++i) {
            this->data[indices.at(i)] = values.at(j++);
        }
    }

    // Copia valores de outro Vetor a partir do índice i para o
    // atual no índice j, parando no índice definido por limit
    void merge(const Vetor<T>& other, uint i = 0,
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

    // Diminui o tamanho do Vetor e retorna seu último elemento
    T pop_back() {
        if (this->_size == 0) {
            throw std::out_of_range("Vetor está vazio.");
        }
        return this->data[--this->_size];
    }

    T& back() {
        if (this->_size == 0) {
            throw std::out_of_range("Vetor está vazio.");
        }
        return this->data[this->_size - 1];
    }

    T back() const {
        if (this->_size == 0) {
            throw std::out_of_range("Vetor está vazio.");
        }
        return this->data[this->_size - 1];
    }

    // Quicksort inplace
    void sort(bool use_random_pivot = false, bool use_median = false) {
        if (this->_size <= 16) {
            this->insertion_sort(0, this->_size - 1);
            return;
        }
        if (use_median) {
            T temp;
            swap(this->argmedian(), this->_size - 1);
        }
        if (use_random_pivot) {
            srand(time(nullptr));
            this->quick_sort_random(0, this->_size - 1);
            return;
        }
        this->quick_sort(0, this->_size - 1);
    }

    // Quicksort que retorna cópia
    Vetor<T> sorted(bool use_random_pivot = false, bool use_median = false) const {
        Vetor<T> copy(*this);
        copy.sort(use_random_pivot, use_median);
        return copy;
    }

    uint argmin() const {
        if (this->_size == 0) {
            throw std::out_of_range("Vetor está vazio.");
        }
        T min = this->data[0];
        uint argmin = 0;
        for (uint i = 1; i < this->_size; ++i) {
            if (this->data[i] < min) {
                min = this->data[i];
                argmin = i;
            }
        }
        return argmin;
    }

    T min() const {
        return this->data[this->argmin()];
    }

    uint argmax() const {
        if (this->_size == 0) {
            throw std::out_of_range("Vetor está vazio.");
        }
        T max = this->data[0];
        uint argmax = 0;
        for (uint i = 1; i < this->_size; ++i) {
            if (this->data[i] > max) {
                max = this->data[i];
                argmax = i;
            }
        }
        return argmax;
    }

    T max() const {
        return this->data[this->argmax()];
    }

    uint argmedian() {
        if (this->_size == 0) {
            throw std::out_of_range("Vetor está vazio.");
        }
        Vetor<uint> indices(this->_size / 5 + 1);
        for (uint i = 0; i < this->_size - this->_size % 5; i += 5) {
            this->insertion_sort(i, i + 4);
            indices.push_back(i + 2);
        }

        Vetor<uint> keep(indices.size() / 5 + 1);
        while (indices.size() >= 5) {
            for (uint i = 0; i < indices.size() - indices.size() % 5; i += 5) {
                this->insertion_sort_idx(&indices, i, i + 4);
                keep.push_back(i + 2);
            }
            indices.keep_only(keep);
            keep.clear();
        }
        this->insertion_sort_idx(&indices, 0, indices.size() - 1);
        return indices[indices.size() / 2];
    }

    T median() {
        return this->data[this->argmedian()];
    }

    void select(uint i) {
        this->quick_select(0, this->_size - 1, i + 1);
    }

    // TODO: fazer isso funcionar de fato
    void heapify() {
        this->heap_sort();
    }

    // Operador para acessar os dados no array interno
    T& operator[](int index) {
        if (index < 0) {
            index += this->_size;
        }
        if (index >= this->_size || index < 0) {
            throw std::out_of_range("Índice " + std::to_string(index) + \
            " fora de alcance, tamanho do Vetor é " + \
            std::to_string(this->_size) + ".");
        }
        return this->data[index];
    }

    // Mesmo operador anterior, mas para quando o Vetor não pode ser alterado
    T operator[](int index) const {
        if (index < 0) {
            index += this->_size;
        }
        if (index >= this->_size || index < 0) {
            throw std::out_of_range("Índice " + std::to_string(index) + \
            " fora de alcance, tamanho do Vetor é " + \
            std::to_string(this->_size) + ".");
        }
        return this->data[index];
    }

    Vetor<T> operator[](const Vetor<uint>& indices) const {
        Vetor<T> result(indices.size());
        result.set_size(indices.size());
        int j = 0;
        for (uint i = 0; i < indices.size(); ++i) {
            result.at(j++) = this->data[indices.at(i)];
        }
        return result;
    }

    Vetor<T> operator[](const Vetor<bool>& indices) const {
        if (this->_size != indices.size()) {
            throw std::invalid_argument(
                "Vetores devem possuir o mesmo tamanho. Tamanho " + \
                std::to_string(this->_size) + " != " + \
                std::to_string(indices.size()) + ".");
        }
        Vetor<T> result(8);
        for (int i = 0; i < indices.size(); ++i) {
            if (indices.at(i)) {
                result.push_back(this->data[i]);
            }
        }
        return result;
    }

    explicit operator bool() const {
        return this->_size > 0;
    }

    // Alternativa ao operador [] para acessar elementos com um método
    // sem "bounds checking"

    T& at(uint index) {
        return this->data[index];
    }

    T at(uint index) const {
        return this->data[index];
    }

    Vetor<T> at(uint i, uint j) const {
        if (i > j || j > this->_size) {
            throw std::out_of_range("Índices " + std::to_string(i) + \
            " e/ou " + std::to_string(j) + " fora de alcance.");
        }
        Vetor<T> result = Vetor<T>(*this, 0, i, j);
        return result;
    }

    void set(uint index, const T& value) {
        if (index >= this->_size || index < 0) {
            throw std::out_of_range("Índice " + std::to_string(index) + \
            " fora de alcance, tamanho do Vetor é " + \
            std::to_string(this->_size) + ".");
        }
        this->data[index] = value;
    }

    // Operadores matemáticos

    Vetor<T>& operator+=(const T& value) {
        for (int i = 0; i < this->_size; ++i) {
            this->data[i] += value;
        }
        return *this;
    }

    Vetor<T>& operator-=(const T& value) {
        for (int i = 0; i < this->_size; ++i) {
            this->data[i] -= value;
        }
        return *this;
    }

    Vetor<T>& operator*=(const T& value) {
        for (int i = 0; i < this->_size; ++i) {
            this->data[i] *= value;
        }
        return *this;
    }

    Vetor<T>& operator/=(const T& value) {
        for (int i = 0; i < this->_size; ++i) {
            this->data[i] /= value;
        }
        return *this;
    }

    Vetor<T>& operator+=(const Vetor<T>& vec) {
        this->operation_inplace(vec, [](
            const T& a, const T& b) { return a + b; });
        return *this;
    }

    Vetor<T>& operator-=(const Vetor<T>& vec) {
        this->operation_inplace(vec, [](
            const T& a, const T& b) { return a - b; });
        return *this;
    }

    Vetor<T>& operator*=(const Vetor<T>& vec) {
        this->operation_inplace(vec, [](
            const T& a, const T& b) { return a * b; });
        return *this;
    }

    Vetor<T>& operator/=(const Vetor<T>& vec) {
        this->operation_inplace(vec, [](
            const T& a, const T& b) { return a / b; });
        return *this;
    }

    Vetor<T> operator+(const T& value) const {
        Vetor<T> result(*this);
        for (int i = 0; i < this->_size; ++i) {
            result.at(i) += value;
        }
        return result;
    }

    Vetor<T> operator-(const T& value) const {
        Vetor<T> result(*this);
        for (int i = 0; i < this->_size; ++i) {
            result.at(i) -= value;
        }
        return result;
    }

    Vetor<T> operator*(const T& value) const {
        Vetor<T> result(*this);
        for (int i = 0; i < this->_size; ++i) {
            result.at(i) *= value;
        }
        return result;
    }

    Vetor<T> operator/(const T& value) const {
        Vetor<T> result(*this);
        for (int i = 0; i < this->_size; ++i) {
            result.at(i) /= value;
        }
        return result;
    }

    Vetor<T> operator%(const T& value) const {
        Vetor<T> result(*this);
        for (int i = 0; i < this->_size; ++i) {
            result.at(i) %= value;
        }
        return result;
    }

    Vetor<T> operator+(const Vetor<T>& vec) const {
        return this->operation(vec, [](
            const T& a, const T& b) { return a + b; });
    }

    Vetor<T> operator-(const Vetor<T>& vec) const {
        return this->operation(vec, [](
            const T& a, const T& b) { return a - b; });
    }

    Vetor<T> operator*(const Vetor<T>& vec) const {
        return this->operation(vec, [](
            const T& a, const T& b) { return a * b; });
    }

    Vetor<T> operator/(const Vetor<T>& vec) const {
        return this->operation(vec, [](
            const T& a, const T& b) { return a / b; });
    }

    Vetor<T> operator%(const Vetor<T>& vec) const {
        return this->operation(vec, [](
            const T& a, const T& b) { return a % b; });
    }

    // Operadores de comparação

    Vetor<T>& operator=(const Vetor<T>& other) {
        this->_capacity = other._capacity;
        this->_size = other._size;
        delete[] this->data;
        this->data = reinterpret_cast<T*>(operator new(other.capacity() * sizeof(T)));
        std::copy(other.raw_data(), other.raw_data() + other.capacity(), this->data);
        return *this;
    }

    Vetor<T>& operator=(Vetor<T>&& other) {
        this->_capacity = other._capacity;
        this->_size = other._size;
        delete[] this->data;
        this->data = other.data;
        other.data = nullptr;
        return *this;
    }

    Vetor<bool> operator==(const T& value) const {
        Vetor<bool> result(this->_size);
        result.set_size(this->_size);
        for (int i = 0; i < this->_size; ++i) {
            result.at(i) = this->data[i] == value;
        }
        return result;
    }

    Vetor<bool> operator!=(const T& value) const {
        Vetor<bool> result(this->_size);
        result.set_size(this->_size);
        for (int i = 0; i < this->_size; ++i) {
            result.at(i) = this->data[i] != value;
        }
        return result;
    }

    Vetor<bool> operator<(const T& value) const {
        Vetor<bool> result(this->_size);
        result.set_size(this->_size);
        for (int i = 0; i < this->_size; ++i) {
            result.at(i) = this->data[i] < value;
        }
        return result;
    }

    Vetor<bool> operator>(const T& value) const {
        Vetor<bool> result(this->_size);
        result.set_size(this->_size);
        for (int i = 0; i < this->_size; ++i) {
            result.at(i) = this->data[i] > value;
        }
        return result;
    }

    Vetor<bool> operator<=(const T& value) const {
        Vetor<bool> result(this->_size);
        result.set_size(this->_size);
        for (int i = 0; i < this->_size; ++i) {
            result.at(i) = this->data[i] <= value;
        }
        return result;
    }

    Vetor<bool> operator>=(const T& value) const {
        Vetor<bool> result(this->_size);
        result.set_size(this->_size);
        for (int i = 0; i < this->_size; ++i) {
            result.at(i) = this->data[i] >= value;
        }
        return result;
    }

    Vetor<bool> operator==(const Vetor<T>& vec) const {
        return this->comparison(vec, [](
            const T& a, const T& b) { return a == b; });
    }

    Vetor<bool> operator!=(const Vetor<T>& vec) const {
        return this->comparison(vec, [](
            const T& a, const T& b) { return a != b; });
    }

    Vetor<bool> operator<(const Vetor<T>& vec) const {
        return this->comparison(vec, [](
            const T& a, const T& b) { return a < b; });
    }

    Vetor<bool> operator>(const Vetor<T>& vec) const {
        return this->comparison(vec, [](
            const T& a, const T& b) { return a > b; });
    }

    Vetor<bool> operator<=(const Vetor<T>& vec) const {
        return this->comparison(vec, [](
            const T& a, const T& b) { return a <= b; });
    }

    Vetor<bool> operator>=(const Vetor<T>& vec) const {
        return this->comparison(vec, [](
            const T& a, const T& b) { return a >= b; });
    }

    void print() const {
        if (this->_size == 0) {
            std::cout << "[ ]";
            return;
        }
        std::cout << "[ ";
        for (int i = 0; i < this->_size - 1; ++i) {
            std::cout << this->data[i] << ", ";
        }
        std::cout << this->data[this->_size - 1] << " ]" << std::endl;
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

        bool operator!=(const Iterator& other) const {
            return this->ptr != other.ptr;
        }
    };

    Iterator begin() {
        return Iterator(this->data);
    }

    Iterator end() {
        return Iterator(this->data + this->_size);
    }

    // Encontra todas as ocorrências de uma sequência dentro do Vetor
    Vetor<uint> sequence(const Vetor<T>& seq) const {
        Vetor<uint> result;
        Vetor<uint> subseq;
        uint j = 0;
        bool is_progressing = false;
        for (uint i = 0; i < this->_size; ++i) {
            if (this->data[i] == seq.at(j)) {
                if (j != 0 && this->data[i] == seq.at(0)) {
                    subseq.push_back(i);
                }
                if (is_progressing) {
                    if (j == seq.size() - 1) {
                        j = 0;
                        is_progressing = false;
                        subseq.clear(false);
                    } else {
                        ++j;
                    }
                } else {
                    result.push_back(i);
                    is_progressing = true;
                    ++j;
                }
            } else {
                if (is_progressing) {
                    if (subseq) {
                        j -= subseq.at(0) - result.back();
                        result.back() = subseq.pop(0);
                    } else {
                        result.pop_back();
                        j = 0;
                        is_progressing = false;
                    }
                    --i;
                }
            }
        }
        if (is_progressing) result.pop_back();
        return result;
    }
};

// Cria Vetor a partir de cstring
Vetor<char> operator""_A(const char* str, size_t size) {
    Vetor<char> vec(str, size);
    return vec;
}

// Cria Vetor a partir de cstring com tamanho múltiplo de 2 para otimização
Vetor<char> operator""_AO(const char* str, size_t size) {
    Vetor<char> vec(str, size, true);
    return vec;
}

std::ostream& operator<<(std::ostream& os, const Vetor<char>& vec) {
    for (int i = 0; i < vec.size(); ++i) {
        os << vec.at(i);
    }
    return os;
}

// Cria vetor com os números dispostos em um intervalo dado

Vetor<int> arange(int start, int stop, int step = 1) {
    Vetor<int> result = Vetor<int>();
    if (step > 0 && stop > start) {
        result = Vetor<int>(((stop - start - 1) / step) + 1);
        for (int i = start; i < stop; i += step) {
            result.push_back(i);
        }
    } else if (step < 0 && stop < start) {
        result = Vetor<int>((start - stop - 1) / -step) + 1;
        for (int i = start; i > stop; i += step) {
            result.push_back(i);
        }
    }
    return result;
}

Vetor<uint> arange(uint start, uint stop, uint step = 1) {
    Vetor<uint> result = Vetor<uint>();
    if (stop > start && step > 0) {
        result = Vetor<uint>(((stop - start - 1) / step) + 1);
        for (uint i = start; i < stop; i += step) {
            result.push_back(i);
        }
    }
    return result;
}

Vetor<float> arange(float start, float stop, float step = 1.0) {
    Vetor<float> result = Vetor<float>();
    if (step > 0 && stop > start) {
        result = Vetor<float>(((stop - start - 1) / step) + 1);
        for (float i = start; i < stop; i += step) {
            result.push_back(i);
        }
    } else if (step < 0 && stop < start) {
        result = Vetor<float>((start - stop - 1) / -step) + 1;
        for (float i = start; i > stop; i += step) {
            result.push_back(i);
        }
    }
    return result;
}

// Cria um vetor de números aleatórios no intervalo especificado

Vetor<int> random_vector(uint size, int start, int end) {
    Vetor<int> result = Vetor<int>(size);
    result.set_size(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(start, end);
    for (uint i = 0; i < size; ++i) {
        result.at(i) = distrib(gen);
    }
    return result;
}

Vetor<uint> random_vector(uint size, uint start, uint end) {
    Vetor<uint> result = Vetor<uint>(size);
    result.set_size(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint> distrib(start, end);
    for (uint i = 0; i < size; ++i) {
        result.at(i) = distrib(gen);
    }
    return result;
}

Vetor<float> random_vector(uint size, float start, float end) {
    Vetor<float> result = Vetor<float>(size);
    result.set_size(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distrib(start, end);
    for (uint i = 0; i < size; ++i) {
        result.at(i) = distrib(gen);
    }
    return result;
}

// Valor absoluto

Vetor<int> abs(const Vetor<int>& vec) {
    Vetor<int> result(vec.size());
    result.set_size(vec.size());
    for (int i = 0; i < vec.size(); ++i) {
        result.at(i) = abs(vec.at(i));
    }
    return result;
}

Vetor<float> abs(const Vetor<float>& vec) {
    Vetor<float> result(vec.size());
    result.set_size(vec.size());
    for (int i = 0; i < vec.size(); ++i) {
        result.at(i) = abs(vec.at(i));
    }
    return result;
}

// Potência

Vetor<int> power(const Vetor<int>& vec, int value) {
    Vetor<int> result(vec.size());
    result.set_size(vec.size());
    for (int i = 0; i < vec.size(); ++i) {
        result.at(i) = std::pow(vec.at(i), value);
    }
    return result;
}

Vetor<float> power(const Vetor<float>& vec, float value) {
    Vetor<float> result(vec.size());
    result.set_size(vec.size());
    for (int i = 0; i < vec.size(); ++i) {
        result.at(i) = std::pow(vec.at(i), value);
    }
    return result;
}

// Operador de negativo (?)

Vetor<int> neg(const Vetor<int>& vec) {
    Vetor<int> result(vec.size());
    result.set_size(vec.size());
    for (int i = 0; i < vec.size(); ++i) {
        result.at(i) = -vec.at(i);
    }
    return result;
}

Vetor<float> neg(const Vetor<float>& vec) {
    Vetor<float> result(vec.size());
    result.set_size(vec.size());
    for (int i = 0; i < vec.size(); ++i) {
        result.at(i) = -vec.at(i);
    }
    return result;
}

// Compara os pares de elementos no mesmo índice de dois Vetores de booleanos
// e retorna true se ambos forem true
Vetor<bool> _and(const Vetor<bool>& vec, const Vetor<bool>& vec2) {
    Vetor<bool> result(vec.size());
    result.set_size(vec.size());
    for (int i = 0; i < vec.size(); ++i) {
        result.at(i) = vec.at(i) && vec2.at(i);
    }
    return result;
}

// Retorna true para cada par se pelo menos um dos elementos for true
Vetor<bool> _or(const Vetor<bool>& vec, const Vetor<bool>& vec2) {
    Vetor<bool> result(vec.size());
    result.set_size(vec.size());
    for (int i = 0; i < vec.size(); ++i) {
        result.at(i) = vec.at(i) || vec2.at(i);
    }
    return result;
}

// Retorna true apenas se todos os elementos de um vetor forem true
bool _all(const Vetor<bool>& vec) {
    for (int i = 0; i < vec.size(); ++i) {
        if (!vec.at(i)) {
            return false;
        }
    }
    return true;
}

// Retorna true se qualquer elemento do Vetor for true
bool _any(const Vetor<bool>& vec) {
    for (int i = 0; i < vec.size(); ++i) {
        if (vec.at(i)) {
            return true;
        }
    }
    return false;
}

// Para cada elemento do Vetor, retorna true se é false e vice-versa
Vetor<bool> _invert(const Vetor<bool>& vec) {
    Vetor<bool> result(vec.size());
    result.set_size(vec.size());
    for (int i = 0; i < vec.size(); ++i) {
        result.at(i) = !vec.at(i);
    }
    return result;
}

// Retorna a quantidade de elementos true no Vetor
uint _count(const Vetor<bool>& vec) {
    int result = 0;
    for (int i = 0; i < vec.size(); ++i) {
        if (vec.at(i)) {
            ++result;
        }
    }
    return result;
}

// Encontra os k elementos mais próximos do índice dado; O(n * k)
Vetor<int> k_closest(const Vetor<int>& vec, uint k, uint x) {
    if (k >= vec.size()) {
        return Vetor<int>(vec);
    }
    int value = vec.at(x);
    // Separa k elementos e os mantém para comparação com os próximos
    Vetor<int> result = vec.at(0, k);
    // Obtém os valores absolutos da diferença em relação ao elemento principal
    Vetor<int> differences = abs(result - value);
    uint max_idx = differences.argmax();
    int threshold = differences.at(max_idx);
    int diff;
    for (uint i = 0; i < vec.size(); ++i) {
        if (i != x) {
            diff = std::abs(vec.at(i) - value);
            // Se a diferença é menor que a maior das diferenças, atualiza
            // o valor da diferença e o elemento no Vetor resultante
            if (diff < threshold) {
                result.at(max_idx) = vec.at(i);
                differences.at(max_idx) = diff;
                max_idx = differences.argmax();
                threshold = differences.at(max_idx);
            }
        }
    }
    return result;
}

// Encontra os k elementos mais próximos do índice em um Vetor ordenado; O(k)
Vetor<int> k_closest_ordered(const Vetor<int>& vec, uint k, uint x) {
    if (x >= vec.size()) {
        throw std::out_of_range("Índice " + std::to_string(x) + \
            " fora de alcance, tamanho do Vetor é " + \
            std::to_string(vec.size()) + ".");
    }
    if (k >= vec.size()) {
        return Vetor<int>(vec);
    }
    if (x == 0) {
        return vec.at(1, k + 1);
    } else if (x == vec.size() - 1) {
        return vec.at(vec.size() - k - 1, vec.size() - 1);
    }
    Vetor<int> result(k);
    int value = vec.at(x);
    uint left = x - 1;
    uint right = x + 1;
    int left_value = std::abs(vec.at(left) - value);
    int right_value = std::abs(vec.at(right) - value);
    do {
        // Se não há mais elementos para a esquerda, preenche o resultado com
        // o restante dos elementos à direita e talvez o único à esquerda
        if (left == 0) {
            uint i = result.size();
            for (; i < k; ++i) {
                if (left_value <= right_value) {
                    result.push_back(vec.at(left));
                    ++i;
                    break;
                } else {
                    result.push_back(vec.at(right));
                    right_value = std::abs(vec.at(++right) - value);
                }
            }
            for (; i < k; ++i) {
                result.push_back(vec.at(right++));
            }
            break;
        }
        if (right == vec.size() - 1) {
            uint i = result.size();
            for (; i < k; ++i) {
                if (right_value <= left_value) {
                    result.push_back(vec.at(right));
                    ++i;
                    break;
                } else {
                    result.push_back(vec.at(left));
                    left_value = std::abs(vec.at(--left) - value);
                }
            }
            for (; i < k; ++i) {
                result.push_back(vec.at(left--));
            }
            break;
        }
        if (left_value < right_value) {
            result.push_back(vec.at(left));
            left_value = std::abs(vec.at(--left) - value);
        } else {
            result.push_back(vec.at(right));
            right_value = std::abs(vec.at(++right) - value);
        }
    } while (result.size() < k);
    return result;
}

// Encontra o intervalo cujos valores somados compõem a maior soma possível
// O(n)?
Vetor<int> greatest_range(const Vetor<int>& vec) {
    long long int value = vec.at(0);
    Vetor<long long int> sum;
    sum.push_back(value);
    Vetor<int> indices;
    indices.push_back(0);
    bool positive = value >= 0;
    uint argmax = 0;
    for (uint i = 1; i < vec.size(); ++i) {
        if (vec.at(i) == 0) {
            continue;
        } else if (vec.at(i) < 0) {
            if (positive) {
                positive = false;
                indices.push_back(i);
                if (sum.back() > sum.at(argmax)) {
                    argmax = sum.size() - 1;
                }
                sum.push_back(vec.at(i));
            } else {
                sum.back() += vec.at(i);
            }
        } else {
            if (positive) {
                sum.back() += vec.at(i);
            } else {
                indices.push_back(i);
                sum.push_back(vec.at(i));
                positive = true;
            }
        }
    }
    indices.push_back(vec.size());
    // Se todos os elementos são negativos, retorna o maior deles
    if (sum.size() == 1 && sum.at(0) < 0) {
        int max = vec.argmax();
        return {max, max + 1};
    }
    int i = 0;
    int j = sum.size() - 1;
    value = sum.at(argmax);
    while (j > i) {
        if (sum.at(i) > value) {
            value = sum.at(i);
            argmax = i;
        } else {
            if (sum.at(i) > 0) {
                sum.at(i + 1) += sum.at(i);
                indices.at(i + 1) = indices.at(i);
                if (i + 1 == j) {
                    break;
                }
            }
            ++i;
        }
        if (sum.at(j) > value) {
            value = sum.at(j);
            argmax = j;
        } else {
            if (sum.at(j) > 0) {
                sum.at(j - 1) += sum.at(j);
                indices.at(j - 1) = indices.at(j);
                if (j - 1 == i) {
                    break;
                }
            }
            --j;
        }
    }
    if (sum.at(i) > value) {
        value = sum.at(i);
        argmax = i;
    }
    return {indices.at(argmax), indices.at(argmax + 1)};
}

Vetor<int> greatest_range2(const Vetor<int>& vec) {
    if (!vec) {
        throw std::invalid_argument("Vetor vazio.");
    }
    uint start = 0;
    uint end = 1;
    uint test_start = 1;
    uint test_end = 1;
    long long int max = vec.at(0);
    long long int sum = max;
    bool positive = sum >= 0;
    for (uint i = 1; i < vec.size(); ++i) {
        if (vec.at(i) < 0) {
            if (positive) {
                positive = false;
                if (sum > max) {
                    end = i;
                }
            } else {
                sum += vec.at(i);
            }
        } else {
            if (positive) {
                sum += vec.at(i);
                ++test_end;
            } else {
                test_start = i;
            }
        }
    }
    return {0, 1};
}

struct IntHash {
    IntHash(const Vetor<int>& order) {
        
    }
};

Vetor<int> order_using(const Vetor<int>& vec, const Vetor<int>& order) {
    Vetor<int> result(vec.size());
    return result;
}

// https://www.geeksforgeeks.org/heap-sort/

#endif  // VETOR_HPP_
