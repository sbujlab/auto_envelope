#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "TMinuit.h"
#include "TFitter.h"
//using namespace std;

// Constants/variables needed by whichSeptant and zPlane
double pi = 3.14159265358979323846;
const int numTracks = 25162809; //3080000; // I think this actually needs to be dynamic.
double sep = 360./7.;


// Filled by whichSeptant, used by zPlane
int septantData[numTracks][2];

// Filled by zPlane, used by plotPoints_sep
double particlesX[numTracks];
double particlesY[numTracks];
int goodParticles;


// This reads through the text file once and determines which septant the particles were originally passing through when they reach the acceptance defining collimator, and stores that informatio for later.
void whichSeptant() {
    
    ifstream sepData("position_output.txt",ios::in);
    
    double xcol, ycol, rcol, xi, yi, zi, xf, yf, zf, dx, dy, dz, mx, my;
    double r, theta;
    double vertexBeamP, P_initial, P_final, E_final, deltaE;
    int particleID;
    double colPos = 5875.;
    
    int sameEvent;
    
    for(int i = 0; i<numTracks; i++) {

//	cout << i << endl;
        
        sameEvent = 1;
        
        //sepData >> vertexBeamP >> P_initial; // Must add this print command when the parents are generated.
        sepData >> particleID;
        sepData >> xi >> yi >> zi;
        sepData >> xf >> yf >> zf;
        
        septantData[i][0] = particleID;
        //cout << i+1 << "\n" ;
        //cout << xf << "\t" << yf << "\t" << zf << "\n";
        
        while (sameEvent) {
            
            if (zi < colPos && zf > colPos) {
                dx = xf - xi;
                dy = yf - yi;
                dz = zf - zi;
                mx = dx/dz;
                my = dy/dz;
                xcol = xi + mx*(colPos - zi);
                ycol = yi + my*(colPos - zi);
                
                r = sqrt(xcol*xcol + ycol*ycol);
                theta = atan2(ycol,xcol) * 180 / pi;
                
                if (theta < 0.) {
                    theta += 360.;
                }
                
                if(theta > 0. && theta < sep) {septantData[i][1] = 4;}
                if(theta > sep && theta < 2*sep) {septantData[i][1] = 3;}
                if(theta > 2*sep && theta < 3*sep) {septantData[i][1] = 2;}
                if(theta > 3*sep && theta < 4*sep) {septantData[i][1] = 1;}
                if(theta > 4*sep && theta < 5*sep) {septantData[i][1] = 7;}
                if(theta > 5*sep && theta < 6*sep) {septantData[i][1] = 6;}
                if(theta > 6*sep && theta < 7*sep) {septantData[i][1] = 5;}
            }
            
            xi = xf;
            yi = yf;
            zi = zf;
            sepData >> xf >> yf >> zf;
            //cout << xf << "\t" << yf << "\t" << zf << "\n";
            if ((xf == -29 && yf == -29 && zf == -29) || (xf == -37 && yf == -37 && zf == -37)) {
                sameEvent = 0;
            }
            if (xf == -29 && yf == -29 && zf == -29) {
                sepData >> E_final >> P_final;
            }
        }
    }
    
    sepData.close();
    /*
    for (int v = 0; v<numTracks; v++) {
        cout << septantData[v][0] << "\t" << septantData[v][1] << "\n";
    }
   */ 
}



void zPlane(double zPos) {

    cout << zPos << endl;
    ifstream posData("position_output.txt",ios::in);
    
    goodParticles = 0;
    double x, y, xi, yi, zi, xf, yf, zf, dx, dy, dz, mx, my, xdet, ydet, rdet;
    double r, theta;
    double vertexBeamP, P_initial, P_final, E_final, deltaE;
    int particleID;
    int sameEvent;
    
    
    for (int i = 0; i < numTracks; i++) {
        
//        cout << i << "\n";
        
        sameEvent = 1;                                                                      // Flag will be changed to 0 when end of event is reached
        
        //posData >> vertexBeamP >> P_initial; // This data must be added into the files at the generation point.
        posData >> particleID;
        posData >> xi >> yi >> zi;
        posData >> xf >> yf >> zf;
        
        while (sameEvent) {
            if (zPos == zi) {
                x = xi;
                y = yi;
            }
            if (zPos == zf) {
                x = xf;
                y = yf;
            }
            if (zPos < zf && zPos > zi) {                                                   // Check if the desired z-position is in between these two points
                dx = xf - xi;                                                               // If so, interpolate the xy-position at the desired z-position
                dy = yf - yi;
                dz = zf - zi;
                mx = dx/dz;
                my = dy/dz;
                x = xi + mx*(zPos - zi);
                y = yi + my*(zPos - zi);

            }
            
            if (28500. < zf && 28500. > zi) {
                dx = xf - xi;                                                               // If so, interpolate the xy-position at the desired z-position
                dy = yf - yi;
                dz = zf - zi;
                mx = dx/dz;
                my = dy/dz;
                xdet = xi + mx*(28500. - zi);
                ydet = yi + my*(28500. - zi);
                rdet = sqrt(xdet*xdet+ydet*ydet);
            }
            xi = xf;                                                                           // Shift to the next postition pair
            yi = yf;
            zi = zf;
            posData >> xf >> yf >> zf;
            if ((xf == -29 && yf == -29 && zf == -29) || (xf == -37 && yf == -37 && zf == -37)) {
                sameEvent = 0;
            }
        }
        if (xf == -29 && yf == -29 && zf ==-29) {                                           // Check if this is a good event (hit the detector)
            posData >> E_final >> P_final;
            //deltaE = (11. - vertexBeamP) + (P_initial - P_final); This information must be added at the generation of the particles.
            
            //if (deltaE <= 1.) {
                
                if (rdet > 1200. && rdet < 1900. ){ // WAS: 1572.) {
                    
                    r = sqrt((x*x) + (y*y));                                                        // Convert to polar coordinates
                    
                    theta = atan2(y,x) * 180 / pi;
                    
                    if (theta < 0.) {
                        theta += 360.;
                    }

                    if (particleID != septantData[i][0]) {cout << "MISMATCH!!! \n";}
                    //cout << "Septant ID = " << septantData[i][0] << "\t Particle ID = " << particleID << "\n";
                
                    theta+=(septantData[i][1] - 1)*sep;
                    
                    particlesX[goodParticles] = r*cos(theta*pi/180.);
                    particlesY[goodParticles] = fabs(r*sin(theta*pi/180.));
        
                    goodParticles++;
                    
                }
           // }
        }
    }

    posData.close();

}


void peripheralPlotPoints_sep( int start_point_input = 10000 ) {

    gROOT->SetBatch();
    
    whichSeptant();
    
    double thisZ;
    char fileout[100];
   
    int startPoint = start_point_input; // 10000; // millimeters 
    double endPoint = start_point_input*1.0 + 1000.0; // 30000.0; // millimeters

    for (int l = startPoint; l <= endPoint; l=l+10) { // WAS: 2550; l++) {

	// Go until 30,000 mm, or 30 meters downstream	
	// Start at 4500 mm (but for peripheral studies I can start at end of hybrid: around 20 meters downstream    
        thisZ = l*1.0; // WAS: 4500. + l*10.; where l was in centimeters
        
        //cout << thisZ << "\n";
       
        // Call the zplane function	
        zPlane(thisZ);
        
        
        //if (thisZ < 10000.) {
        //    sprintf(fileout, "peripheral/points0%d.txt",thisZ);
        //} else {
            sprintf(fileout, "peripheral/points%d.txt",thisZ);
        //}
        
        ofstream outfile;
        outfile.open (fileout);
       
        outfile << thisZ << "\n";
        
        for (int j = 0; j < goodParticles; j++) {
            
            outfile << particlesX[j] << "  \t"  << particlesY[j] << "\n";
            
        }
        outfile.close();
        
        char title[50];
        
        sprintf(title,"z = %.2lf m",thisZ/1000.);
        
        //if (thisZ < 10000.) {
        //    sprintf(fileout, "peripheral/points0%d.png",thisZ);
        //} else {
            sprintf(fileout, "peripheral/points%d.png",thisZ);
        //}
	

	//TCanvas *c1 = new TCanvas();
        TGraph *gr = new TGraph(goodParticles,particlesX,particlesY);
        gr->SetTitle(title);
        gr->GetXaxis()->SetTitle("x (mm)");
        gr->GetYaxis()->SetTitle("y (mm)");
        gr->SetMarkerStyle(6);
        gr->SetMarkerColor(4);
	gr->Draw("AP");
	c1->Print(fileout);
	//c1->SaveAs(fileout);
        //gr->SaveAs(fileout,"AP");
    }
}
