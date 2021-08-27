#include"Tensor.h"

namespace linalg
{
	namespace Matrix
	{
		bool isMatrix(const d_Tensor::Tensor& A);

		d_Tensor::Tensor identity(std::size_t i);
	
		d_Tensor::Tensor matmul(const d_Tensor::Tensor& A, const d_Tensor::Tensor& B);

		d_Tensor::Tensor inverse(const d_Tensor::Tensor& A);

		d_Tensor::Tensor Transpose(d_Tensor::Tensor& A, bool inplace = false);

		d_Tensor::Tensor det(const d_Tensor::Tensor& A);
	}
	namespace Vector
	{
		bool isVector(const d_Tensor::Tensor& A);

		double dot(const d_Tensor::Tensor& A, const d_Tensor::Tensor& B);

		double l2norm(const d_Tensor::Tensor& A);

		double l2normSq(const d_Tensor::Tensor& A);


	}
}