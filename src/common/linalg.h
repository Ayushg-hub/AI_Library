#include"Tensor.h"

namespace linalg
{
	namespace Matrix
	{
		d_Tensor::Tensor identity(std::size_t i)
		{
			d_Tensor::Tensor I(i * i, { i,i });
			for (std::size_t j = 0; j < i; j++)
			{
				I[j][j] = 1;
			}
			return I;
		}

		d_Tensor::Tensor matmul(const d_Tensor::Tensor& A, const d_Tensor::Tensor& B)
		{
			if (A.shape().empty() || A.shape().empty())
			{
				throw "un initialized matrices";
			}
			if (A.shape().size() > 2 || B.shape().size() > 2)
			{
				throw "matrices of incorrect shape";
			}
			if (A.shape()[1] != B.shape()[0])
			{
				throw "incompatible matrices \n";
			}

			std::size_t rows{ A.shape()[0] };
			std::size_t columns;
			if (B.shape().size() == 1)
			{
				columns = 1;
			}
			else
			{
				columns = B.shape()[1];
			}

			d_Tensor::Tensor C = d_Tensor::Tensor(rows * columns, { rows,columns });

			for (std::size_t i = 0; i < rows; i++)
			{
				for (std::size_t j = 0; j < columns; j++)
				{
					for (std::size_t k = 0; k < A.shape()[1]; k++)
					{
						C[i][j] += A[i][k] * B[k][j];
					}
				}
			}



			return C;

		}

		d_Tensor::Tensor inverse(const d_Tensor::Tensor& A)
		{
			//finding inverse using gaussian elimination

			if (A.shape().size() != 2)
			{
				throw "matrices of incorrect shape";
			}

			if (A.shape()[0] != A.shape()[1])
			{
				throw ("not a square matrix");
			}

			std::size_t n = A.shape()[0];
			d_Tensor::Tensor t = A;
			d_Tensor::Tensor I = identity(n);



			bool inv_exists{ true };
			for (std::size_t i = 0; i < A.shape()[0]; i++)
			{

				d_Tensor::Tensor temp;
				temp = t[i][i];
				for (std::size_t j = 0; j < A.shape()[1]; j++)
				{
					I[i][j] = I[i][j] / temp;
					t[i][j] = t[i][j] / temp;	
				}

				 
				for (std::size_t j = i + 1; j < A.shape()[0]; j++)
				{
					temp = t[j][i];
					for (std::size_t k = 0; k < A.shape()[1]; k++)
					{
						I[j][k] -= I[i][k] * temp;
						t[j][k] -= t[i][k] * temp;	
					}
				}
				for (int j = i - 1; j >= 0; j--)
				{
					temp = t[j][i];
					for (std::size_t k = 0; k < A.shape()[1]; k++)
					{
						I[j][k] -= I[i][k] * temp;
						t[j][k] -= t[i][k] * temp;
					}
				}

				if (i + 1 != A.shape()[0])
				{
					if (t[i + 1] == d_Tensor::Tensor(t[i + 1].size(), t[i + 1].shape()))
					{
						inv_exists = false;
						break;
					}
				}
			}

			if (!inv_exists)
			{
				return d_Tensor::Tensor();
			}
			else
			{
				return I;
			}
		}

		d_Tensor::Tensor Transpose(d_Tensor::Tensor& A, bool inplace = false)
		{
			std::vector<std::size_t> T_shape = { A.shape()[1],A.shape()[0] };
			d_Tensor::Tensor t(A.size(), T_shape);
			for (std::size_t i = 0; i < A.shape()[0]; i++)
			{
				for (std::size_t j = 0; j < A.shape()[1]; j++)
				{
					t[j][i] = A[i][j];
				}
			}

			if (inplace)
			{
				A.reshape(T_shape);
				for (std::size_t i = 0; i < A.shape()[0]; i++)
				{
					for (std::size_t j = 0; j < A.shape()[1]; j++)
					{
						A[i][j] = t[i][j];
					}
				}
			}

			return t;
		}

		d_Tensor::Tensor det(const d_Tensor::Tensor& A)
		{
			
			if (A.shape().size() != 2 )
			{
				throw "matrices of incorrect shape";
			}

			if (A.shape()[0] != A.shape()[1])
			{
				throw ("not a square matrix");
			}

			d_Tensor::Tensor t = A;
			if (A.shape()[0] == 2)
			{
				return (A[0][0] * A[1][1]) - (A[0][1] * A[1][0]);
			}
			else
			{ 
				//creating zeroes on al positions except (0,0)
				for (std::size_t i = 0; i < A.shape()[0]; i++)
				{
					for (std::size_t j = 1; j < A.shape()[1]; j++)
					{
						t[i][j] -= ((A[i][0] / A[0][0]) * A[0][j]);
					}
				}

				d_Tensor::Tensor A11 = t.Slice({ 1,1 });
				return t[0][0] * det(A11);
			}
		}

		//Tensor Tensor::dot(const Tensor& A, const Tensor& B)
		//{

		//}
	}
}