#include"../common/Tensor.h"

//a class for performing linear regression

class linearRegression
{
    Tensor weights;

public:
    linearRegression();
    void compile();
    void fit(double* inputs, double* outputs, unsigned int epochs);
    void eval(double* inputs, double* outputs);
    void predict(double* inputs);

private:

};

