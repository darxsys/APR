/**
    APR
    f1.hpp
    Purpose: Declaration of a class that implements the following function:
    f(x,y) = 10*(x2-y)^2+(1-x)^2.

    @author Dario Pavlovic
    @version 1.0 12/01/2013
*/

#ifndef F1_HPP
#define F1_HPP

#include <vector>
using namespace std;

#include "function.hpp"

class F1 : public Function {
public:
	F1();
	~F1();
	virtual double evaluate(vector<double> &point);

};

#endif