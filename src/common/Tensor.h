#include<vector>
#include<initializer_list>
#include<iostream>


namespace d_Tensor
{
	class Tensor
	{
		double* c_data{ nullptr };
		std::vector<std::size_t> dim;
		bool subtensor{ false };//is it a part of another tensor

		//might add gpu data

	public:
		Tensor();
		Tensor(double* data, std::size_t size ,std::vector<std::size_t> dimensions = { 0 },bool copy = true);
		Tensor(std::size_t size, const std::vector<std::size_t> dimensions = {0});
		Tensor(const Tensor& T);
		Tensor(Tensor&& T);

		template <typename T>
		Tensor(std::initializer_list<T> lst);

		template <typename T>
		Tensor(std::initializer_list<std::initializer_list<T>> lst);

		Tensor& operator=(const Tensor& other);
		Tensor& operator=(Tensor&& other);
		Tensor& operator=(const double& val);

		
		Tensor& operator+=(const Tensor& other);
		Tensor& operator-=(const Tensor& other);
		Tensor& operator*=(const Tensor& other);
		Tensor& operator/=(const Tensor& other);

		const Tensor operator+(const Tensor& other) const;
		const Tensor operator-(const Tensor& other) const;
		const Tensor operator*(const Tensor& other) const;
		const Tensor operator/(const Tensor& other) const;

		bool operator==(const Tensor& other) const;
		bool operator!=(const Tensor& other) const;

		Tensor operator[](const std::size_t& i) const;
		double operator[](const std::vector<std::size_t>& lst);

		friend std::ostream& operator<<(std::ostream& out, const Tensor& T);

		void print();

		/*
		  --inputs : 
		         start : starting indices of a tensor you want to slice from
				 dim_size: lenth you want to slice in each axis.
				           Pass an empty vector {} to slice till end from 'start'

		  --return :
		         returns the sliced tensor
		*/
		Tensor Slice(std::vector<std::size_t> start, std::vector<std::size_t> dim_size = {}) const;

		const Tensor* checkCompatibility(const Tensor& A, const Tensor& B) const;
		Tensor broadcast(std::vector<unsigned int> _dst, Tensor& T);
		
		void reshape(std::vector<std::size_t> dimensions);
		std::size_t size() const;
		std::vector<std::size_t> shape() const;

		~Tensor();

	};

	template<typename T>
	inline Tensor::Tensor(std::initializer_list<T> lst)
	{

		dim.push_back(lst.size());
		dim.push_back(1);

		c_data = new double[lst.size()];
		unsigned int i = 0;
		for (const auto& it : lst)
		{
			c_data[i] = it;
			i++;
		}

	}

	template<typename T>
	inline Tensor::Tensor(std::initializer_list<std::initializer_list<T>> lst)
	{

		dim.push_back(lst.size());
		dim.push_back((*(lst.begin())).size());

		c_data = new double[size()];

		unsigned int i = 0;
		for (const auto& it1 : lst)
		{
			for (const auto& it2 : it1)
			{
				c_data[i] = it2;
				i++;
			}
		}
	}

	inline std::ostream& operator<<(std::ostream& out, const Tensor& T)
	{
		std::size_t size = T.size();
		out << "[ ";
		for (std::size_t i = 0; i < size; i++)
		{
			out << T.c_data[i] << " , ";
		}
		out << "\b\b]";
		return out;
	}
}


