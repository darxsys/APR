/**
    APR
    hj.hpp
    Purpose: Defines a class for implementing Hooke and Jeeves alogrithm 
    for function optimization.

    @author Dario Pavlovic
    @version 0.8 12/01/2013
*/

#ifndef HJ_HPP
#define HJ_HPP

#include <string>
#include <vector>
using namespace std;

#include "function.hpp"

#define DX 0.5
#define E 1e-9

class HookeJeeves {
public:
	HookeJeeves(Function* func, string starting_point, string error_file="", string dx_file="");
	~HookeJeeves(); 
	vector<double> optimize();
private:
	vector<double> search(vector<double> &point);
	bool stop();
	int n;
	vector<double> x0;
	vector<double> xb;
	vector<double> xp;
	vector<double> xn;
	vector<double> dx;
	vector<double> e;
	Function* f;
};

#endif