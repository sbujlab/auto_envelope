#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "TGraph.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TPad.h"
#include "TBenchmark.h"
#include "TExec.h"

int z_pos = 0; //default starting position is z=0
int i = 0;
TGraph* g2;
const int maxv = 20;
double xVertices[maxv];
double yVertices[maxv];

using namespace std;	
int countTextFile(int thisZ) {
	int tempGoodParticles = 0;

	char filename[100];
	sprintf(filename,"peripheral/points%i.txt",thisZ);
	ifstream particleData3(filename,ios::in);
	int burn1;
	particleData3 >> burn1;

	if (burn1 != thisZ) {
		printf("mismatched z position and file name");
		return 0;
	}
	std::string line;
	while (std::getline(particleData3, line)) {
		tempGoodParticles++;
	}
	return tempGoodParticles;	
}



void readTextFile(int thisZ, int tempGoodParticles, double* particlesX, double* particlesY){
	char filename[100];
	sprintf(filename,"peripheral/points%i.txt",thisZ);	
	ifstream particleData2(filename,ios::in);
	int burn1;
	particleData2 >> burn1;

	if (burn1 != thisZ) {
		printf("mismatched z position and file name");
		return;
	}
	
	for (int j = 0; j<tempGoodParticles; j++) {
		particleData2 >> particlesX[j] >> particlesY[j];
	}
}

void DynamicCoordinates() {
	gPad->GetCanvas()->FeedbackMode(kTRUE);
	if (!gPad) {
		cout << "gPad is null" << endl;
		return;
	}	
	int event = 0;
	event = gPad->GetEvent();
	
	if (( i == maxv ) || ( event == 24 )){ //keypresses trigger exit
		if (i == maxv) {
			cout << "Too many points (keep below " << maxv << ")\n";      
		}
		return;
	}
	else if (event == 1) {
		double px = gPad->GetEventX();
		double py = gPad->GetEventY();
		double xd = gPad->AbsPixeltoX(px);
		double yd = gPad->AbsPixeltoY(py);
		double x = gPad->PadtoX(xd);
		double y = gPad->PadtoY(yd);
		g2->SetPoint(i,x,y);
		g2->Draw("+L");
		xVertices[i] = x;
		yVertices[i] = y;
		//outfile << x << "\t" << y << endl;
		i = i + 1;
		cout << i << " vertices collected, stop with keypress (avoid double clicks)\n\n";
		cout << "Selected: " << x << "\t" << y << "\n";
	}
}

void reader(){
	gROOT->SetBatch(kFALSE);
	
	double* particlesX;
	double* particlesY;
	int goodParticles = 0;
	char ans = 'y'; //default answer is yes
	TPad* pad1;
	
	while (ans=='y'){
		gROOT->Reset();
		i = 0;	
		g2 = new TGraph();
		cout << "\nWould you like to choose vertex points? \t y/n: ";
		cin >> ans;
	  		

		if (ans=='n'){
			cout << "\nFinally: Would you like to specify one last z position to append to vertex_storage.txt? \t y/n: ";
			char ans4 = 'y';
			cin >> ans4;
			if (ans4=='y'){
				char ans5[64];
				cout << "\nPlease input final z position to add to file: \t";
				cin >> ans5;
				char filenameO[50];
				ofstream outfile2;
				sprintf(filenameO, "vertex_storage.txt");
				outfile2.open(filenameO,ios::app);
				outfile2 << "-999999\t" << ans5 << "\n";
				outfile2.close();
				cout << "\nEnd of Line\n";
			}
			else {
				cout << "Not adding any final z position to file. End Of Line.\n";
			}
			return;
		}
		else if (ans=='y'){
			cout << "What z position would you like to examine?" << endl;
			cin >> z_pos;
			
			TCanvas* c2 = new TCanvas("c2","points display", 10, 10, 1600, 900);
			c2->ToggleEventStatus();
			pad1 = new TPad("pad1","pad1",.1,.1,.9,.9);
			pad1->Draw();
			goodParticles = countTextFile(z_pos);
			particlesX = new double[goodParticles];
			particlesY = new double[goodParticles];
			readTextFile(z_pos, goodParticles, particlesX, particlesY);
		
			cout << "Number of points displayed: " << goodParticles << "\n";
			if (goodParticles >= 1){
				TGraph *particleGraph = new TGraph(goodParticles,particlesX,particlesY);
				particleGraph->SetMarkerStyle(6);
				particleGraph->SetMarkerColor(4);
				particleGraph->Draw("AP");
				gPad->Modified();
				gPad->Update();
				// do user interactions here:
				if (!gPad) {
					cout << "pad1 is null" << endl;
					break;
				}	
				char command[50];
				sprintf(command,"DynamicCoordinates()");
				gPad->AddExec("ex1",command);
				gPad->WaitPrimitive();
				gPad->WaitPrimitive();
				gPad->DeleteExec(command);

				char ans2 = 'y';
				while (ans2=='y') {
					cout << "Do you want to save these " << i << " points?\ty/n: ";
					cin >> ans2;
					if (ans2=='n') {
						cout << "Deleting " << i << " x and y coordinates \n";
					}
					if (ans2=='y'){
						char filenameO[50];
						ofstream outfile;
						sprintf(filenameO, "vertex_storage.txt");
						outfile.open(filenameO,ios::app);
						outfile << "-999999\t" << z_pos << "\n";
						for (int k = 0; k < i; k++) {
							outfile << xVertices[k] << "\t" << yVertices[k] << endl;
						}	
						cout << "Stored " << i << " x and y coordinates at z = " << z_pos << endl;
					
						outfile.close();
						ans2 = 'n';
					}
					else if (( ans2 != 'y' ) && ( ans2 != 'n' )) {
						cout << "Error, please try again" << endl;
						ans2 = 'y';
					}
				}
				// DONE
				particleGraph->Delete();
			cout << "Done with z = " << z_pos << endl;
			}

			else {
				cout << "Sorry, the file you have chosen is empty, please try again" << endl;
			}
			delete particlesX;
			delete particlesY;
			//gPad->Delete();
			//pad2->Delete();
		//	c2->Delete();
			particlesX = NULL;
			particlesY = NULL;
		}
		else if (( ans != 'y' ) && ( ans != 'n' )) {
			cout << "Error, please try again using the characters 'y' and 'n'" << endl;
			ans = 'y';
		}
	}
}
