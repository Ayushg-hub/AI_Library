#include"Tensor.h"
#include<typeinfo>
#include<sstream>

namespace d_Tensor
{
	Tensor::Tensor()
	{

	}
	Tensor::Tensor(double* data, std::size_t size, std::vector<std::size_t> dimensions,bool copy)
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

		dim = dimensions;

		std::size_t total = 1;
		for (auto i : dimensions)
		{
			if (i > 0)
			{
				total *= i;
			}
		}

		auto it = std::find(dim.begin(), dim.end(), 0);

		if (it != dim.end())
		{
			dim.insert(it, size - total);
		}

		if (dim.size() == 1)
		{
			dim.push_back(1);
		}
	}

	Tensor::Tensor(std::size_t size, const std::vector<std::size_t> dimensions)
	{
		c_data = new double[size] ;

		//initializing explicitley
		for (std::size_t i = 0; i < size; i++)
		{
			c_data[i] = 0;
		}

		std::size_t total = 1;

		dim = dimensions;

		for (auto i : dimensions)
		{
			if (i > 0)
			{
				total *= i;
			}
		}

		auto it = std::find(dim.begin(), dim.end(), 0);

		if (it != dim.end())
		{
			dim.insert(it, size - total);
		}

		if (dim.size() == 1)
		{
			dim.push_back(1);
		}
	}
	Tensor::Tensor(const Tensor& T)
	{
		if (this == &T)
		{
			return;
		}
		
		std::size_t size = 1;
		
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

	Tensor& Tensor::operator=(const Tensor& other)
	{
		if (this == &other)
		{
			return *this;
		}

		if (!subtensor)
		{
			if (c_data != nullptr)
			{
				delete[] c_data;
			}

			if (other.c_data != nullptr)
			{
				c_data = new double[other.size()];
				memcpy(c_data, other.c_data, other.size() * sizeof(double));
			}

			dim = other.dim;
			subtensor = false;
		}
		else
		{
			if (dim != other.dim)
			{
				throw ("dimensions must be equal for subtensor assignment");
			}
			else
			{
				for (std::size_t i = 0; i < other.size(); i++)
				{
					c_data[i] = other.c_data[i];
				}
			}
		}
		
		return *this;
	}

	Tensor& Tensor::operator=(Tensor&& other)
	{
		if (!this->subtensor && !other.subtensor)
		{
			if (c_data != nullptr)
			{
				delete[] c_data;
			}

			c_data = other.c_data;
			dim = other.dim;
			subtensor = other.subtensor;

			other.c_data = nullptr;
			other.dim.clear();
			other.subtensor = false;
		}
		else if (!this->subtensor && other.subtensor)
		{
			if (c_data != nullptr)
			{
				delete[] c_data;
			}

			c_data = new double[other.size()];
			memcpy(c_data, other.c_data, other.size() * sizeof(double));
			dim = other.dim;

			//cleanup
			other.c_data = nullptr;
			other.dim.clear();
			other.subtensor = false;
		}
		else if (this->subtensor && !other.subtensor)
		{
			if (this->dim != other.dim)
			{
				throw ("cannot perform move assignment. subtensor has different dimensions");
			}

			memcpy(c_data, other.c_data, other.size() * sizeof(double));

			//cleanup
			delete[] other.c_data;
			other.c_data = nullptr;
			other.dim.clear();
			other.subtensor = false;
		}
		else//both are subtensors
		{
			if (this->dim != other.dim)
			{
				throw ("cannot perform move assignment. subtensor has different dimensions");
			}

			memcpy(c_data, other.c_data, other.size() * sizeof(double));
		}

		return *this;
	}

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
			std::size_t count = size();
			for (std::size_t i = 0; i < count; i++)
			{
				c_data[i] = val;
			}
		}
		return *this;
	}

	const Tensor* Tensor::checkCompatibility(const Tensor& A,const Tensor& B) const
	{
		//breadcasting only supported for upto 3d for now

		if (A.shape().size() > 3 || B.shape().size() > 3)
		{
			return nullptr;
		}

		bool compatible{ true };
		bool A_t{ true };
		bool B_t{ true };
		bool A_small{ A.shape().size() < B.shape().size() };
		bool equal{ A.shape().size() == B.shape().size() };

		if (equal)
		{
			for (int i = 0; i < B.shape().size(); i++)
			{
				if (B.shape()[i] % A.shape()[i] != 0)
				{
					A_t = false;
				}
				if (A.shape()[i] % B.shape()[i] != 0)
				{
					B_t = false;
				}
			}

			if (A_t)
			{
				return &A;
			}
			else if (B_t)
			{
				return &B;
			}
			else
			{
				return nullptr;
			}
		}
		else
		{
			for (int i = 0; i < std::min(B.shape().size(), A.shape().size()); i++)
			{
				if (A_small)
				{
					if (B.shape()[i] % A.shape()[i] != 0)
					{
						compatible = false;
						break;
					}
				}
				else
				{
					if (A.shape()[i] % B.shape()[i] != 0)
					{
						compatible = false;
						break;
					}
				}
			}

			if (compatible)
			{
				if (A_small)
				{
					return &A;
				}
				else
				{
					return &B;
				}
			}
			else
			{
				return nullptr;
			}
		}
	}

	//Tensor Tensor::broadcast(std::vector<unsigned int> _dst, Tensor& T)
	//{
	//	Tensor t(T);

	//	for (int i = 0; i < _dst.size(); i++)
	//	{
	//		if (t.dim[i] != _dst[i])
	//		{
	//			t.dim.erase(t.dim.begin());
	//		    
	//			Tensor temp;
	//		}
	//	}
	//	
	//}

	Tensor& Tensor::operator+=(const Tensor& other)
	{
		if (dim == other.dim && size() == other.size())
		{
			for (int i = 0; i < size(); i++)
			{
				c_data[i] += other.c_data[i];
			}

			return *this;
		}
		else
		{
			std::ostringstream ss;
			ss << "op(+=) : tensors need have the same dimensions \n"
				"Dimensions given : \n"
				"This : (";

			for (auto it : dim)
			{
				ss << it << " ";
			}

			ss << ")\n"
				"other : (";

			for (auto it : other.dim)
			{
				ss << it << " ";
			}
			ss << ")\n";

			throw (ss.str());

			//no broadcasting support for now;

			////check for broadcasting
			//const Tensor* t = checkCompatibility(other, *this);

			//if (t == nullptr)
			//{
			//	throw ("incompatible tensors (only upto 3 dimensions supported)");
			//}

			//std::vector<int> parts;
			//if (t == this)
			//{
			//	
			//}


		}
	}


	Tensor& Tensor::operator-=(const Tensor& other)
	{
		Tensor temp(size(), dim);

		if (dim == other.dim && size() == other.size())
		{
			for (int i = 0; i < size(); i++)
			{
				c_data[i] -= other.c_data[i];
			}

			return *this;
		}
		else
		{
			std::ostringstream ss;
			ss << "op(-=) : tensors need have the same dimensions \n"
				"Dimensions given : \n"
				"This : (";

			for (auto it : dim)
			{
				ss << it << " ";
			}

			ss << ")\n"
				"other : (";

			for (auto it : other.dim)
			{
				ss << it << " ";
			}
			ss << ")\n";

			throw (ss.str().c_str());

			//no broadcasting support for now;

		}
	}


	Tensor& Tensor::operator*=(const Tensor& other)
	{
		Tensor temp(size(), dim);

		if (dim == other.dim && size() == other.size())
		{
			for (int i = 0; i < size(); i++)
			{
				c_data[i] *= other.c_data[i];
			}

			return *this;
		}
		else
		{
			std::ostringstream ss;
			ss << "op(*=) : tensors need have the same dimensions \n"
				"Dimensions given : \n"
				"This : (";

			for (auto it : dim)
			{
				ss << it << " ";
			}

			ss << ")\n"
				"other : (";

			for (auto it : other.dim)
			{
				ss << it << " ";
			}
			ss << ")\n";

			throw (ss.str());

			//no broadcasting support for now;

		}
	}


	Tensor& Tensor::operator/=(const Tensor& other)
	{
		Tensor temp(size(), dim);

		if (dim == other.dim && size() == other.size())
		{
			for (int i = 0; i < size(); i++)
			{
				c_data[i] /= other.c_data[i];
			}

			return *this;
		}
		else
		{
			std::ostringstream ss;
			ss << "op(/=) : tensors need have the same dimensions \n"
				"Dimensions given : \n"
				"This : (";

			for (auto it : dim)
			{
				ss << it << " ";
			}

			ss << ")\n"
				"other : (";

			for (auto it : other.dim)
			{
				ss << it << " ";
			}
			ss << ")\n";

			throw (ss.str().c_str());

			//no broadcasting support for now;

		}
	}


	const Tensor Tensor::operator+(const Tensor& other) const
	{
		Tensor temp(size(), dim);

		if (dim == other.dim && size() == other.size())
		{
			for (int i = 0; i < size(); i++)
			{
				temp.c_data[i] = c_data[i] + other.c_data[i];
			}

			return temp;
		}
		else
		{
			std::ostringstream ss;
			ss << "op(+) : tensors need have the same dimensions \n"
				"Dimensions given : \n"
				"This : (";

			for (auto it : dim)
			{
				ss << it <<" ";
			}

			ss << ")\n"
				"other : (";

			for (auto it : other.dim)
			{
				ss << it << " ";
			}
			ss << ")\n";

			throw (ss.str().c_str());

			//no broadcasting support for now;

		}
	}


	const Tensor Tensor::operator-(const Tensor& other) const
	{
		Tensor temp(size(), dim);

		if (dim == other.dim && size() == other.size())
		{
			for (int i = 0; i < size(); i++)
			{
				temp.c_data[i] = c_data[i] - other.c_data[i];
			}

			return temp;
		}
		else
		{
			std::ostringstream ss;
			ss << "op(-) : tensors need have the same dimensions \n"
				"Dimensions given : \n"
				"This : (";

			for (auto it : dim)
			{
				ss << it << " ";
			}

			ss << ")\n"
				"other : (";

			for (auto it : other.dim)
			{
				ss << it << " ";
			}
			ss << ")\n";

			throw (ss.str().c_str());

			//no broadcasting support for now;

		}
	}


	const Tensor Tensor::operator*(const Tensor& other) const
	{
		Tensor temp(size(),dim);

		if (dim == other.dim && size() == other.size())
		{
			for (int i = 0; i < size(); i++)
			{
				temp.c_data[i] = c_data[i] * other.c_data[i];
			}

			return temp;
		}
		else
		{
			std::ostringstream ss;
			ss << "op(*) : tensors need have the same dimensions \n"
				"Dimensions given : \n"
				"This : (";

			for (auto it : dim)
			{
				ss << it << " ";
			}

			ss << ")\n"
				"other : (";

			for (auto it : other.dim)
			{
				ss << it << " ";
			}
			ss << ")\n";

			throw (ss.str());

			//no broadcasting support for now;

		}
	}


	const Tensor Tensor::operator/(const Tensor& other) const
	{
		Tensor temp(size(), dim);

		if (dim == other.dim && size() == other.size())
		{
			for (int i = 0; i < size(); i++)
			{
				temp.c_data[i] = c_data[i] / other.c_data[i];
			}

			return temp;
		}
		else
		{
			std::ostringstream ss;
			ss << "op(/) : tensors need have the same dimensions \n"
				"Dimensions given : \n"
				"This : (";

			for (auto it : dim)
			{
				ss << it << " ";
			}

			ss << ")\n"
				"other : (";

			for (auto it : other.dim)
			{
				ss << it << " ";
			}
			ss << ")\n";

			throw (ss.str());

			//no broadcasting support for now;

		}
	}


	bool Tensor::operator==(const Tensor& other) const
	{
		if (this == &other)
		{
			return true;
		}

		if (dim == other.dim)
		{
			if (c_data == other.c_data)
			{
				return true;
			}
			else if (c_data == nullptr || other.c_data == nullptr)
			{
				return false;
			}
			else
			{
				bool equal = true;
				for (std::size_t i = 0; i < size(); i++)
				{
					if (c_data[i] != other.c_data[i])
					{
						equal = false;
						break;
					}
				}
				return equal;
			}
		}
		else
		{
			return false;
		}
	}
	bool Tensor::operator!=(const Tensor& other) const
	{
		return !(*this==other);
	}

	Tensor Tensor::operator[](const std::size_t& i) const
	{
		std::size_t skip = 1;
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
			std::vector<std::size_t> d;
			it = dim.begin();
			it++;

			while (it != dim.end())
			{
				d.push_back((*it));
				it++;
			}

			Tensor t(&c_data[i * skip], skip, d,false);
			return t;
		}
	}

	double Tensor::operator[](const std::vector<std::size_t>& lst) const
	{
		if (lst.size() != dim.size())
		{
			throw ("use the correct number of axes. check the size of the list of indices passesed");
		}

		std::vector<std::size_t> lst2 = dim;
		std::size_t idx = 0;
		for (auto it1 : lst)
		{
			lst2.erase(lst2.begin());
			double s = 1;

			for (auto it2 : lst2)
			{
				s *= it2;
			}
			idx += it1 * s;
		}
		return c_data[idx];
	}

	void Tensor::print()
	{
		std::size_t count = size();

		if (count < 1)
		{
			std::cout << "EMPTY" << std::endl;
			return;
		}

		auto d = dim.begin();
		for (std::size_t i = 0; i < count; i++)
		{
			std::cout << c_data[i];
		}
	}

	Tensor Tensor::Slice(std::vector<std::size_t> start, std::vector<std::size_t> dim_size) const
	{
		//check for empty tensor
		if (c_data == nullptr)
		{
			throw ("nothing to slice from. Tensor not initialized with data");
		}

		//only support slicing for tensors upto 3 axis
		if (dim.size() > 3)
		{
			throw ("Tensor has > 3 axis. Currently slicing is supported for tensors upto 3 axis.");
		}

		//check for empty size
		if (dim_size.empty())
		{
			for (std::size_t i = 0; i < start.size(); i++)
			{
				dim_size.push_back(dim[i] - start[i]);
			}
		}

		//check for same number of axis 
		if (start.size() != dim_size.size() || start.size() != dim.size())
		{
			throw ("enter valid vectors");
		}

		//check if the dim_size vector contains a zero
		for (auto it : dim_size)
		{
			if (it == 0)
			{
				throw("dim_size cannot have a zero. lenth to slice along all dimensions should atleast be 1");
			}
		}

		//check out of bounds 
		for (std::size_t i = 0; i < start.size(); i++)
		{
			if (start[i] + dim_size[i] > dim[i])
			{
				throw("index out of bounds. enter slice dimensions that lie within the tensor");
			}
		}

		//only support slicing for tensors upto 3 axis
		std::size_t total_size{ 1 };
		for (auto it : dim_size)
		{
			total_size *= it;
		}
		Tensor S(total_size, dim_size);

		switch (dim.size())
		{
		case 1 :
		{
			for (std::size_t i = 0; i < dim_size[0]; i++)
			{
				S[i] = (*this)[start[0] + i];
			}
			break;
		}
		case 2:
		{
			for (std::size_t i = 0; i < dim_size[0]; i++)
			{
				for (std::size_t j = 0; j < dim_size[1]; j++)
				{
					S[i][j] = (*this)[start[0] + i][start[1] + j];
				}
			}
			std::cout<<std::endl;
			break;
		}
		case 3:
		{
			for (std::size_t i = 0; i < dim_size[0]; i++)
			{
				for (std::size_t j = 0; j < dim_size[1]; j++)
				{
					for (std::size_t k = 0; k < dim_size[2]; k++)
					{
						S[i][j][k] = (*this)[start[0] + i][start[1] + j][start[2] + k];
					}
				}
			}
			break;
		}
		}

		return S;
	}

	void Tensor::reshape(std::vector<std::size_t> dimensions)
	{
		dim = dimensions;
	}

	std::size_t Tensor::size() const
	{
		if (dim.empty())
		{
			return 0;
		}

		std::size_t size = 1;
		for (auto it = dim.begin(); it != dim.end(); it++)
		{
			size *= (*it);
		}

		return size;
	}

	std::vector<std::size_t> Tensor::shape() const
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
