// AI_Library.cpp : Defines the entry point for the application.
//
#include<iostream>
#include<typeinfo>
#include<Tensor.h>

using namespace std;

int main()
{
	int A[10][10];
	char B;
	float C;
	double D;
	unsigned int E;
	std::string F;
	int* G;
	char* H;
	float* I;
	double* J;

	double data[] = { 1,2,3,
					  4,5,6,
					  7,8,9,
					  1,2,3,
					  4,5,6,
					  7,8,9, 
					  1,2,3,
					  4,5,6,
					  7,8,9, };

	d_Tensor::Tensor t1(data, 27, { 3,3,3 });
	std::cout << t1[{1, 2, 3}] << std::endl;
	std::cout << t1[1][1] << std::endl;
	t1[1][1] = 1000;
	std::cout << t1[1][1] << std::endl;
	std::cout << t1 << std::endl;

	

	std::cout << "int : " << typeid(A[1]).name() << std::endl;
	std::cout << "char : " << typeid(B).name() << std::endl;
	std::cout << "float : " << typeid(C).name() << std::endl;
	std::cout << "double : " << typeid(D).name() << std::endl;
	std::cout << "unsigned int : " << typeid(E).name() << std::endl;
	std::cout << "string : " << typeid(F).name() << std::endl;
	std::cout << "int* : " << typeid(G).name() << std::endl;
	std::cout << "char* : " << typeid(H).name() << std::endl;
	std::cout << "float* : " << typeid(I).name() << std::endl;
	std::cout << "double* : " << typeid(J).name() <<" "<<(typeid(float*) == typeid(float*)) <<std::endl;
	std::cout << "double* : " << typeid(t1).name() << std::endl;

}
