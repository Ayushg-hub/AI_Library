#include"linearRegression.h"

linearRegression::linearRegression()
{

}
void linearRegression::compile()
{

}
d_Tensor::Tensor linearRegression::fit(d_Tensor::Tensor inputs, d_Tensor::Tensor outputs, unsigned int epochs)
{
	if (inputs.shape().size() != 2)
	{
		throw "A 2 dimensional tensor expected";
	}

	weights = d_Tensor::Tensor::Tensor(inputs.shape()[1], { inputs.shape()[1] });

	d_Tensor::Tensor T_inputs = linalg::Matrix::Transpose(inputs);
	d_Tensor::Tensor inv = linalg::Matrix::inverse(linalg::Matrix::matmul(T_inputs, inputs));
	weights = linalg::Matrix::matmul(linalg::Matrix::matmul(inv, T_inputs), outputs);

	return weights;

}
void linearRegression::eval(d_Tensor::Tensor inputs, d_Tensor::Tensor outputs)
{

}
d_Tensor::Tensor linearRegression::predict(d_Tensor::Tensor inputs)
{
	return linalg::Matrix::matmul(inputs, weights);
}