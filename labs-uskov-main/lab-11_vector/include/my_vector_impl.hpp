#ifndef MY_VECTOR_IMPL_H_
#define MY_VECTOR_IMPL_H_

#include <cstddef>
#include <iostream>

namespace containers
{
    template <typename T>
    my_vector<T>::my_vector()
    {
        capacity_ = 1;
        size_ = 0;
        data_ = new char[sizeof(T) * capacity_];
        array_ = (T *)data_;
    }

    template <typename T>
    my_vector<T>::my_vector(const my_vector &other)
    {
        capacity_ = other.capacity_;
        size_ = other.size_;
        data_ = new char[sizeof(T) * capacity_];
        array_ = (T *)data_;
        for (size_t i = 0; i < size_; ++i)
            new (array_ + i) T(*(other.array_ + i));
    }

    template <typename T>
    void my_vector<T>::swap(my_vector &other)
    {
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(data_, other.data_);
        std::swap(array_, other.array_);
    }

    template <typename T>
    my_vector<T> &my_vector<T>::operator=(const my_vector &other)
    {
        if (&other == this)
            return *this;
        my_vector copy(other);
        swap(copy);
        return *this;
    }

    template <typename T>
    my_vector<T>::~my_vector()
    {
        this->clear();
        delete[] data_;
    }

    template <typename T>
    std::size_t my_vector<T>::size() const
    {
        return size_;
    }

    template <typename T>
    std::size_t my_vector<T>::capacity() const
    {
        return capacity_;
    }

    template <typename T>
    bool my_vector<T>::operator==(const my_vector &other) const
    {
        if (size_ != other.size_)
            return false;
        if (capacity_ != other.capacity_)
            return false;
        for (size_t i = 0; i < size_; ++i)
            if ((*this)[i] != other[i])
                return false;
        return true;
    }

    template <typename T>
    bool my_vector<T>::operator!=(const my_vector &other) const
    {
        return !(*this == other);
    }

    template <typename T>
    bool my_vector<T>::empty() const
    {
        return (size_) ? false : true;
    }

    template <typename T>
    void my_vector<T>::reserve(std::size_t n)
    {
        if (n <= capacity_)
            return;
        capacity_ = calc_cap(n);
        char *new_ar = new char[sizeof(T) * capacity_];
        T *old_array_ = array_;
        array_ = (T *)new_ar;
        for (size_t i = 0; i < size_; ++i)
        {
            new (array_ + i) T(*(old_array_ + i));
            (old_array_ + i)->~T();
        }
        std::swap(data_, new_ar);
        delete[] new_ar;
    }

    template <typename T>
    T &my_vector<T>::operator[](std::size_t index) const
    {
        return *(array_ + index);
    }

    template <typename T>
    void my_vector<T>::push_back(const T &t)
    {
        if (size_ == capacity_)
            reserve(size_ + 1);
        new (array_ + (size_++)) T(t);
    }

    template <typename T>
    void my_vector<T>::pop_back()
    {
        if (size_)
            (array_ + (--size_))->~T();
    }

    template <typename T>
    void my_vector<T>::clear()
    {
        destruct_block(0, size_);
        size_ = 0;
    }

    template <typename T>
    my_vector<T>::my_vector(std::size_t n)
    {
        capacity_ = calc_cap(n);
        size_ = n;
        data_ = new char[sizeof(T) * capacity_];
        array_ = (T *)data_;
        default_init_(0, size_);
    }

    template <typename T>
    void my_vector<T>::resize(std::size_t n)
    {
        size_t old_size = size_;
        reserve(n);
        size_ = n;
        destruct_block(size_, old_size);
        default_init_(old_size, size_);
    }

    template <typename T>
    std::ostream &operator<<(std::ostream &os, const my_vector<T> &vect)
    {
        for (size_t i = 0; i < vect.size(); ++i)
        {
            os << vect[i];
            if (i != vect.size() - 1)
                os << " ";
        }
        return os;
    }

    template <typename T>
    void my_vector<T>::destruct_block(size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
            (array_ + i)->~T();
    }

    template <typename T>
    void my_vector<T>::default_init_(size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
            new (array_ + i) T();
    }

    template <typename T>
    size_t my_vector<T>::calc_cap(size_t n)
    {
        size_t res = 1;
        while ((res *= 2) < n)
            ;
        return res;
    }
}

#endif