/**
    APR
    function.hpp
    Purpose: Definition of abstract function interface.

    @author Dario Pavlovic
    @version 1.0 12/01/2013
*/

#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <stdio.h>
#include <vector>
using namespace std;

class Function {
public:
	virtual double evaluate(vector<double> &point)=0;
	int get_dimensions() { return dimensions; }
	
protected:
	int check_dimensions(vector<double> &point) {
		if (point.size() != this->dimensions) {
			printf("Point does not have the proper dimensionality.\n");
			return 0;
		}; 
		return 1;
	}
	int dimensions;
};

#endif