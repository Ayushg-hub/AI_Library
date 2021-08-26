#include"linearRegression.h"

linearRegression::linearRegression()
{

}
void linearRegression::compile()
{

}
void linearRegression::fit(d_Tensor::Tensor inputs, d_Tensor::Tensor outputs, unsigned int epochs)
{
	if (inputs.shape().size() != 2)
	{
		throw "A 2 dimensional tensor expected";
	}

	weights = d_Tensor::Tensor::Tensor(inputs.shape()[1], { inputs.shape()[1] });

	
}
void linearRegression::eval(d_Tensor::Tensor inputs, d_Tensor::Tensor outputs)
{

}
d_Tensor::Tensor linearRegression::predict(d_Tensor::Tensor inputs)
{

}