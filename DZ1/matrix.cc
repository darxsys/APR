#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

#include "matrix.h"
#include "utils.h"

Matrix::Matrix() {

	height = 0;
	width = 0;
	decomposed = false;
}

// Allocates a new matrix
Matrix::Matrix(int height, int width) {

	this->height = height;
	this->width = width;
	elements = (double*) malloc(sizeof(double) * height * width);
	decomposed = false;
}

// Copy constructor
Matrix::Matrix(const Matrix& source) {

	height = source.height;
	width = source.width;

	elements = (double*) malloc(sizeof(double) * height * width);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			// cout << "joining element: " << source.elements[i * width + j] << endl;
			elements[i * width + j] = source.elements[i * width + j];
		}
	}

	decomposed = source.decomposed;
}

// destructor
Matrix::~Matrix() {

	delete elements;
}

// Sets new dimensions of the matrix.
// Deletes all elements of the old matrix.
void Matrix::changeDimensions(int newHeight, int newWidth) {

	delete elements;

	height = newHeight;
	width = newWidth;

	elements = (double*) malloc(sizeof(double) * height * width);
}

int Matrix::getHeight() {

	return height;
}

int Matrix::getWidth() {

	return width;
}

// Returns a new matrix which is a transposed version
// of the original.
Matrix Matrix::transpose() {

	// reverse dimensions
	Matrix result(width, height);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			result[j * result.width + i] = elements[i * width + j];
		}
	}

	return result;
}

double& Matrix::operator[] (const int index) {

	return elements[index];
}

// Matrix to string
string Matrix::toString() {

	string out("");
	ostringstream s;
	string temp = "\n";

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {

			if (j < width - 1) {
				s << elements[i * width + j] << " ";
			} else {
				s << elements[i * width + j];	
			}
		}

		s << endl;
	}

	return s.str();
}

void Matrix::print() {

	cout << toString() << endl;
}

void Matrix::printToFile(string filename) {

	ofstream writer(filename.c_str());
	if (writer == NULL) {

		cout << "Could not open the file " << filename << " to output the matrix." << endl;
		return;
	}

	string matrix = toString();
	writer.write(matrix.c_str(), matrix.length());

	writer.close();
}


// Reads a matrix from file.
// If this method is called on an already
// existing matrix, the old matrix will be
// deleted and replaced by the new one.
void Matrix::read(string filename) {

	ifstream input(filename.c_str());
	if (input == NULL) {
		cout << "Failed opening file: " << filename << "." << endl;
		cout << "Exiting." << endl;
		exit(1);
	}

	if (height != 0 && width != 0) {
		// Delete the old matrix
		delete elements;
	}

	height = 0; width = 0;
	string line;

	// find number of rows
	while (getline(input, line)) {

		height++;
		if (width == 0) {
			// find the width in the first line
			stringstream stream(line);
			while (1) {

				double a;
				stream >> a;
				if (!stream) {
					break;
				}

				width++;
			}			
		}
	}

	// rollback to the beginning of the file
	input.clear();
	input.seekg(ios_base::beg);

	if (width == 0 || height == 0) {

		cout << "Incorrectly given input matrix. Exiting." << endl;
		input.close();
		exit(1);
	}

	elements = (double*) malloc(sizeof(double) * width * height);

	int i = 0;
	int j = 0;
	while (getline(input, line)) {
		// cout << "here" << endl;

		stringstream stream(line);
		while (1) {

			double a;
			stream >> a;
			if (!stream) {
				break;
			}

			elements[i * width + j] = a;
			// cout << "inputting: " << a << endl;
			j++;
		}
	}

	input.close();
}

void Matrix::setElement(int i, int j, double element) {

	elements[i * width + j] = element;
}

double Matrix::getElement(int i, int j) {

	return elements[i * width + j];
}

Matrix& Matrix::operator= (const Matrix& other) {

	// cout << "Here" << endl;
	if (this == &other) {
		return *this;
	}

	if (height != 0 && width != 0) {
		delete elements;
	}

	height = other.height;
	width = other.width;

	elements = (double*) malloc(sizeof(double) * height * width);

	for (int i  = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {

			elements[i * width + j] = other.elements[i * width + j];
		}
	}

	return (*this);
}

const Matrix Matrix::operator+ (const Matrix& other) {

	if (height != other.height || width != other.width) {
		cout << "ERROR: uncompatible matrices in the addition." << endl;
		// cout << "Have: " << toString() << "and: " << other.toString() << endl;
		exit(1);
	}

	Matrix result(height, width);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			result[i * width + j] = elements[i * width + j] + other.elements[i * width + j];
		}
	}

	return result;
}

const Matrix Matrix::operator- (const Matrix& other) {

	if (height != other.height || width != other.width) {
		cout << "ERROR: uncompatible matrices in the subtraction." << endl;
		// cout << "Have: " << toString() << "and: " << other.toString() << endl;
		exit(1);
	}

	Matrix result(height, width);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			result[i * width + j] = elements[i * width + j] - other.elements[i * width + j];
		}
	}

	return result;
}

const Matrix Matrix::operator* (const Matrix& other) {

	if (width != other.height) {

		cout << "ERROR: Uncompatible matrices in multiplication." << endl;
		exit(1);
	}

	Matrix result(height, other.width);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < other.width; j++) {
			result[i * other.width + j] = 0;

			for (int k = 0; k < width; k++) {
				result[i * other.width + j] += elements[i * width + k] * other.elements[k * other.width + j]; 
			}
		}
	}

	return result;
}

Matrix& Matrix::operator+= (const Matrix& other) {
	
	if (height != other.height || width != other.width) {
		cout << "ERROR: Incompatible matrices in addition." << endl;
		exit(1);
		// delete elements;
	}

	for (int i  = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {

			elements[i * width + j] += other.elements[i * width + j];
		}
	}

	return (*this);
}

Matrix& Matrix::operator-= (const Matrix& other) {

	if (height != other.height || width != other.width) {
		cout << "ERROR: Incompatible matrices in subtraction." << endl;
		exit(1);
		// delete elements;
	}

	for (int i  = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {

			elements[i * width + j] -= other.elements[i * width + j];
		}
	}

	return (*this);	
}

bool Matrix::operator== (const Matrix& other) {

	if (height != other.height || width != other.width) {
		return false;
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {

			double val = elements[i * width + j] - other.elements[i * width + j];
			
			if (fabs(val) > EPSILON) {
				return false;
			}
		}
	}

	return true;
}

// LU decomposition of a matrix. 
// If any pivot element during
// decomposition becomes zero,
// function returns effectively
// changing nothing.
int Matrix::LUdecompose() {

	if (width != height) {

		cout << "Can't do LU decomposition of non square matrix." << endl;
		return 0;
	}

	Matrix result(*this);

	int n = height;

	for (int i = 0; i < n - 1; i++) {

		if (fabs(result[i * width + i]) < EPSILON) {

			cout << "ERROR: Illegal pivot element: zero." << endl;
			return 0;
		}

		for (int j = i + 1; j < n; j++) {

			result[j * width + i] /= result[i * width + i];
			for (int k = i + 1; k < n; k++) {

				result[j * width + k] -= result[j * width + i] * result[i * width + k];	
			}
		}
	}

	*this = result;
	decomposed = true;
	return 1;
}


// LUP decomposition of a matrix
// If there is an ERROR during decomposition,
// effectively nothing happens.
int Matrix::LUPdecompose(Matrix& p) {

	if (width != height) {

		cout << "ERROR: Can't do LUP decomposition of non square matrix." << endl;
		return 0;
	}

	if (p.getHeight() != width || p.getWidth() != height) {

		cout << "ERROR: Permutation matrix not properly given." << endl;
		return 0;
	}

	initializePermutationMatrix(p);
	Matrix result(*this);
	int n = height;

	for (int i = 0; i < n - 1; i++) {

		int pivot = selectPivotElement(result, i);

		if (fabs(result[pivot * width + i]) < EPSILON) {

			cout << "ERROR: Encountered zero pivot element." << endl;
			return 0;
		}

		switchMatrixRows(result, i, pivot);
		switchMatrixRows(p, i, pivot);

		for (int j = i + 1; j < n; j++) {

			result[j * width + i] /= result[i * width + i];
			for (int k = i + 1; k < n; k++) {

				result[j * width + k] -= result[j * width + i] * result[i * width + k];
			}
		}
	}

	*this = result;
	return 1;
}

// Initializes permutation matrix to a diagonal matrix
void Matrix::initializePermutationMatrix(Matrix &p) {

	for (int i = 0; i < p.height; i++) {
		for(int j = 0; j < p.height; j++) {

			if (i == j) {
				p[i * width + j] = 1;
			} else {
				p[i * width + j] = 0;
			}
		}
	}
}

// Switches positions of rows i and j
void Matrix::switchMatrixRows(Matrix& p, int i, int j) {

	double temp;
	for (int k = 0; k < width; k++) {

		temp = p[i * width + k];
		p[i * width + k] = p[j * width + k];
		p[j* width + k] = temp; 
	}
}

// Selects the largest element of a column starting from 
// "columnth" element
int Matrix::selectPivotElement(Matrix& p, int column) {

	int max = column;
	int width = p.getWidth();
	int height = p.getHeight();

	for (int i = column + 1; i < height; i++) {

		if (fabs(p[i * width + column]) > fabs(p[max * width + column])) {
			max = i;
		}
	}

	return max;
}

// Does forward supstitution Ly = b
// result y is stored in b
void Matrix::forwardSupstitution(Matrix& b) {

	if (b.getWidth() != 1 || b.getHeight() != height) {

		cout << "ERROR: Vector b is not compatible with this matrix." << endl;
		exit(1);
	}

	int n = height;

	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			b[j] -= elements[j * width + i] * b[i];
		}
	}
}

// Does backward supstitution Ux = y
// result x is stored in y
void Matrix::backwardSupstitution(Matrix& y) {

	if (y.getWidth() != 1 || y.getHeight() != height) {

		cout << "ERROR: Vector y is not compatible with this matrix." << endl;
		exit(1);
	}

	int n = height;

	for (int i = n - 1; i >-1; i--) {

		y[i] /= elements[i * width + i];
		for (int j = 0; j < i; j++) {

			y[j] -= elements[j * width + i] * y[i];
		}
	}
}

void Matrix::printLowerTriangular() {

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {

			if (i == j) {
				cout << "1 ";
			} else if (j < i) {
				cout << elements[i * width + j] << " ";
			} else {
				cout << "0 ";
			}
		}

		cout << endl;
	}
}

void Matrix::printUpperTriangular() {

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {

			if (i <= j) {
				cout << elements[i * width + j] << " ";
			} else {
				cout << "0 ";
			}
		}

		cout << endl;
	}
}


int main(int argc, char *argv[]) {

	if (argc != 3) {

		cout << "matrix and vector b files needed." << endl;
		exit(1);
	}

	bool LUP = false;
	string method;
	cout << "Method? {LU, LUP}" << endl;
	cin >> method;

	if (method == "LUP") {

		LUP = true;
	} else if (method == "LU") {

		LUP = false;
	} else {

		cout << "ERROR: Not an option." << endl;
		exit(1);
	}

	Matrix m;
	m.read(argv[1]);
	cout << "Matrix A:" << endl;
	cout << m.toString() << endl;

	Matrix b;
	b.read(argv[2]);
	cout << "Vector b:" << endl;
	cout << b.toString() << endl;

	if (LUP) {
		
		cout << "LUP decomposition..." << endl;
		cout << "########" << endl;

		Matrix p(m);
		int status = m.LUPdecompose(p);

		if (status == 0) {
			exit(1);
		}

		cout << "Permutation matrix" << endl;
		cout << p.toString() << endl;

		b = p * b;
		cout << "New vector b:" << endl;
		cout << b.toString() << endl;

	} else {

		cout << "LU decomposition..." << endl;
		cout << "########" << endl;

		int status = m.LUdecompose();
		if (status == 0) {
			exit(1);
		}
	}

	cout << "Lower triangular matrix:" << endl;
	m.printLowerTriangular();
	cout << endl;

	cout << "Upper triangular matrix:" << endl;
	m.printUpperTriangular();
	cout << endl;

	// cout << "Upper and lower triangular matrices:" << endl;
	// cout << m.toString() << endl;

	int height = m.getHeight();
	int width = m.getWidth();

	if (fabs(m[(height - 1) * width + width - 1]) < EPSILON) {

		cout << "ERROR: Matrix A has 0 as its last element. Not possible to solve." << endl;
		exit(1);
	}

	cout << "Doing forward supstitution." << endl;
	m.forwardSupstitution(b);
	cout << "Vector y:" << endl;
	cout << b.toString() << endl;

	m.backwardSupstitution(b);
	cout << "Solution:" << endl;
	cout << "########" << endl;
	cout << b.toString() << endl;

	return 0;
}

