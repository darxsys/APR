/**
    APR
    f3.hpp
    Purpose: Declaration of a class that implements the following function:
    f(x1,x2,x3,x4,x5) = (x1-p1)^2 + (x2-p2)^2 + ... + (x5-p5)^2.

    @author Dario Pavlovic
    @version 1.0 12/01/2013
*/

#ifndef F3_HPP
#define F3_HPP

#include <vector>
#include <string>
using namespace std;

#include "function.hpp"

class F3 : public Function {
public:
	F3(string filename);
	~F3(); 
	virtual double evaluate(vector<double> &point);
private:
	vector<double> params;
};

#endif