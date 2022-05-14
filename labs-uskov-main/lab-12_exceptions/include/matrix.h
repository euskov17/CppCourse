#pragma once

#include <iostream>
#include <fstream>
#include <exception>

namespace MatrixException{
	class MatrixException : public std:: exception{};
	class FileOpenFailed : public MatrixException{
        public:
            const char* what() const noexcept override{
                return "LOAD: unable to open file.";
            };
    };
	class InvalidFile: public MatrixException{
		public:
			const char* what() const noexcept override{
				return "LOAD: invalid file format.";
			};
	};
	class BadAlloc: public MatrixException{
		public:
			const char* what() const noexcept override{
				return "Unable to allocate memory.";
			};
	};
	class InvalidAdd: public MatrixException{
		public:
			const char* what() const noexcept override{
				return "ADD: dimensions do not match.";
			};
	};
	class MulInvalidSize: public MatrixException{
		public:
			const char* what() const noexcept override{
				return "MUL: #arg1.columns != #arg2.rows.";
			};
	};
	class BadIndex: public MatrixException{
		public:
			const char* what() const noexcept override{
				return "ACCESS: bad index.";
			};
	};
}



class Matrix
{
public:
	Matrix(std:: ifstream& in);
	Matrix(const Matrix &);
	Matrix(int rows = 0, int cols = 0, int** array = nullptr);
	~Matrix();
	int get(int i, int j) const;

	Matrix &operator=(const Matrix &m);
	Matrix operator+(const Matrix &m) const;
	Matrix operator*(const Matrix &m) const;

	Matrix &operator+=(const Matrix &m);
	Matrix &operator*=(const Matrix &m);

	friend std:: ostream& operator<<(std:: ostream& os, Matrix& m);
private:
	int** make_array(int rows, int cols) const;
	bool check_index(int i, int j) const;
	void swap(Matrix &other);
	int rows_;
	int cols_;
	int **data_;
};

std:: ostream& operator<<(std:: ostream& os, Matrix& m);
