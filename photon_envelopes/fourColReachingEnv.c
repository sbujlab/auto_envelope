#include <stdio.h>

void fourColReachingEnv() {
		TCanvas *c1 = new TCanvas("c1","Envelope reaching collimator 4",750,750);
		//c1->Divide(1,1);
				
		double targrad = 5.0;
		double targlen = 1500.0;

		double col2inrad = 35.3;
		double col2outrad = 98.0;
		double ang = 3.1415926535897932846/14.0;

		const int targetPoints = 8;
    const int col2points = 6;
    const int col4points = 10;

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
    
    //double detZ = 29047.5; //measuring from the origin of the detector system to the origin of the target.
    
    double col4_upEnvX[targetPoints*col2points];
    double col4_upEnvY[targetPoints*col2points];
    
    //double detEnvX[targetPoints*col4points];
    //double detEnvY[targetPoints*col4points];
    
    //cout << "Col 4 env has " << targetPoints*col2points << " points \n";
    
    for (int i = 0; i<targetPoints; i++) {
        for (int j = 0; j<col2points; j++) {
            col4_upEnvX[(col2points*i)+j] = targetX[i] + ((col2X[j]-targetX[i])/(col2Z+col2thickness-targetZ[i]))*(col4Z - targetZ[i]);
            col4_upEnvY[(col2points*i)+j] = targetY[i] + ((col2Y[j]-targetY[i])/(col2Z+col2thickness-targetZ[i]))*(col4Z - targetZ[i]);
            cout << (col2points*i)+j << "\t x = " << col4_upEnvX[(col2points*i)+j] << "\t y = " << col4_upEnvY[(col2points*i)+j] << endl;
        
        };
    };
    
    TGraph *graph = new TGraph(targetPoints*col2points,col4_upEnvX,col4_upEnvY);
    graph->SetMarkerStyle(20);
		graph->SetMarkerColor(2);
		//c1->cd(1);
    graph->Draw("AP");   
}
