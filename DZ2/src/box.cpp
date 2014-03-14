/**
    APR
    box.cpp
    Purpose: Implements Box algorithm for function optimization.

    @author Dario Pavlovic
    @version 0.8 12/01/2013
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#include "function.hpp"
#include "box.hpp"

Box::Box(Function *func, string starting_point, string error_file, double refl) {
	// cout << "here" << endl;
	ifstream point(starting_point.c_str());
	if (!point.is_open()) {
		printf("Cant open file with the starting point.\n");
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

	if (!implicit_check(x0, x0) || !explicit_check(x0)) {
		printf("Starting point does not satisfy explicit or implicit conditions.\n");
		exit(1);
	}

	e.insert(e.begin(), n, E);
	// What happens if there is less elements
	// than the dimension?
	if (x0.size() != n) {
		printf("Starting point not well defined.\n");
		exit(1);
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

	// Be careful here
	reflection = refl;
	// cout << reflection << endl;
	generate_points();
	// cout << "Done generating" << endl;	
}

Box::~Box() {
	for (int i = 0; i < 2 * n + 1; i++) {
		points[i].clear();
	}

	points.clear();
	xc.clear();
	x0.clear();
	e.clear();
} 

bool Box::explicit_check(vector<double> &point) {
	bool ret = true;
	for (int i = 0; i < point.size(); i++) {
		if (point[i] > UPPER) {
			point[i] = UPPER;
			ret = false;
		}
		if (point[i] < LOWER) {
			point[i] = LOWER;
			ret = false;
		}
	}

	return ret;
}

bool Box::implicit_check(vector<double> &point, vector<double> &xc) {
	bool ret = true;

	if (point == xc) {
		if (point[0] - point[1] > 0) {
			ret = false;
		}
		if (point[0] - 2 > 0) {
			ret = false;
		}
	} else {
		while ((point[0] - point[1] > 0) || (point[0] - 2 > 0)) {
			ret = false;
			for (int i = 0; i < n; i++) {
				point[i] = 0.5 * (point[i] + xc[i]);
			}
		}
	}

	return ret;
}

void Box::generate_points() {
	vector<double> new_point(n, 0);

	points.push_back(x0);
	xc = x0;
	double R;
	for (int t = 0; t < 2 * n; t++) {
		for (int i = 0; i < n; i++) {
			// TODO: Watch out for bugs here
			R = (double)rand() / (double)RAND_MAX;
			// cout << R << endl;
			new_point[i] = (double)LOWER + R * (UPPER - LOWER);
		}

		implicit_check(new_point, xc);
		points.push_back(new_point);
		calculate_centroid();
	}
	// cout << points.size() << endl;
}

void Box::calculate_centroid(int except) {
	double sum = 0;
	int num = points.size();

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < num; j++) {
			if (j == except) {
				continue;
			}

			sum += points[j][i];
		}
		sum /= (num-1);
		xc[i] = sum;
		sum = 0;
	}
}

bool Box::stop(vector<double> &xc, vector<double> &xh) {
	bool ret = true;

	for (int i = 0; i < n; i++) {
		if (fabs(xc[i] - xh[i]) > e[i]) {
			ret = false;
			break;
		}
	}

	return ret;
}

void Box::calculate_reflection(vector<double> &xc, vector<double> &xh) {
	for (int i = 0; i < n; i++) {
		xh[i] = (1 + reflection) * xc[i] - reflection * xh[i];
	}
}

vector<double> Box::optimize() {
	int worst, second_worst;
	double worst_eval, second_worst_eval;

	vector<double> xh;
	vector<double> evals(2 * n + 1, 0);

	do {
		worst = 0;
		second_worst = 0;
		worst_eval = f->evaluate(points[0]);
		second_worst_eval = worst_eval;

		for (int i = 1; i < 2 * n + 1; i++) {
			evals[i] = f->evaluate(points[i]);
			if (evals[i] > worst_eval) {
				worst = i;
				worst_eval = evals[i];
			}
			// put the lowest one into second worst
			if (evals[i] < second_worst_eval) {
				second_worst_eval = evals[i];
				second_worst = i;
			}
		}
		// printf("after first for\n");
		for (int i = 0; i < 2 * n + 1; i++) {
			if (i == worst) {
				continue;
			}

			if (evals[i] > second_worst_eval) {
				second_worst = i;
				second_worst_eval = evals[i];
			}
		}

		xh = points[worst];
		// printf("after second for\n");
		calculate_centroid(worst);
		// printf("after second for\n");
		calculate_reflection(xc, xh);
		// printf("after second for\n");
		explicit_check(xh);
		// printf("after second for\n");
		implicit_check(xh, xc);
		// printf("after second for\n");
		if (f->evaluate(xh) >= second_worst_eval) {
			for (int i = 0; i < n; i++) {
				xh[i] = 0.5 * (xh[i] + xc[i]);
			}
		}

		// cout << worst_eval << " " << second_worst_eval << endl;

		points[worst] = xh;
		// printf("after all\n");
	} while (!stop(xc, xh));

	// find solution
	int best = 0;
	double best_eval = evals[0];

	for (int i = 0; i < 2 * n + 1; i++) {
		if (evals[i] < best_eval) {
			best = i;
			best_eval = evals[i];
		}

		// cout << "(" << points[i][0] << "," << points[i][1] << ")" << endl;
	}

	return points[best];
}
