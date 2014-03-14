/**
    APR
    f4.hpp
    Purpose: Declaration of a class that implements the following function:
    f(x,y) = |(x-y)*(x+y)| + (x2+y2)^0.5.

    @author Dario Pavlovic
    @version 1.0 12/01/2013
*/

#ifndef F4_HPP
#define F4_HPP

#include <vector>
using namespace std;

#include "function.hpp"

class F4 : public Function {
public:
	F4();
	~F4();
	virtual double evaluate(vector<double> &point);
};

#endif