#include<list>
#include<initializer_list>
#include<iostream>


namespace d_Tensor
{
	class Tensor
	{
		double* c_data{ nullptr };
		std::list<unsigned int> dim;
		bool subtensor{ false };//is it a part of another tensor

		//might add gpu data

	public:
		Tensor();
		Tensor(double* data, unsigned int size ,std::list<int> dimensions = { -1 },bool copy = true);
		Tensor(unsigned int size, const std::list<int> dimensions = {-1});
		Tensor(const Tensor& T);
		Tensor(Tensor&& T);

		template <typename T>
		Tensor(std::initializer_list<T> lst);

		Tensor& operator=(const Tensor& other);
		Tensor& operator=(const Tensor&& other);
		Tensor& operator=(const double& val);

		Tensor& operator+=(const Tensor& other);
		Tensor& operator-=(const Tensor& other);
		Tensor& operator*=(const Tensor& other);
		Tensor& operator/=(const Tensor& other);

		const Tensor operator+(const Tensor& other);
		const Tensor operator-(const Tensor& other);
		const Tensor operator*(const Tensor& other);
		const Tensor operator/(const Tensor& other);

		bool operator==(const Tensor& other) const;
		bool operator!=(const Tensor& other) const;

		Tensor operator[](const std::size_t& i);
		double operator[](const std::list<std::size_t>& lst);

		friend std::ostream& operator<<(std::ostream& out, const Tensor& T);

		void print();

		Tensor matmul(const Tensor& A, const Tensor& B);
		Tensor dot(const Tensor& A, const Tensor& B);
		void reshape(std::list<int> dimensions);
		unsigned int size() const; 
		std::list<unsigned int> shape();

		~Tensor();

	};

	template<typename T>
	inline Tensor::Tensor(std::initializer_list<T> lst)
	{
		dim.push_back(lst.size());

		T* temp = new T[lst.size()];
		unsigned int i = 0;
		for (const auto& it : lst)
		{
			temp[i] = it;
			i++;
		}
		c_data = (void*)temp;
	}

	inline std::ostream& operator<<(std::ostream& out, const Tensor& T)
	{
		unsigned int size = T.size();
		out << "[ ";
		for (std::size_t i = 0; i < size; i++)
		{
			out << T.c_data[i] << " , ";
		}
		out << "\b\b]";
		return out;
	}
}


