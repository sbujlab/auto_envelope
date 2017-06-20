#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

void counter(){

	int particleID;
	int total_number_of_tracks = 0;
	int sameEvent;
	double xi, yi, zi, xf, yf, zf, E, P;
	ifstream data("position_output.txt",ios::in);
	
	data >> particleID;

	while(!data.eof()) {
		sameEvent = 1;
		data >> xi >> yi >> zi;
		data >> xf >> yf >> zf;
		while(sameEvent) {
			data >> xf >> yf >> zf;
			if ((xf==-29 && yf==-29 && zf==-29) || (xf==-37 && yf==-37 && zf==-37)) {
				sameEvent = 0;
			}
			if (xf==-29 && yf==-29 && zf==-29){
				data >> E >> P;
			}
		}
		if(!data.eof()) {
			data >> particleID;
		}	
		total_number_of_tracks++;
	}
	data.close();
	std::cout << total_number_of_tracks << std::endl;
}
