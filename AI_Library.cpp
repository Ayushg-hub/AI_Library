// AI_Library.cpp : Defines the entry point for the application.
//
#pragma once
#include<iostream>
#include<typeinfo>
#include"linalg.h"
//#include<Tensor.h>
#include<sstream>


std::string str(std::vector<std::size_t> v)
{
	std::ostringstream s;
	s << "(";
	for (auto it : v)
	{
		s << it << ", ";
	}
	s << "\b\b)";
	return s.str();
}

int& add(int a)
{
	int u;
	u = a + 1;
	return u;
}



int main()
{
	//int A[10][10];
	//char B;
	//float C;
	//double D;
	//unsigned int E;
	//std::string F;
	//int* G;
	//char* H;
	//float* I;
	//double* J;

	d_Tensor::Tensor t1 = { {2,0,0},
							{0,4,0},
							{0,0,8 } };

	d_Tensor::Tensor t2 = { 1,2,3 };

	//d_Tensor::Tensor t1(data, 9, { 3,3 });
	//d_Tensor::Tensor t2(data2, 3, { 3 });
	std::cout << "t1 shape :" << str(t1.shape()) << std::endl;
	
	std::cout << "t2 shape :" << str(t2.shape()) << std::endl;

	try
	{
		std::cout << "t1 det : " << linalg::Matrix::det(t1) << std::endl;
		std::cout << "t1 inv : " << linalg::Matrix::inverse(t1) << std::endl;
		std::cout << "inv check : t*t_inv = " << linalg::Matrix::matmul(t1, linalg::Matrix::inverse(t1)) << std::endl;
	}
	catch (const char* e)
	{
		std::cout << e << std::endl;
	}
	catch (std::string e)
	{
		std::cout << e << std::endl;
	}

	try
	{
		std::cout <<linalg::Matrix::matmul(t1, t2) << std::endl;
	}
	catch (const char* e)
	{
		std::cout << e << std::endl;
	}
	catch (std::string e)
	{
		std::cout << e << std::endl;
	}

	int a = 1;
	int x = add(a);
	std::cout <<"x : "<< x << std::endl;


	std::cout << t1[{1, 2, 3}] << std::endl;
	std::cout << t1[1] << std::endl;
	t1[1][1] = 1000;
	std::cout << t1[1][1] << std::endl;
	t1[1][1] = t2[1];
	std::cout << t1[1][1] << std::endl;
	std::cout << t1 << std::endl;
	std::cout << t1.Slice({ 0,0 }, {2,2}) << std::endl;
	

	

	//std::cout << "int : " << typeid(A[1]).name() << std::endl;
	//std::cout << "char : " << typeid(B).name() << std::endl;
	//std::cout << "float : " << typeid(C).name() << std::endl;
	//std::cout << "double : " << typeid(D).name() << std::endl;
	//std::cout << "unsigned int : " << typeid(E).name() << std::endl;
	//std::cout << "string : " << typeid(F).name() << std::endl;
	//std::cout << "int* : " << typeid(G).name() << std::endl;
	//std::cout << "char* : " << typeid(H).name() << std::endl;
	//std::cout << "float* : " << typeid(I).name() << std::endl;
	//std::cout << "double* : " << typeid(J).name() <<" "<<(typeid(float*) == typeid(float*)) <<std::endl;
	//std::cout << "double* : " << typeid(t1).name() << std::endl;

}
