#ifndef MY_VECTOR_H_
#define MY_VECTOR_H_

#include <cstddef>
#include <iostream>

namespace containers {

template<typename T>
class my_vector {
public:
    my_vector();
    explicit my_vector(std::size_t n);
    explicit my_vector(const my_vector &other);
    my_vector& operator=(const my_vector &other);
    ~my_vector();

    std::size_t size() const;
    std::size_t capacity() const;

    bool empty() const;
    bool operator==(const my_vector &)const;
    bool operator!=(const my_vector &)const;

    void resize(std::size_t n);
    void reserve(std::size_t n);

    T &operator[](std::size_t index) const;

    void push_back(const T &t);
    void pop_back();
    void clear();

private:
    void swap(my_vector &);
    size_t calc_cap(size_t n);
    void destruct_block(size_t start, size_t end);
    void default_init_(size_t start, size_t end);
    
    size_t capacity_;
    size_t size_;
    char *data_;
    T* array_;
};
}

#include "my_vector_impl.hpp"

#endif
