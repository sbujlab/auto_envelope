#include <stdio.h>

void DetPlaneReachingEnv() {
		TCanvas *c1 = new TCanvas("c1","envelope reaching detector plane origin (not first scintillator face), intersect of black and red, not intersect of green",750,750);
		//c1->Divide(1,1);
				
		double targrad = 5.0;
		double targlen = 1500.0;

		double col2inrad = 35.3;
		double col2outrad = 98.0;
		double ang = 3.1415926535897932846/14.0;

		const int targetPoints = 8;
    const int col2points = 6;
    const int col4points = 10;
		const int col5points = 14;

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
    
    //cout << "Col 4 env has " << targetPoints*col2points << " points \n";



		for (int i = 0; i<targetPoints; i++) {
        for (int j = 0; j<col2points; j++) {
            col4_upEnvX[(col2points*i)+j] = targetX[i] + ((col2X[j]-targetX[i])/(col2Z+col2thickness-targetZ[i]))*(detZ - targetZ[i]);
            col4_upEnvY[(col2points*i)+j] = targetY[i] + ((col2Y[j]-targetY[i])/(col2Z+col2thickness-targetZ[i]))*(detZ - targetZ[i]);
            cout << (col2points*i)+j << "\t x = " << col4_upEnvX[(col2points*i)+j] << "\t y = " << col4_upEnvY[(col2points*i)+j] << endl;
        
        };
    };
    
		for (int i = 0; i<targetPoints; i++) {
        for (int j = 0; j<col4points; j++) {
            col5EnvX[(col4points*i)+j] = targetX[i] + ((col4X[j]-targetX[i])/(col4Z+col4thickness-targetZ[i]))*(detZ - targetZ[i]);
            col5EnvY[(col4points*i)+j] = targetY[i] + ((col4Y[j]-targetY[i])/(col4Z+col4thickness-targetZ[i]))*(detZ - targetZ[i]);
            cout << (col4points*i)+j << "\t x = " << col5EnvX[(col4points*i)+j] << "\t y = " << col5EnvY[(col4points*i)+j] << endl;
        
        };
    };
     
		for (int i = 0; i<targetPoints; i++) {
        for (int j = 0; j<col5points; j++) {
            detEnvX[(col5points*i)+j] = targetX[i] + ((col5X[j]-targetX[i])/(col5Z+col5thickness-targetZ[i]))*(detZ - targetZ[i]);
            detEnvY[(col5points*i)+j] = targetY[i] + ((col5Y[j]-targetY[i])/(col5Z+col5thickness-targetZ[i]))*(detZ - targetZ[i]);
            cout << (col5points*i)+j << "\t x = " << detEnvX[(col5points*i)+j] << "\t y = " << detEnvY[(col5points*i)+j] << endl;
        
        };
    };
    

    TGraph *graph = new TGraph(targetPoints*col2points,col4_upEnvX,col4_upEnvY);
    graph->SetMarkerStyle(20);
		graph->SetMarkerColor(2);
		//c1->cd(1);
		TGraph *graph2 = new TGraph(targetPoints*col4points,col5EnvX,col5EnvY);
		graph2->SetMarkerStyle(20);
		graph2->SetMarkerColor(1);
  	TGraph *graph3 = new TGraph(targetPoints*col5points,detEnvX,detEnvY);
		graph3->SetMarkerStyle(20);
		graph3->SetMarkerColor(3);

    //graph->Draw("AP");
		//graph2->Draw("P");
		//graph3->Draw("P");
		
		TMultiGraph *mg = new TMultiGraph();
		mg->SetTitle("detector plane envelope;x;y");

		mg->Add(graph);
		mg->Add(graph2);
		mg->Add(graph3);
		
		mg->Draw("AP");
		
		mg->GetXaxis()->SetRangeUser(0.0,-800.0);
		mg->GetYaxis()->SetRangeUser(-200.0,200.0);
		
		mg->Draw("AP");
		c1->Update();
}
