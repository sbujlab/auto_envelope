#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "TMinuit.h"
#include "TFitter.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TROOT.h"
//using namespace std;

double pi = 3.14159265358979323846;

//make this a user supplied variable
const int maxV = 20; // maximum number of vertices allowed
int numVertices; // change this to change the number of vertices
int goodParticles = 0;

double* particlesX;
double* particlesY;

double verticesX[maxV];
double verticesY[maxV];

int countVertices(int thisZ,int& maxZ) {
    int temp = 0;
    int cont = 1;
    int take = 0;
    char filename[100];
    sprintf(filename,"vertex_storage.txt");
    ifstream particleData(filename,ios::in);
    double xspot;
    double yspot;
    particleData >> xspot >> yspot;
    while (cont == 1) {
        if ((xspot == -999999.) && (yspot == thisZ)) {
	    temp = 0;
	    take = 1;
	    std::cout << "Starting point z position noted, z= " << thisZ << std::endl;
        }
	if (particleData.eof()) {
	    cont = 0;
	    return temp;
	}
	if (!particleData.eof()) {
	    particleData >> xspot >> yspot;
	    if ((xspot == -999999.) && (take == 1)){
                maxZ = yspot;//updates the zMax parameter
	        cont = 0;
		std::cout << "Final point z position noted, z= " << maxZ << std::endl;
	        return temp;
	    }
	    if (take == 1){
		std::cout << "vertices read, x= " << xspot << ", y= " << yspot << std::endl; 
	        verticesX[temp] = xspot;
	        verticesY[temp] = yspot;
	        temp++;
            }
	}
    }
    return temp;
}

int countTextFile(int thisZ) {//counts good particles
    
    int tempGoodParticles;

    char filename[100];
    sprintf(filename,"peripheral/points%i.txt",thisZ);
    ifstream particleData(filename,ios::in);
    int burn1;
    particleData >> burn1;

    if (burn1 != thisZ) {
	printf("mismatched z position and file name");
    }
    
    std::string line;
    while (std::getline(particleData , line)) {
          tempGoodParticles++;
    }
    return tempGoodParticles;

}





// Returns angle between lines connecting (origin, (x1, y1)) and (origin, (x2, y2))
double findAngle(double x1, double y1, double x2, double y2) {
    double dtheta, theta1, theta2;
    theta1 = atan2(y1,x1);
    theta2 = atan2(y2,x2);
    dtheta = theta2 - theta1;
    while (dtheta > pi) {
        dtheta -= 2.*pi;
    }
    while (dtheta < -pi) {
        dtheta += 2.*pi;
    }
    return dtheta;
}

double findAngle2(double x1, double y1, double x2, double y2) {
    double dtheta, theta1, theta2;
    theta1 = atan2(y2,x2);
    theta2 = atan2(y1,x1);
    dtheta = theta2 - theta1;
    if (dtheta < 0.) {
        dtheta = (2*pi)+dtheta;
    }
    return dtheta;
}

double get_max(double d1, double d2) {

	if (d1 - d2 > 0) {
		return d1; 
	} else {
		return d2;
	}
}

// Function to be minimized
// Paramters are the vertices of the polygon, such that:
// par[0] = v1_x
// par[1] = v1_y
// par[2] = v2_x
// par[3] = v2_y
// etc...

void figureOfMerit(int &nDim, double *gout, double &result, double par[], int flg) {
    
    //assert(!particlesX || !particlesY);
    
    // Calculates area of the polygon using the shoelace algorithm
    double sum = 0.;
    double A = 0.;
    double polygonAngles_good = (numVertices-2)*pi;
    double polygonAngles_actual = 0;
    double nonSimplePolygon = 0;
    int numOutside = 0;
    
    for(int n = 0; n<2*numVertices; n+=2) {
        sum += par[n]*par[(n+3)%(2*numVertices)]-par[(n+1)%(2*numVertices)]*par[(n+2)%(2*numVertices)];
    }
    A = 0.5*fabs(sum);
    double sumOfAngles = 0.;
    double sum_d2 = 0.;
    // sumOfAngles      sum of angles from particle to every adjacent pair of polygon vertices
    //                  sumOfAngles = 2pi if particle is inside polygon
    //                  sumOfAngles = 0 if particle is outside polygon
    // sum_d2           sum of squared distances from exterior particles to polygon
    // rInv		sum 1/r_(i,i+1), where r_(i,j) is distance from vertex i to vertex j
    double px, py, dVertex, min_dVertex, dEdge, theta_m, theta_p;
    // px,py        x,y coordinates of particle
    // dVertex      distance from particle to polygon vertex
    // min_dVertex  stores minimum dVertex
    // dEdge        distance from particle to edge of polygon
    // theta_m      angle between particle, closest vertex v, and vertex v-1
    // theta_p      angle between particle, closest vertex v, and vertex v+1
    
    int v, vm, vp;
    // v        index of vertex closest to particle
    // vm       v-1
    // vp       v+1
    
    // Loop through each particle in envelope
    for (int i = 0; i<goodParticles; i++) {
        
        px = particlesX[i];
        py = particlesY[i];
        
        min_dVertex = 1e9;
        
        sumOfAngles = 0.0;
        // Loop through each vertex in the polygon
        for (int j=0; j<2*numVertices; j+=2) {
            
            sumOfAngles += findAngle(par[j]-px, par[j+1]-py,par[(j+2)%(2*numVertices)]-px,par[(j+3)%(2*numVertices)]-py);
            
            dVertex = sqrt((px-par[j])*(px-par[j])+(py-par[j+1])*(py-par[j+1]));
            //printf("Particle %i : dVertex = %f\n",i,dVertex);
            if (dVertex < min_dVertex) {
                min_dVertex = dVertex;
                v = j;
            }
            
        }
        if (fabs(sumOfAngles) < pi) {
            //printf("Particle %i is outside!",i);
            numOutside++;
	    vp = (v+2)%(2*numVertices);
            if (v-2 >= 0) {
                vm = v - 2;
            } else {
                vm = 2*numVertices;
            }
            
            theta_m = fabs(findAngle(px-par[v],py-par[v+1],par[vm]-par[v],par[vm+1]-par[v+1]));  // Calculates angle (p, v, v-1)
            theta_p = fabs(findAngle(px-par[v],py-par[v+1],par[vp]-par[v],par[vp+1]-par[v+1]));  // Calculates angle (p, v, v+1)
            
            dEdge = -1e9;
            
            // Checks which unique polygon edge can be connected to the particle with a line perpendicular to that edge,
            // and calculates the particle's distance from that edge.  If such an edge doesn't exist, or is not unique,
            // the particle's distance from the polygon is set equal to the min_dVertex (previously calculated)
            if (theta_m <= pi/2. && theta_p > pi/2.) {
                dEdge = min_dVertex*sin(theta_m);
            } else if (theta_m > pi/2. && theta_p <= pi/2.) {
                dEdge = min_dVertex*sin(theta_p);
            } else if (theta_m <= pi/2. && theta_p <= pi/2.) {
                if( theta_m < theta_p ){
                    dEdge = min_dVertex*sin(theta_m);
                } else {
                    dEdge = min_dVertex*sin(theta_p);
                }
            } else {
                dEdge = min_dVertex;
            }
            //printf("min_dVertex = %f and theta_m = %f and theta_p = %f\n",min_dVertex, theta_m,theta_p);
            //printf("The distance from the edge is %f\n",dEdge);
            
            // if the particles are too far away just ignore them
            if (dEdge < 60) {
                sum_d2 += dEdge*dEdge;
            }
	}
        
    }
    
    for (int j=0; j<2*numVertices; j+=2) {
        
        polygonAngles_actual += findAngle2(     par[j]-par[(j+2)%(2*numVertices)],
                                           par[(j+1)%(2*numVertices)]-par[(j+3)%(2*numVertices)],
                                           par[(j+4)%(2*numVertices)]-par[(j+2)%(2*numVertices)],
                                           par[(j+5)%(2*numVertices)]-par[(j+3)%(2*numVertices)]);
        
        nonSimplePolygon = polygonAngles_actual - polygonAngles_good;
    }

    //result = A + 500.*sum_d2/(sqrt((double)goodParticles)*0.5) + 1e9*nonSimplePolygon*nonSimplePolygon; // Returns the figure of merit
    result = 100.*numOutside + 1.00*A + 100.*sum_d2/(sqrt((double)goodParticles)*0.5) + 1e9*nonSimplePolygon*nonSimplePolygon; // Returns the figure of merit
//    cout << 1.*numOutside << "\t \t Number Outside polygon" << endl;
//    cout << 1.*A << "\t \t Area" << endl;
//    cout << 500.*sum_d2/(sqrt((double)goodParticles)*0.5) << "\t \t Fit quality" << endl;
//    cout << 1e9*nonSimplePolygon*nonSimplePolygon << "\t \t Angles parameter" << endl;
}


void envelopeFit(int zVal, int zMin) {
   
    TCanvas* c1 = new TCanvas;

    goodParticles = countTextFile(zVal); 
   
    particlesX = new double[goodParticles];
    particlesY = new double[goodParticles];

    char filename[100];
    sprintf(filename,"peripheral/points%i.txt",zVal);
    ifstream particleData;
    particleData.open(filename,ios::in);
    if (particleData.fail()) {
        std::cout << "File: " << filename << "failed to open\n";
        return;
    } 

    
    int burn3;
    particleData >> burn3;

    if (burn3 != zVal) {
	printf("mismatched z position and file name");
    }
    
    for (int i = 0; i<goodParticles; i++) {
        particleData >> particlesX[i] >> particlesY[i];
    }   
    
    TFitter *minimizer = new TFitter(numVertices*2);
     
    {
	double p1 = -1;
	minimizer->ExecuteCommand("SET PRINTOUT",&p1, 1);
    }

    minimizer->SetFCN(figureOfMerit);
    
    double deltaP = 100.;

    // Minimizer parameters concerning vertex positions: update this to match number of vertices when changed.


    minimizer->SetParameter(1,"y1",verticesY[0],0,0,0);
    for (int h = 0; h<numVertices; h++) {
        char namex[32];
        char namey[32];
        sprintf(namex,"y%i",h+1);
        sprintf(namey,"y%i",h+1);
	minimizer->SetParameter(2*h,namex,verticesX[h],1,verticesX[h]-deltaP,verticesX[h]+deltaP);
	if ((h!=0)&&(h!=numVertices-1)) {
	    minimizer->SetParameter(2*h+1,namey,verticesY[h],1,get_max(0.,verticesY[h]-deltaP),verticesY[h]+deltaP);
	}
	if (h==0) {
	    minimizer->SetParameter(2*h+1,namey,verticesY[h],0,0,0);
	}
	if (h==numVertices-1){
	    minimizer->SetParameter(2*h+1,namey,verticesY[h],0,0,0);
	}
    }

    for (int n = 0; n<numVertices; n++) {
	printf("xi = %.2f \t yi = %.2f \n",verticesX[n],verticesY[n]); 
    }
    
    //minimizer->ExecuteCommand("SIMPLEX",0,0);
    //minimizer->ExecuteCommand("SCAN",0,0);
    
    minimizer->ExecuteCommand("MINIMIZE",0,0);
    minimizer->ExecuteCommand("MIGRAD",0,0);
    //minimizer->ExecuteCommand("MIGRAD",0,0);
    
    ofstream outFile2;
    char filenameB[50];
    sprintf(filenameB,"text/envelope_output_%ipts_%i.txt",numVertices,zMin);
    
    outFile2.open (filenameB,ios::app);
    outFile2 << zVal << " \n";
    
    for (int n = 0; n<numVertices; n++) {
        verticesX[n] = minimizer->GetParameter(2*n);
        verticesY[n] = minimizer->GetParameter(2*n+1);
	printf("xf = %.2f \t yf = %.2f \n",verticesX[n],verticesY[n]);
	outFile2 << verticesX[n] << "     \t" << verticesY[n] << " \n";
    }
    outFile2.close();    
    
    
    //TGraph *initialPolygonGraph = new TGraph(numVertices, xInitial,yInitial);
    TGraph *particleGraph = new TGraph(goodParticles,particlesX,particlesY);
    TGraph *polygonGraph = new TGraph(numVertices,verticesX,verticesY);
    
    //initialPolygonGraph->SetMarkerStyle(34);
    //initialPolygonGraph->SetMarkerColor(8);
    
    particleGraph->SetMarkerStyle(6);
    particleGraph->SetMarkerColor(4);
    
    polygonGraph->SetMarkerStyle(8);
    polygonGraph->SetMarkerColor(2);
    polygonGraph->SetLineColor(2);
    polygonGraph->SetTitle("");    
    polygonGraph->Draw("APL");
    particleGraph->Draw("P");
    //initialPolygonGraph->Draw("P");


    char filename1[50];
    sprintf(filename1, "plot/z%.2lfm_fit.pdf",zVal/1000.);
    char filename2[50];
    sprintf(filename2, "plot/z%.2lfm_fit.png",zVal/1000.);
    c1->SaveAs(filename1);
    c1->SaveAs(filename2);
    minimizer->Delete();
    particleGraph->Delete();
    polygonGraph->Delete();

    delete particlesX;
    delete particlesY;
    
    particlesX = NULL;
    particlesY = NULL;
}

void envelopeFitLoop_batch(int zMin = 0, int zStep = 0, int zMax = 0) {
    gROOT->SetBatch();
    numVertices = countVertices(zMin,zMax);
    if ( (numVertices < 3) || (numVertices>maxV) ) {
	std::cout << "Error, this requires at least three vertices, and at max " << maxV << " vertices\n";
	return;
    }
    int z = zMin;
    while (z < zMax) {
	envelopeFit(z,zMin);
        z+=zStep;
    }
    envelopeFit(zMax,zMin);
}
