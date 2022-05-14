#include "matrix.h"

Matrix:: Matrix(std:: ifstream& in){
    if (!in.is_open())
        throw  MatrixException::FileOpenFailed();
    in >> rows_ >> cols_;
    if(in.fail()){
        throw MatrixException:: InvalidFile();
    };
    data_ = make_array(rows_, cols_);
    for (int i = 0 ; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j){
            in >> data_[i][j];
            if (in.fail()){
                delete[] data_[0];
                delete[] data_;
                throw MatrixException:: InvalidFile();
            };
        }
}

bool Matrix:: check_index(int row, int col)const{
    return (row < rows_ and col <  cols_ and row >= 0 and col >= 0)? true: false; 
}

int Matrix::get(int row, int col)const{    
    if (not check_index(row, col)){
        throw MatrixException::BadIndex();
    };
    return data_[row][col];
}

Matrix:: ~Matrix(){
    if (data_){
        delete[] data_[0];
        delete[] data_;
    }
}


std:: ostream& operator<<(std:: ostream& os, Matrix& m){
    for (int i = 0; i < m.rows_; ++i){
        os << m.data_[i][0];
        for (int j = 1; j < m.cols_; ++j)
            os << " " <<  m.data_[i][j]; 
        os << std:: endl;
    }
    return os;
}

int** Matrix:: make_array(int rows, int cols) const{
    int** data;
    if (cols * rows == 0)
        return nullptr;
    int* array;
    try{    
        array = new int [cols * rows];
    }catch(...){
        throw MatrixException:: BadAlloc();   
    }
    try{
        data = new int*[rows];
    }catch(...){
        delete[] array;
        throw MatrixException:: BadAlloc();
    }
    for (int i = 0; i < rows; ++i)
        data[i] = array + i * cols;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            data[i][j] = 0;
    return data;
}

Matrix Matrix:: operator+(const Matrix& m)const{
    if (rows_ != m.rows_ or cols_ != m.cols_)
        throw MatrixException::InvalidAdd();
    Matrix sum(rows_, cols_, make_array(rows_, cols_));
    for (int i  = 0 ; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j)
            sum.data_[i][j] = data_[i][j] + m.data_[i][j];
    return sum;
}

Matrix& Matrix:: operator+=(const Matrix& m){
    Matrix tmp = *this + m;
    swap(tmp);
    return *this;
}

void Matrix::swap(Matrix& other){
    std:: swap(cols_, other.cols_);
    std:: swap(rows_, other.rows_);
    std:: swap(data_, other.data_);
}

Matrix:: Matrix(int rows, int cols, int** arr):rows_(rows), cols_(cols), data_(arr){}

Matrix Matrix:: operator*(const Matrix& m) const{
    if (cols_ != m.rows_)
        throw MatrixException::MulInvalidSize();
    Matrix mul_matrix(rows_, m.cols_, make_array(rows_, m.cols_));
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < m.cols_; ++j){
            int sum = 0;
            for (int k = 0; k < cols_; ++k)
                sum += data_[i][k]* m.data_[k][j];
            mul_matrix.data_[i][j] = sum;
        }
    return mul_matrix;
}

Matrix& Matrix:: operator*=(const Matrix& m){
    Matrix tmp_matrix = *this * m;
    swap(tmp_matrix);
    return *this;
}


Matrix:: Matrix(const Matrix& other): Matrix(other.rows_, other.cols_, make_array(other.rows_, other.cols_)){
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j< cols_; ++j)
            data_[i][j] = other.data_[i][j];
}


Matrix& Matrix:: operator=(const Matrix& m){
    Matrix copy(m);
    swap(copy);
    return *this;
}
