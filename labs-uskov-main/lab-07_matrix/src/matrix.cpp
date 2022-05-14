#include "matrix.h"

Matrix::Matrix(std::size_t r, std::size_t c)
{
    _rows = r;
    _cols = c;
    _data = new int *[r];
    int *array = new int[c * r];
    for (std ::size_t i = 0; i < c * r; ++i)
        array[i] = 0;
    for (std::size_t i = 0; i < r; ++i)
        _data[i] = &array[c * i];
}

Matrix::~Matrix()
{
    delete[] _data[0];
    delete[] _data;
}

Matrix::Matrix(const Matrix &other) : Matrix(other._rows, other._cols)
{
    for (std::size_t i = 0; i < _rows; ++i)
    {
        memcpy(_data[i], other._data[i], _cols * sizeof(int));
    }
}

std::size_t Matrix::get_rows() const { return _rows; }
std::size_t Matrix::get_cols() const { return _cols; }

bool Matrix::check_index(std::size_t i, std::size_t j) const
{
    if (i >= _rows || i < 0 || j >= _cols || j < 0)
    {
        fprintf(stderr, "Index out of range\n");
        return false;
    }
    return true;
}

void Matrix::set(std::size_t i, std::size_t j, int val)
{
    if (check_index(i, j))
        _data[i][j] = val;
}

int Matrix::get(std::size_t i, std::size_t j) const
{
    if (check_index(i, j))
        return _data[i][j];
    return INT32_MIN;
}

bool Matrix::iterator(const Matrix &other, int (Matrix::*foo)(const Matrix &other, std::size_t i, std::size_t j))
{
    for (std::size_t i = 0; i < _rows; ++i)
        for (std ::size_t j = 0; j < _cols; ++j)
            if ((this->*foo)(other, i, j))
                return false;
    return true;
}

bool Matrix::iterator(const Matrix &other, int (Matrix::*foo)(const Matrix &other, std::size_t i, std::size_t j) const) const
{
    for (std::size_t i = 0; i < _rows; ++i)
        for (std ::size_t j = 0; j < _cols; ++j)
            if ((this->*foo)(other, i, j))
                return false;
    return true;
}

void Matrix::print(FILE *f) const
{
    for (std::size_t i = 0; i < _rows; ++i)
    {
        fprintf(f, "%d", _data[i][0]);
        for (std::size_t j = 1; j < _cols; ++j)
            fprintf(f, " %d", _data[i][j]);
        fprintf(f, "\n");
    }
}

int Matrix::sum_el(const Matrix &other, std::size_t i, std::size_t j)
{
    if (check_index(i, j))
    {
        set(i, j, _data[i][j] + other._data[i][j]);
        return 0;
    }
    return 1;
}

int Matrix::dif_el(const Matrix &other, std::size_t i, std::size_t j)
{
    if (check_index(i, j))
    {
        set(i, j, _data[i][j] - other._data[i][j]);
        return 0;
    }
    return 1;
}

int Matrix::equal_el(const Matrix &other, std::size_t i, std::size_t j) const
{
    if (check_index(i, j) && _data[i][j] == other._data[i][j])
        return 0;
    return 1;
}

bool Matrix::operator==(const Matrix &other) const
{
    return (bool)iterator(other, &Matrix::equal_el);
}

bool Matrix::operator!=(const Matrix &other) const
{
    return 1 - iterator(other, &Matrix::equal_el);
}

void Matrix::swap(Matrix &other)
{
    std::swap(_rows, other._rows);
    std::swap(_cols, other._cols);
    std::swap(_data, other._data);
}

Matrix &Matrix::operator=(const Matrix &other)
{
    if (this == &other)
        return *this;
    Matrix tmp(other);
    swap(tmp);
    return *this;
}

Matrix &Matrix::operator+=(const Matrix &other)
{
    if (!iterator(other, &Matrix::sum_el))
        return *this;
    fprintf(stderr, "Operator += error\n");
    return *this;
}

Matrix &Matrix::operator-=(const Matrix &other)
{
    if (!iterator(other, &Matrix::dif_el))
        return *this;
    fprintf(stderr, "Operator += error\n");
    return *this;
}

Matrix &Matrix::operator*=(const Matrix &other)
{
    Matrix a = *this * other;
    swap(a);
    return *this;
}

Matrix Matrix::operator+(const Matrix &other) const
{
    Matrix a(*this);
    a += other;
    return a;
}

Matrix Matrix::operator-(const Matrix &other) const
{
    Matrix a(*this);
    a -= other;
    return a;
}

Matrix Matrix::operator*(const Matrix &other) const
{
    Matrix a(_rows, other._cols);
    for (std::size_t i = 0; i < _rows; ++i)
        for (std::size_t j = 0; j < other._cols; ++j)
        {
            int value = 0;
            for (std::size_t k = 0; k < _cols; ++k)
                value += _data[i][k] * other._data[k][j];
            a.set(i, j, value);
        }
    return a;
}
