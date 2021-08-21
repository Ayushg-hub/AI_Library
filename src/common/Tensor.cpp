#include"Tensor.h"
#include<typeinfo>

namespace d_Tensor
{
	Tensor::Tensor()
	{

	}
	Tensor::Tensor(double* data, unsigned int size, std::list<int> dimensions,bool copy)
	{

		if (copy)
		{
			c_data = new double[size];
			memcpy(c_data, data, size * sizeof(double));
		}
		else
		{
			subtensor = true;
			c_data = data;
		}

		unsigned int total = 1;
		for (auto i : dimensions)
		{
			if (i > 0)
			{
				total *= i;
				dim.push_back(i);
			}
			else
			{
				dim.push_back(0);
			}
		}
		auto it = std::find(dim.begin(), dim.end(), 0);
		if (it != dim.end())
		{
			dim.insert(it, size - total);
		}
	}

	Tensor::Tensor(unsigned int size, const std::list<int> dimensions)
	{
		unsigned int total = 1;
		for (auto i : dimensions)
		{
			if (i > 0)
			{
				total *= i;
				dim.push_back(i);
			}
			else
			{
				dim.push_back(0);
			}
		}
		auto it = std::find(dim.begin(), dim.end(), 0);
		dim.insert(it, size - total);
	}
	Tensor::Tensor(const Tensor& T)
	{
		if (this == &T)
		{
			return;
		}
		
		unsigned int size = 1;
		
		for (auto i : T.dim)
		{
			size *= i;
		}

		if (size < 1)
		{
			return;
		}

		c_data = new double[size];
		memcpy(c_data, T.c_data, size*sizeof(double));
		dim = T.dim;
	}

	Tensor::Tensor(Tensor&& T)
	{
		c_data = T.c_data;
		dim = T.dim;
		subtensor = T.subtensor;

		T.c_data = nullptr;
		T.dim.clear();
		T.subtensor = false;
	}

	//Tensor& Tensor::operator=(const Tensor& other)
	//{
	//	// TODO: insert return statement here
	//}
	//Tensor& Tensor::operator=(const Tensor&& other)
	//{
	//	// TODO: insert return statement here
	//}

	Tensor& Tensor::operator=(const double& val)
	{
		if (c_data == nullptr)
		{
			c_data = new double[1];
			c_data[0] = val;
			dim = { 1 };
		}
		else
		{
			unsigned int count = size();
			for (unsigned int i = 0; i < count; i++)
			{
				c_data[i] = val;
			}
		}
		return *this;
	}

	//Tensor& Tensor::operator+=(const Tensor& other)
	//{
	//	// TODO: insert return statement here
	//}
	//Tensor& Tensor::operator-=(const Tensor& other)
	//{
	//	// TODO: insert return statement here
	//}
	//Tensor& Tensor::operator*=(const Tensor& other)
	//{
	//	// TODO: insert return statement here
	//}
	//Tensor& Tensor::operator/=(const Tensor& other)
	//{
	//	// TODO: insert return statement here
	//}
	//const Tensor Tensor::operator+(const Tensor& other)
	//{
	//	return Tensor();
	//}
	//const Tensor Tensor::operator-(const Tensor& other)
	//{
	//	return Tensor();
	//}
	//const Tensor Tensor::operator*(const Tensor& other)
	//{
	//	return Tensor();
	//}
	//const Tensor Tensor::operator/(const Tensor& other)
	//{
	//	return Tensor();
	//}
	//bool Tensor::operator==(const Tensor& other) const
	//{
	//	return false;
	//}
	//bool Tensor::operator!=(const Tensor& other) const
	//{
	//	return false;
	//}

	Tensor Tensor::operator[](const std::size_t& i)
	{
		unsigned int skip = 1;
		if (dim.empty())
		{
			throw std::runtime_error("tensor is of lesser dimensions");
		}
		auto it = dim.begin();
		it++;

		while(it!=dim.end())
		{
			skip *= *it;
			it++;
		}


		if(skip == 1)
		{
			Tensor t(&c_data[i * skip], 1, {1},false);
			return t;
		}
		else
		{
			std::list<int> d;
			it = dim.begin();
			it++;

			while (it != dim.end())
			{
				d.push_back((int)*it);
				it++;
			}

			Tensor t(&c_data[i * skip], skip, d,false);
			return t;
		}
	}

	double Tensor::operator[](const std::list<std::size_t>& lst)
	{
		return c_data[0];
	}

	void Tensor::print()
	{
		unsigned int count = size();

		if (count < 1)
		{
			std::cout << "EMPTY" << std::endl;
			return;
		}

		auto d = dim.begin();
		for (unsigned int i = 0; i < count; i++)
		{
			std::cout << c_data[i];
		}
	}

	/*Tensor Tensor::matmul(const Tensor& A, const Tensor& B)
	{

	}
	Tensor Tensor::dot(const Tensor& A, const Tensor& B)
	{

	}
	void Tensor::reshape(std::list<int> dimensions)
	{

	}*/

	unsigned int Tensor::size() const
	{
		if (dim.empty())
		{
			return 0;
		}

		unsigned int size = 1;
		for (auto it = dim.begin(); it != dim.end(); it++)
		{
			size *= (*it);
		}

		return size;
	}

	std::list<unsigned int> Tensor::shape()
	{
		return dim;
	}

	Tensor::~Tensor()
	{
		if (c_data != nullptr && !subtensor)
		{
			delete[] c_data;
		}
	}
}
