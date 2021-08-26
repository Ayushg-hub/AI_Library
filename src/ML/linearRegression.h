#include"../common/Tensor.h"

//a class for performing linear regression

class linearRegression
{
    d_Tensor::Tensor weights;

public:
    linearRegression();
    void compile();

    /* input : rows = each data point
               column = each feature
               tensor of dim {rows,columns}
    */
    void fit(d_Tensor::Tensor inputs, d_Tensor::Tensor outputs, unsigned int epochs);

    void eval(d_Tensor::Tensor inputs, d_Tensor::Tensor outputs);
    d_Tensor::Tensor predict(d_Tensor::Tensor inputs);

private:

};

