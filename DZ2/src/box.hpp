/**
    APR
    box.hpp
    Purpose: Defines a class for implementing Box alogrithm 
    for function optimization.

    @author Dario Pavlovic
    @version 0.8 12/01/2013
*/

#ifndef BOX_HPP
#define BOX_HPP

#include <vector>
using namespace std;

#include "function.hpp"

#define E 1e-9
#define A 1.3
#define UPPER 100
#define LOWER -100

class Box {
public:
	Box(Function *func, string starting_point, string error_file="", double refl = 1.3);
	~Box(); 
	vector<double> optimize();
private:
	void generate_points();
	bool explicit_check(vector<double> &point);
	bool implicit_check(vector<double> &point, vector<double> &xc);
	void calculate_centroid(int except = -1);
	void calculate_reflection(vector<double> &xc, vector<double> &xh);
	bool stop(vector<double> &xc, vector<double> &xh);

	vector<vector<double> > points;
	vector<double> xc;
	vector<double> x0;
	vector<double> e;
	Function* f;
	double reflection;
	int n;
};

#endif