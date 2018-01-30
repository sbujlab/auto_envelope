#include <stdio.h>


void photonEnvelope() {
		TCanvas *c1 = new TCanvas("c1","Canvas",1500,750);
		c1->Divide(2,1);
				

		const int targetPoints = 8;
    const int col2points = 6;
    const int col4points = 7;
    
    double targetX[targetPoints] = {  0.,     5.,     0.,   -5.,      0.,     5.,     0.,    -5.};
    double targetY[targetPoints] = {  5.,     0.,    -5.,    0.,      5.,     0.,    -5.,     0.};
    double targetZ[targetPoints] = {750.,   750.,   750.,  750.,   -750.,  -750.,  -750.,  -750.};
    
    double col2X[col2points] = {-35.3,  -35.3*cos(0.224449342),   -106.1*cos(0.224449342),   -106.1,    -106.1*cos(0.224449342),    -35.3*cos(0.224449342)};
    double col2Y[col2points] = {  0.,    35.3*sin(0.224449342),    106.1*sin(0.224449342),      0.,     -106.1*sin(0.224449342),    -35.3*sin(0.224449342)};
    double col2Z = 5875.;
    double col2thickness = 100.;
    
    double col4X[col4points] = {-20.,-70.,-70.,-60.,-50.,-30.,-20.};
    double col4Y[col4points] = {0.,0.,50.,40.,25.,20.,20.};
    double col4Z = 9875.;
    double col4thickness = 100;
    
    double detZ = 30000.;
    
    
    double col4_upEnvX[targetPoints*col2points];
    double col4_upEnvY[targetPoints*col2points];
    
    
    
    double detEnvX[targetPoints*col4points];
    double detEnvY[targetPoints*col4points];
    
    
    //cout << "Col 4 env has " << targetPoints*col2points << " points \n";
    
    for (int i = 0; i<targetPoints; i++) {
        for (int j = 0; j<col2points; j++) {
            col4_upEnvX[(col2points*i)+j] = targetX[i] + ((col2X[j]-targetX[i])/(col2Z+col2thickness-targetZ[i]))*(col4Z - targetZ[i]);
            col4_upEnvY[(col2points*i)+j] = targetY[i] + ((col2Y[j]-targetY[i])/(col2Z+col2thickness-targetZ[i]))*(col4Z - targetZ[i]);
            cout << (col2points*i)+j << "\t x = " << col4_upEnvX[i+j] << "\t y = " << col4_upEnvY[i+j] << endl;
        
        }
        
    }
    
    //work on this part

    for (int i = 0; i<targetPoints; i++) {
        for (int j = 0; j<col4points; j++) {
            detEnvX[(col4points*i)+j] = targetX[i] + ((col4X[j]-targetX[i])/(col4Z+col4thickness-targetZ[i]))*(detZ - targetZ[i]);
            detEnvY[(col4points*i)+j] = targetY[i] + ((col4Y[j]-targetY[i])/(col4Z+col4thickness-targetZ[i]))*(detZ - targetZ[i]);
            
        }
        
    }
    
    TGraph *graph = new TGraph(targetPoints*col2points,col4_upEnvX,col4_upEnvY);
    graph->SetMarkerStyle(20);


		c1->cd(1);
    graph->Draw("AP");

    TGraph *graph2 = new TGraph(targetPoints*col4points,detEnvX,detEnvY);
    graph2->SetMarkerStyle(20);

    c1->cd(2);
    graph2->Draw("AP");
    
    
    
    
}



