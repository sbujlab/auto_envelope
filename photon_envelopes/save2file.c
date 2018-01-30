#include <stdio.h>
#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include <sstream>
#define PI 3.14159265
using namespace std;

int main() {
		const int numEnvs = 3;
		
		//TCanvas *c1 = new TCanvas("c1","envelope is intersect of black and red, not intersect of green",1800,700);
		//c1->Divide(numEnvs,1);
		
		// set up text file printing. Have k text files and loop through them.
		// print each collimator's (x,y) point contributions as seperate columns for easy differentiation.
		
		ofstream OutFiles[numEnvs];

		double targrad = 5.0;
		double targlen = 1500.0;

		double col2inrad = 35.3;
		double col2outrad = 98.0;
		double ang = PI/14.0;

		const int targetPoints = 8;
    const int col2points = 6;
    const int col4points = 10;
		const int col5points = 14;

		double posZ[numEnvs] = { 9775.0, 12800.0, 29047.5 };

		double targetX[targetPoints] = {  0.,     targrad,     0.,   -1.0*targrad,      0.,     targrad,     0.,    -1.0*targrad};
    double targetY[targetPoints] = {  targrad,     0.,    -1.0*targrad,    0.,      targrad,     0.,    -1.0*targrad,     0.};
		double targetZ[targetPoints] = {targlen/2.0,   targlen/2.0,   targlen/2.0,  targlen/2.0,   -1.0*targlen/2.0,  -1.0*targlen/2.0,  -1.0*targlen/2.0,  -1.0*targlen/2.0};
    double col2X[col2points] = {-1.0*col2inrad,  -1.0*col2inrad*cos(ang),   -1.0*col2outrad*cos(ang),   -1.0*col2outrad,    -1.0*col2outrad*cos(ang),    -1.0*col2inrad*cos(ang)};
    double col2Y[col2points] = {col2inrad*sin(0.0),    col2inrad*sin(ang),    col2outrad*sin(ang),      col2outrad*sin(0.0),     -1.0*col2outrad*sin(ang),    -1.0*col2inrad*sin(ang)};
		double col2Z = 5875.;
    double col2thickness = 100.0;

    double col4X[col4points] = {-65.01487639,-63.0,-106.5,-158.0,-228.0,-248.4932514,-228.0,-158.0,-106.5,-63.0};
    double col4Y[col4points] = {0.0,10.0,19.0,36.0,55.0,0.0,-55.0,-36.0,-19.0,-10.0};
    double col4Z = 9775.;// represents front face of collimator 4, 100 mm thickness is important as well.
    double col4thickness = 100.0;
    
    double detZ = 29047.5; //measuring from the origin of the detector system to the origin of the target.
   
	 	double col5X[col5points] = {-42.0,-42.0,-72.0,-72.0,-178.0,-178.0,-105.5,-105.5,-105.5,-178.0,-178.0,-72.0,-72.0,-42.0};
		double col5Y[col5points] = {0.0,2.5,2.5,13.5,55.0,30.0,8.5,0.0,-8.5,-30.0,-55.0,-13.5,-2.5,-2.5};
		double col5Z = 12800.0;
		double col5thickness = 70.0;


    double col4_upEnvX[targetPoints*col2points];
    double col4_upEnvY[targetPoints*col2points];
    
		double col5EnvX[targetPoints*col4points];
		double col5EnvY[targetPoints*col4points];
		
    double detEnvX[targetPoints*col5points];
    double detEnvY[targetPoints*col5points];
		
		/*TGraph *graph[numEnvs];
		TGraph *graph2[numEnvs];
		TGraph *graph3[numEnvs];
		
		stringstream pos[numEnvs];
		TString title[numEnvs];

		TMultiGraph *mg[numEnvs];
    */
    //cout << "Col 4 env has " << targetPoints*col2points << " points \n";

//for loop
	for (int k = 0; k<numEnvs; k++) {
	ostringstream strK;
	strK << posZ[k];
	string title;
  title += strK.str();
	title += "envelope";
	title += ".csv";
	OutFiles[k].open(title.c_str());

	OutFiles[k] << "Envelope at z = " << posZ[k] << "\n";
	OutFiles[k] << "Collimator 2 envelope\n";

		for (int i = 0; i<targetPoints; i++) {
        for (int j = 0; j<col2points; j++) {
            col4_upEnvX[(col2points*i)+j] = targetX[i] + ((col2X[j]-targetX[i])/(col2Z+col2thickness-targetZ[i]))*(posZ[k] - targetZ[i]);
            col4_upEnvY[(col2points*i)+j] = targetY[i] + ((col2Y[j]-targetY[i])/(col2Z+col2thickness-targetZ[i]))*(posZ[k] - targetZ[i]);
            cout << "Envelope passing through " << k+1 << " collimators, at Z = " << posZ[k] << ", Target point number " << i << ", and collimator 2 point " << j << endl;
						cout << (col2points*i)+j << "\t x = " << col4_upEnvX[(col2points*i)+j] << "\t y = " << col4_upEnvY[(col2points*i)+j] << endl;
						OutFiles[k] << col4_upEnvX[(col2points*i)+j] << "," << col4_upEnvY[(col2points*i)+j] << "\n";
				}
		}
    
    
		if (k > 0) {
		
		OutFiles[k] << "\n\n\nCollimator 4 envelope\n";

		for (int i = 0; i<targetPoints; i++) {
        for (int j = 0; j<col4points; j++) {
            col5EnvX[(col4points*i)+j] = targetX[i] + ((col4X[j]-targetX[i])/(col4Z+col4thickness-targetZ[i]))*(posZ[k] - targetZ[i]);
						col5EnvY[(col4points*i)+j] = targetY[i] + ((col4Y[j]-targetY[i])/(col4Z+col4thickness-targetZ[i]))*(posZ[k] - targetZ[i]);
            cout << "Envelope passing through " << k+1 << " collimators, at Z = " << posZ[k] << ", Target point number " << i << ", and collimator 4 point " << j << endl;
            cout << (col4points*i)+j << "\t x = " << col5EnvX[(col4points*i)+j] << "\t y = " << col5EnvY[(col4points*i)+j] << endl;
						OutFiles[k] << col5EnvX[(col4points*i)+j] << "," << col5EnvY[(col4points*i)+j] << "\n";
				}
    }
     
		if (k > 1) {
		
		OutFiles[k] << "\n\n\nCollimator 5 envelope\n";

		for (int i = 0; i<targetPoints; i++) {
        for (int j = 0; j<col5points; j++) {
            detEnvX[(col5points*i)+j] = targetX[i] + ((col5X[j]-targetX[i])/(col5Z+col5thickness-targetZ[i]))*(posZ[k] - targetZ[i]);
            detEnvY[(col5points*i)+j] = targetY[i] + ((col5Y[j]-targetY[i])/(col5Z+col5thickness-targetZ[i]))*(posZ[k] - targetZ[i]);
            cout << "Envelope passing through " << k+1 << " collimators, at Z = " << posZ[k] << ", Target point number " << i << ", and collimator 5 point " << j << endl;
            cout << (col5points*i)+j << "\t x = " << detEnvX[(col5points*i)+j] << "\t y = " << detEnvY[(col5points*i)+j] << endl;
						OutFiles[k] << detEnvX[(col5points*i)+j] << "," << detEnvY[(col5points*i)+j] << "\n";
        }
    }
		}
	  }	
		OutFiles[k].close();
/*		mg[k] = new TMultiGraph();

	  graph[k] = new TGraph(targetPoints*col2points,col4_upEnvX,col4_upEnvY);
		graph2[k] = new TGraph(targetPoints*col4points,col5EnvX,col5EnvY);
  	graph3[k] = new TGraph(targetPoints*col5points,detEnvX,detEnvY);
   
	 	graph[k]->SetMarkerStyle(20);
		graph[k]->SetMarkerColor(2);
		graph2[k]->SetMarkerStyle(20);
		graph2[k]->SetMarkerColor(1);
		graph3[k]->SetMarkerStyle(20);
		graph3[k]->SetMarkerColor(3);

		c1->cd(k+1);
	
		pos[k] << posZ[k];
		title[k] = new TString("detector plane envelope at z = " + pos[k].str() + ";x;y");
		//cout<<title[k]<<endl;
		//break;
		mg[k]->SetTitle(title[k]);
		mg[k]->Add(graph[k]);
		if (k>0) {
			mg[k]->Add(graph2[k]);
			if (k>1) {
				mg[k]->Add(graph3[k]);
			}
		}

		mg[k]->Draw("AP");
	//graphical if statements for different ks if needed	
		mg[k]->GetXaxis()->SetLimits(-800.0,0.0);
		mg[k]->GetYaxis()->SetRangeUser(-200.0,200.0);
		
		mg[k]->Draw("AP");
		c1->Update();
*/	}

return 0;	
}
