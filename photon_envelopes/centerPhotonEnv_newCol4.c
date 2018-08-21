#include <stdio.h>
#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include <sstream>
using namespace std;

int main() {
		
    ofstream OutFile;


    double targrad = 5.0;
    double targlen = 1500.0;
    double targetZ = targlen/2;
 
    //This is a cone, but just chose the location of the smallest radius
    double col1rad = 15.53;
    double col1Z = 5575.0;
    double col1thickness = 125.4;

    double col2rad = 26.0; //Can change to 29.55?
    double col2Z = 5875.0;
    double col2thickness = 100.0;

    //This is the new verion. The old collimator 4 is at 9775 mm with 30mm radius
    double col4rad = 44.777;
    double col4Z = 8275; //
    double col4thickness = 100.0;
   
    //This is not solid so...
    double col5rad = 42.0;
    double col5Z = 12800.0;
    double col5thickness = 70.0;

    double y = 0.0;
    double detZ = 29047.5; //measuring from the origin of the detector system to the origin of the target.


    double col2EnvX; 
    double col4EnvX; 
    double col5EnvX; 
    double detEnvX; 


    OutFile.open("PhotonEnvelope_newCol4.txt");

    OutFile << col1rad << "\t" << y << "\t" << col1Z << "\r\n";
    OutFile << col1rad << "\t" << y << "\t" << col1Z+col1thickness << "\r\n";
    //new x = old x + dx/dz (slope of line from bottom of target to top of collimator 1) * dz (distance between target and collimator 2)
    col2EnvX = targrad +  ((col1rad - targrad)/(col1Z - targetZ)) * (col2Z - targetZ);
    OutFile << col2EnvX << "\t" << y << "\t" << col2Z << "\r\n";

    OutFile << col2rad << "\t" << y << "\t" << col2Z << "\r\n";
    OutFile << col2rad << "\t" << y << "\t" << col2Z+col2thickness << "\r\n";
    col4EnvX = targrad +  ((col2rad - targrad)/(col2Z - targetZ)) * (col4Z - targetZ);
    OutFile << col4EnvX << "\t" << y << "\t" << col4Z << "\r\n";

    OutFile << col4rad << "\t" << y << "\t" << col4Z << "\r\n";
    OutFile << col4rad << "\t" << y << "\t" << col4Z+col4thickness << "\r\n";
    detEnvX = targrad +  ((col4rad - targrad)/(col4Z - targetZ)) * (detZ - targetZ);
    OutFile << detEnvX << "\t" << y << "\t" << detZ << "\r\n";

    //OutFile << col5rad << "\t" << y << "\t" << col5Z << "\r\n";
    //OutFile << col5rad << "\t" << y << "\t" << col5Z+col5thickness << "\r\n";
    //detEnvX = targrad +  ((col5rad - targrad)/(col5Z - targetZ)) * (detZ - targetZ);
    //OutFile << detEnvX << "\t" << y << "\t" << detZ << "\r\n";

    OutFile.close();

    return 0;	
}
