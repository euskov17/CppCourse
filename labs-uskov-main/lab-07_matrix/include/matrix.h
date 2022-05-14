#ifndef LAB_08_MATRIX_H
#define LAB_08_MATRIX_H

#include <iostream>
#include <cstdio>
#include <cstddef>
#include <fstream>
#include <string.h>

class Matrix
{
public:
  Matrix(std::size_t r, std::size_t c);
  Matrix(const Matrix &);
  ~Matrix();

  std::size_t get_rows() const;
  std::size_t get_cols() const;

  void set(std::size_t i, std::size_t j, int val);
  int get(std::size_t i, std::size_t j) const;
  void print(FILE *f) const;

  Matrix &operator=(const Matrix &m);
  Matrix operator+(const Matrix &m) const;
  Matrix operator-(const Matrix &m) const;
  Matrix operator*(const Matrix &m) const;

  Matrix &operator+=(const Matrix &m);
  Matrix &operator-=(const Matrix &m);
  Matrix &operator*=(const Matrix &m);

  bool operator==(const Matrix &m) const;
  bool operator!=(const Matrix &m) const;

private:
  bool check_index(std::size_t i, std::size_t j) const;
  bool iterator(const Matrix &other, int (Matrix::*foo)(const Matrix &other, std::size_t i, std::size_t j) const) const;
  bool iterator(const Matrix &other, int (Matrix::*foo)(const Matrix &other, std::size_t i, std::size_t j));
  int sum_el(const Matrix &other, std::size_t i, std::size_t j);
  int dif_el(const Matrix &other, std::size_t i, std::size_t j);
  int equal_el(const Matrix &other, std::size_t i, std::size_t j) const;
  void swap(Matrix &other);
  std::size_t _rows;
  std::size_t _cols;
  int **_data;
};

#endif
