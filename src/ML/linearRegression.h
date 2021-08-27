#include"../common/linalg.h"

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
    d_Tensor::Tensor fit(d_Tensor::Tensor inputs, d_Tensor::Tensor outputs, unsigned int epochs = 5);

    void eval(d_Tensor::Tensor inputs, d_Tensor::Tensor outputs);
    d_Tensor::Tensor predict(d_Tensor::Tensor inputs);

private:

};

