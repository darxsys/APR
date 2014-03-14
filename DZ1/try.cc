#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


int main(void) {
	
	string line;
	while( getline(cin, line)) {
		
		double a;
		stringstream stream(line);

		while(1) {

			stream >> a;
			if(!stream) {
				break;
			}

			cout << a;
		}
	}

	return 0;
}