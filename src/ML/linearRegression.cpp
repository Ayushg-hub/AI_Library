#include"linearRegression.h"

linearRegression::linearRegression();
void linearRegression::compile();
void linearRegression::fit(double* inputs, double* outputs, unsigned int epochs);
void linearRegression::eval(double* inputs, double* outputs);
void linearRegression::predict(double* inputs);