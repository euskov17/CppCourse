#include <string>
#include <map>
#include <iostream>
#include <cstdio>
#include <cassert>

#include "matrix.h"

class Interpretator{
public:
	Interpretator(){
		array_ = new Matrix[10];
	};
	void load(int reg, std:: string fname){
		std:: ifstream in(fname);
		array_[reg] = Matrix(in);
	};
	void print(int reg){
		std:: cout << array_[reg];
	};
	void add(int reg1, int reg2){
		array_[reg1] += array_[reg2];
	};
	void mul(int reg1, int reg2){
		array_[reg1] *= array_[reg2];
	};
	int elem(int reg, int row, int col){
		return array_[reg].get(row, col);
	};
	~Interpretator(){
		delete[] array_;
	};
private:
	Matrix* array_;
};


int main(){
	std:: string str, fname;
	int reg, row, col, reg1, reg2;
	char dollar;
	Interpretator inter;
	while (true){
		std::cin >> str;
		if (str == "exit")
			break;
		if (str == "load"){
			std:: cin >> dollar >> reg >> fname;
			try{
				inter.load(reg, fname);
			}catch(MatrixException:: MatrixException& e){
				std:: cout << e.what() << std:: endl;
			}
		}else if (str == "add"){
			std:: cin >> dollar >> reg1>> dollar >> reg2;
			try{
				inter.add(reg1, reg2);
			}catch(MatrixException:: MatrixException& e){
				std:: cout << e.what() << std:: endl;
			};
		}else if (str == "mul"){
			std:: cin >>dollar>> reg1>> dollar >> reg2;
			try{
				inter.mul(reg1, reg2);
			}catch(MatrixException :: MatrixException& e){
				std:: cout << e.what() << std:: endl;
			}
		}else if (str == "print"){
			std:: cin  >> dollar>> reg;
			try{
				inter.print(reg);
			}catch(MatrixException:: MatrixException& e){
				std:: cout << e.what() << std:: endl;
			}
		}else if (str == "elem"){
			std:: cin >>dollar >> reg >> row>> col;
			try{
				std:: cout << inter.elem(reg, row, col)<< std:: endl;
			}catch(MatrixException:: MatrixException& e){
				std:: cout << e.what() << std:: endl;
			}
		}
	}
	return 0;
}