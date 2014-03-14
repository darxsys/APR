/**
    APR
    hj.cpp
    Purpose: Implements Hooke and Jeeves algorithm for function optimization.

    @author Dario Pavlovic
    @version 0.8 12/01/2013
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#include "hj.hpp"
#include "function.hpp"

HookeJeeves::HookeJeeves(Function* func, string starting_point, string error_file, string dx_file) {
	ifstream point(starting_point.c_str());
	if (!point.is_open()) {
		printf("Cant open file with starting point.\n");
		exit(1);
	}

	f = func;
	n = f->get_dimensions();
	for (int i = 0; i < n; i++) {
		double x;
		point >> x;
		x0.push_back(x);
	}

	point.close();
	dx.insert(dx.begin(), n, DX);
	e.insert(e.begin(), n, E);
	// What happens if there is less elements
	// than dimensions?
	if (x0.size() != n) {
		printf("Starting point not well defined.\n");
		exit(1);
	}

	if (dx_file != "") {
		ifstream param(dx_file.c_str());
		for (int i = 0; i < n; i++) {
			double a;
			param >> a;
			dx[i] = a;
		}
		param.close();
	}

	if (error_file != "") {
		ifstream param(error_file.c_str());
		for (int i = 0; i < n; i++) {
			double a;
			param >> a;
			e[i] = a;
		} 	
		param.close();	
	}
}

HookeJeeves::~HookeJeeves() {
	x0.clear();
	xb.clear();
	xp.clear();
	xn.clear();
	dx.clear();
	e.clear();
}

bool HookeJeeves::stop() {
	bool ret = true;
	for (int i = 0; i < dx.size(); i++) {
		if (dx[i] > e[i]) {
			ret = false;
			break;
		}
	}

	return ret;
}

vector<double> HookeJeeves::optimize() {
	xp = xb = x0;
	vector<double> new_p(n, 0);

	while (!stop()) {
		xn = search(xp);
		if (f->evaluate(xn) < f->evaluate(xb)) {
			for (int i = 0; i < n; i++) {
				new_p[i] = 2 * xn[i] - xb[i];
				xb = xn;
			}
			xp = new_p;
		} else {
			for (int i = 0; i < n; i++) {
				dx[i] /= 2;
			}
			xp = xb;
		}
	}

	return xb;
}

vector<double> HookeJeeves::search(vector<double> &point) {
	vector<double> x = point;
	double P, N;

	for (int i = 0; i < n; i++) {
		P = f->evaluate(x);
		x[i] += dx[i];
		N = f->evaluate(x);

		if (N > P) {
			x[i] -= 2 * dx[i];
			N = f->evaluate(x);

			if (N > P) {
				x[i] += dx[i];
			}
		}
	}

	return x;
}