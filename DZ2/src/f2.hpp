/**
    APR
    f2.hpp
    Purpose: Declaration of a class that implements the following function:
    f(x,y) = 10*(x2-y)2+(1-x)^2.

    @author Dario Pavlovic
    @version 1.0 12/01/2013
*/

#ifndef F2_HPP
#define F2_HPP

#include <vector>
using namespace std;

#include "function.hpp"

class F2 : public Function {
public:
	F2();
	~F2(); 
	virtual double evaluate(vector<double> &point);
};

#endif