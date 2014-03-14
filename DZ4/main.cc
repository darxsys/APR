#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

#include "matrix.h"

void write(ofstream &out, Matrix x0, int counter, int step, double i) {
    if (counter == step) {
        cout << i << " ";
    }

    out << i << " ";
    for (int j = 0; j < x0.getHeight(); j++) {
        if (counter == step) {
            cout << x0[j] << " ";
        }
        out << x0[j] << " ";
    }
    if (counter == step) {
        cout << endl;
    }
    out << endl;
}

void solve(int argc, char **argv) {
    double T_t = atof(argv[4]);
    double T_r = atof(argv[5]);
    // cout << T_r << endl;
    double interval_lower = atof(argv[6]);
    double interval_upper = atof(argv[7]);
    // cout << interval_upper << endl;
    int step = atoi(argv[8]);
    char* out_trapesian = argv[9];
    char* out_runge = argv[10];

    ofstream out_t(out_trapesian);
    ofstream out_r(out_runge);
    Matrix A;
    A.read(argv[1]);
    // cout << A.toString() << endl;

    Matrix B;
    B.read(argv[2]);

    Matrix x0_t;
    x0_t.read(argv[3]);
    Matrix x0_r(x0_t);

    Matrix R(A.getHeight(), A.getWidth());
    Matrix E(A.getHeight(), A.getWidth());
    Matrix S;
    R.initializePermutationMatrix(E);
    // cout << E.toString() << endl;
    // Trapez
    Matrix A_ = A;
    A_ = A_ * (T_t / 2);
    Matrix inv;
    inv = (E - A_).invert();
    R = inv * (E + A_);
    S = (inv * (T_t)) * B;    

    // Runge Kutta
    Matrix n1, n2, n3, n4;

    int counter = 0;
    for (double i_t = interval_lower, i_r = interval_lower; 
            i_t < interval_upper || i_r < interval_upper; 
            i_t += T_t, i_r += T_r, counter++) {

        // Trapez
        if (i_t < interval_upper) {
            write(out_t, x0_t, counter, step, i_t);
            x0_t = (R * x0_t) + S;
        }

        // Runge Kutta
        if (i_r < interval_upper) {
            write(out_r, x0_r, counter, step, i_r);
            // Runge Kutta
            n1 = (A * x0_r) + B;
            n2 = (A * (x0_r + n1 * (T_r/2))) + B;
            n3 = (A * (x0_r + n2 * (T_r/2))) + B;
            n4 = (A * (x0_r + n3 * T_r)) + B;
            x0_r = x0_r + ((n1 + (n2 * 2) + (n3 * 2) + n4) * (T_r/6));
            // x0_r = 
        }

        if (counter == step) {
            counter = 0;
        }
    }

    // write(out_t, x0_t, 0, -1, interval_upper);
    // write(out_r, x0_r, 0, -1, interval_upper);


    out_t.close();
    out_r.close();
}

int main(int argc, char **argv) {
    if (argc != 11) {
        cout << "Wrong number of arguments." << endl;
        cout << "<matrix_A> <matrix_B> <x0> <T_trapesian> <T_runge> <interval_lower> <interval_upper> <output_step> <output_trapes_file> <output_rk>" << endl;
        exit(1);
    }

    solve(argc, argv);
    return 0;
}