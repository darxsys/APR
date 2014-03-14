#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

#include "function.hpp"
#include "f1.hpp"
#include "f2.hpp"
#include "f3.hpp"
#include "f4.hpp"
#include "hj.hpp"
#include "box.hpp"

option options[] = {
	{"algo", required_argument, 0, 'a'},
	{"start", required_argument, 0, 's'},
	{"precision", optional_argument, 0, 'p'},
	{"dx", optional_argument, 0, 'd'},
	{"reflection", optional_argument, 0, 'r'},
	{"function", required_argument, 0, 'f'},
	{"fparams", optional_argument, 0, 'x'},
	{NULL, 0, 0, 0}
}; 

void help() {
	printf("./optimizer <arguments>\n");
	printf("Valid arguments:\n");
	printf("REQUIRED: --algo=<value>, <value> = {hooke, box}\n");
	printf("REQUIRED: --start=<value>, <value> = file that contains the starting point\n");
	printf("REQUIRED: --function=<value>, <value> = {f1, f2, f3, f4}\n");
	printf("REQUIRED (in case of functions with variable parameters: "
		"--fparams=<value>, <value> = file that contains p1, p2, etc...\n");
	printf("OPTIONAL: --precision=<value>, <value> = file that contains n dimensional "
		"vector of desired precision. Default: 1e-9 for every element.\n");
	printf("OPTIONAL: --dx=<value>, <value> = file that contains n dimensional vector "
		"of elements for values of movement for each variable. Applicable only to "
		"Hooke Jeeves.\n");
	printf("OPTIONAL: --reflection=<value>, <value> = real number that represents coefficient "
		"of reflection for Box algorithm.\n");

	exit(1);
}

void output_solution(vector<double> &solution) {
	stringstream out;
	out << "Rjesenje: (";
	for (int i = 0; i < solution.size(); i++) {
		out << solution[i] << ",";		
	}

	string put = out.str();
	put[put.length()-1] = ')';

	cout << put << endl;
}

vector<double> optimize_f_hj(Function *f, string starting_point, 
		string error_file="", string dx_file="") {
	HookeJeeves h(f, starting_point, error_file, dx_file);
	return h.optimize();
}

vector<double> optimize_f_box(Function *f, string starting_point, 
		string error_file="", double a = 1.3) {
	Box b(f, starting_point, error_file, a);
	return b.optimize();
}

int main(int argc, char **argv) {
	srand((unsigned)time(NULL));
	string starting_point = "";
	string error_file="";
	string dx_file="";
	string algo = "";
	string fparams = "";
	string function = "";
	double reflection = 1.3;
	int status = 0;

	while (1) {
		char argument = getopt_long_only(argc, argv, "", options, &status);
		// cout << argument << endl;

		if (argument == -1) {
			break;
		}

		switch(argument) {
		case 'a':
			algo = optarg;
			break;
		case 's':
			starting_point = optarg;
			break;
		case 'p':
			error_file = optarg;
			break;
		case 'd':
			dx_file = optarg;
			break;
		case 'r':
			reflection = atof(optarg);
			break;
		case 'f':
			function = optarg;
			break;
		case 'x':
			fparams = optarg;
			break;
		default:
			//TODO make help
			help();
			break;
		}
	}

	if (starting_point == "") {
		printf("Missing argument: starting point.\n");
		exit(1);
	}
	if (algo == "") {
		printf("Missing argument: algo.\n");
		exit(1);
	}
	if (function == "") {
		printf("Missing argument: function to optimize.\n");
		exit(1);
	}

	if (function == "f3" && fparams == "") {
		printf("Function %s requires a file with parameters.\n", function.c_str());
		exit(1);
	}

	Function *F;
	if (function == "f1") {
		F = new F1;
	} else if (function == "f2") {
		F = new F2;
	} else if (function == "f3") {
		F = new F3(fparams);
	} else if (function == "f4") {
		F = new F4;
	} else {
		printf ("Unsupported function.\n");
		exit(1);
	}

	vector<double> res;
	if (algo == "hooke") {
		res = optimize_f_hj(F, starting_point, error_file, dx_file);
	} else if (algo == "box") {
		// TODO
		res = optimize_f_box(F, starting_point, error_file, reflection);
	} else {
		printf("Unsupported algo.\n");
		exit(1);
	}

	delete F;
	output_solution(res);
	return 0;
}
