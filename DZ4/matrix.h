#ifndef MATRIX_H
#define MATRIX_H

#include <string>

class Matrix {
public:
	Matrix();
	// Copy constructor
	Matrix(const Matrix& source);
	// Constructor which allocates memory for the matrix
	Matrix(int height, int width);
	// Destructor which deallocates matrix
	~Matrix();
	// Reads a matrix from a file
	void read(string filename);
	// Prints the matrix to stdout
	void print();
	void printToFile(string filename);
	// Access to an element (acces over operator [] prefered)
	double getElement(int i, int j);
	// Set a particular element
	void setElement(int i, int j, double element);
	// Access an element of the matrix over [] operator (prefered)
	double& operator[] (const int index);
	// Transpose
	Matrix transpose();
	string toString();
	// Get dimensions
	int getHeight();
	int getWidth();
	// Method for changing matrix dimensions.
	// BE CAREFUL: old matrix' elements get deleted when doing this
	void changeDimensions(int newHeight, int newWidth);
	// Operators
	Matrix& operator= (const Matrix& source);
	Matrix operator+ (const Matrix& other);
	Matrix operator- (const Matrix& other);
	Matrix operator* (const Matrix& other);
	Matrix& operator+= (const Matrix& other);
	Matrix& operator-= (const Matrix& other);
	bool operator== (const Matrix& other);
	Matrix operator* (const double T);

	Matrix invert();

	int LUdecompose();
	int LUPdecompose(Matrix& p);

	void forwardSupstitution(Matrix& b);
	void backwardSupstitution(Matrix& b);

	void printLowerTriangular();
	void printUpperTriangular();
	void initializePermutationMatrix(Matrix &p);

protected:
	int height, width;
	double* elements;
	bool decomposed;

	void switchMatrixRows(Matrix &p, int j, int k);
	int selectPivotElement(Matrix& p, int column);
};

#endif