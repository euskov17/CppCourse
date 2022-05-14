#include <cstdio>
#include <cstring>
#include <utility>
#include <algorithm>
#include "my_vector.hpp"

MyVector::MyVector(std::size_t init_capacity = 2)
{
    _size = 0;
    _capacity = init_capacity;
    _data = new int[_capacity];
};

MyVector::MyVector() : MyVector(2){};

MyVector::~MyVector()
{
    delete[] _data;
};

void MyVector::set(std::size_t index, int value)
{
    _data[index] = value;
};

int MyVector::get(std::size_t index)
{
    return _data[index];
};

std::size_t MyVector::size()
{
    return _size;
};

std::size_t MyVector::capacity()
{
    return _capacity;
};

void MyVector::reserve(std::size_t new_capacity)
{
    if (new_capacity <= _capacity)
        return;
    _capacity = new_capacity;
    int *new_data = new int[new_capacity];
    memcpy(new_data, _data, _size * sizeof(int));
    delete[] _data;
    _data = new_data;
};

void MyVector::resize(std::size_t new_size)
{
    if (new_size > _capacity)
    {
        reserve(std::max(_capacity * 2, new_size));
    }
    for (std::size_t i = _size; i < new_size; i++)
        set(i, 0);
    _size = new_size;
};

void MyVector::push_back(int value)
{
    resize(_size + 1);
    set(_size - 1, value);
};

void MyVector::insert(std::size_t index, int value)
{
    resize(_size + 1);
    for (std::size_t i = index; i < _size; ++i)
    {
        int cur = get(i);
        set(i, value);
        value = cur;
    }
};

void MyVector::erase(std::size_t index)
{
    for (std::size_t i = index + 1; i < _size; ++i)
    {
        int cur = get(i);
        set(i - 1, cur);
    }
    _size--;
};

MyVector::MyVector(const MyVector &other)
{
    _size = other._size;
    _capacity = other._capacity;
    _data = new int[other._capacity];
    std::memcpy(_data, other._data, other._size * sizeof(int));
};

MyVector &MyVector::operator=(const MyVector &other)
{
    if (this == &other)
        return *this;
    MyVector tmp(other);
    swap(tmp);
    return *this;
};

void MyVector::swap(MyVector &other)
{
    std::swap(_size, other._size);
    std::swap(_capacity, other._capacity);
    std::swap(_data, other._data);
}