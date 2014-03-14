/**
    APR
    f.cpp
    Purpose: Implementation of various functions declared in their respective headers.

    @author Dario Pavlovic
    @version 1.0 12/01/2013
*/

#include <stdlib.h>
#include <math.h>
#include <vector>
#include <fstream>
using namespace std;

#include "f1.hpp"
#include "f2.hpp"
#include "f3.hpp"
#include "f4.hpp"

// f1(x,y) = 10*(x2-y)^2+(1-x)^2
F1::F1() {
	dimensions = 2;
}

double F1::evaluate(vector<double> &point) {
	if (Function::check_dimensions(point) == 0) {
		return 0;
	}

	double x = point[0];
	double y = point[1];
	double f = 10 * pow((x * x - y), 2) + pow((1 - x), 2);

	return f;
}

F1::~F1() {

}

// f2(x,y) = (x-4)^2 + 4(y-2)^2
F2::F2() {
	dimensions = 2;
}

double F2::evaluate(vector<double> &point) {
	if (Function::check_dimensions(point) == 0) {
		return 0;
	}

	double x = point[0];
	double y = point[1];
	double f = pow(x-4, 2) + 4 * pow(y-2, 2);

	return f;
}

F2::~F2() {
	
}

// f3(x1,x2,x3,x4,x5) = (x1-p1)^2 + (x2-p2)^2 + ... + (x5-p5)^2
F3::F3(string filename) {
	// this->params = params;
	dimensions = 5;
	ifstream f(filename.c_str());
	if (!f.is_open()) {
		printf("Can't open file %s\n", filename.c_str());
		exit(1);
	}

	for (int i = 0; i < dimensions; i++) {
		double a;
		f >> a;
		params.push_back(a);
	}

	f.close();
}

double F3::evaluate(vector<double> &point) {
	if (Function::check_dimensions(point) == 0) {
		return 0;
	}

	double f = 0;
	for (int i = 0; i < dimensions; i++) {
		f += pow(point[i] - params[i], 2);
	}

	return f;
}

F3::~F3() {
	params.clear();
}

// f4(x,y) = |(x-y)*(x+y)| + (x2+y2)^0.5
F4::F4() {
	dimensions = 2;
}

double F4::evaluate(vector<double> &point) {
	if (Function::check_dimensions(point) == 0) {
		return 0;
	}

	double x = point[0];
	double y = point[1];	

	double f = fabs((x-y) * (x+y)) + pow((x*x + y*y), 0.5);
	return f;
}

F4::~F4() {
	
}
