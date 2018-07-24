#include <stdio.h>
#include <sstream>
#include <vector>
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TTree.h"
#include "TFile.h"
#include "TImage.h"
#include <limits>
#include "remolltypes.hh"

int z_pos = 0; //default starting position is z=0
//int i = 0;
TGraph* g2;
TMultiGraph *mg = new TMultiGraph();
TGraph* hull;
const int maxv = 20;
double minArea; //minimum area unit considered. smaller = less sensitive
double criticalPoints; //hits treated as zero. smaller = more sensitive


double xVertices[maxv];
double yVertices[maxv];

double* particlesX;
double* particlesY;

int goodParticles;

int countTextFile(int thisZ, char* filename) {
    int tempGoodParticles = 0;
    std::vector < remollEventParticle_t > *particle = 0;
    TFile f(filename);
    TTree *tree = (TTree*)f.Get("T");
    tree->Print();
    tree->SetBranchAddress("part", &particle); 
    for (int i = 0; i < tree->GetEntries(); i++)
    {
        tree->GetEntry(i);
        for (size_t j = 0; j < particle->size(); j++)
        {
            std::vector < double > *z = &(particle->at(j).tjz);
            for (size_t k = 0; k < z->size(); k++)
            {
                if (z->at(k) == thisZ)
                {
                    tempGoodParticles++;
                    break;
                }
            }
        }
    }	
    return tempGoodParticles;	
}



void readTextFile(int thisZ, char* filename){
    int tempGoodParticles = 0;
    std::vector < remollEventParticle_t > *particle = 0;
    TFile f(filename);
    TTree *tree = (TTree*)f.Get("T");
    tree->SetBranchAddress("part", &particle); 
    for (int i = 0; i < tree->GetEntries(); i++)
    {
        tree->GetEntry(i);
        for (size_t j = 0; j < particle->size(); j++)
        {
            std::vector < double > *x = &particle->at(j).tjx;
            std::vector < double > *y = &particle->at(j).tjy;
            std::vector < double > *z = &particle->at(j).tjz; 
            std::cout << "check size x " << x->size() << " " << __LINE__ << std::endl;
            std::cout << "check size y " << y->size() << " " << __LINE__ << std::endl;
            std::cout << "check size z " << z->size() << " " << __LINE__ << std::endl;
            for (size_t k = 0; k < z->size(); k++)
            {
                if (z->at(k) == thisZ)
                {
                    particlesX[tempGoodParticles] = x->at(k);
                    particlesY[tempGoodParticles++] = y->at(k);
                    break;
                }
            }
        }
    }
}


int countPointsInBounds(double rightBound, double leftBound, double upperBound, double lowerBound)
{
    int count = 0;
    for (unsigned int i = 0; i < goodParticles; i++)
    {
        if (particlesX[i] < rightBound && particlesX[i] > leftBound && particlesY[i] < upperBound && particlesY[i] > lowerBound)
            count++;
        //std::cout << "X: " << particlesX[i] << std::endl << "Y: :" << particlesY[i] << std::endl;
    }
    return count;
}

void checkQuadrant(double rightBound, double leftBound, double upperBound, double lowerBound)
{
    double xDelta = rightBound - leftBound;
    double yDelta = upperBound - lowerBound;
    double area = xDelta * yDelta;
	
	//std::cout << "points: " << countPointsInBounds(rightBound, leftBound, upperBound, lowerBound) << std::endl;
	//std::cout << "area:" << area << std::endl;
	//std::cout << "right:" << rightBound << std::endl;
	//std::cout << "left:" << leftBound << std::endl;
	//std::cout << "upper:" << upperBound << std::endl;
	//std::cout << "lower:" << lowerBound << std::endl;
	if (area <= minArea)
	{
		return;
	}
	if (countPointsInBounds(rightBound, leftBound, upperBound, lowerBound)/area <= criticalPoints)
	{
		//space is not in the envelope
        std::cout << "drawing" << std::endl;
		TGraph *g3 = new TGraph();
		g3->SetPoint(0, rightBound, upperBound);
		g3->SetPoint(1, rightBound, lowerBound);
		g3->SetPoint(2, leftBound, lowerBound);
		g3->SetPoint(3, leftBound, upperBound);
		g3->SetFillColor(rand() % 100 + 1);
		mg->Add(g3);
		if (area <= 4*minArea)
		{
            std::cout << "point" << std::endl;
			g2->SetPoint(g2->GetN(), rightBound - (xDelta/2), upperBound - (yDelta/2));
			return;
		}
	}
	else //enough points to not be noise
	{ 	//but where are those points?
        std::cout << "recur" << std::endl;
		checkQuadrant(rightBound - (xDelta/2), leftBound, upperBound - (yDelta/2), lowerBound); //bot  left	
		checkQuadrant(rightBound, rightBound - (xDelta/2), upperBound - (yDelta/2), lowerBound); //bot right	
		checkQuadrant(rightBound - (xDelta/2), leftBound, upperBound, upperBound - (yDelta/2)); //top left	
		checkQuadrant(rightBound, rightBound - (xDelta/2), upperBound, upperBound - (yDelta/2)); //top right	
        	
	}
}

bool isCounterClockwise(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y )
{
    return ((p2x - p1x)*(p3y-p1y) - (p2y-p1y)*(p3x-p1x)) < 0;
}

int countPointsUnderLine(double sX, double sY, double eX, double eY)
{
	int count = 0;

    int sign = -1;
    if (sX > eX)
    {
        sign *= -1;   
        double tX = sX, tY = sY;
        sX = eX; sY = eY;
        eX = tX; eY = tY;
    }

    double slope = ((eY-sY) / (eX-sX));
    

	for (unsigned int i = 0; i < goodParticles; i++)
	{
		if (particlesX[i] >= sX  && particlesX[i] <= eX && particlesY[i] <= (sY + slope*(particlesX[i]-sX)))
			count++;
	//std::cout << "X: " << particlesX[i] << endl << "Y: :" << particlesY[i] << endl;
	}
	return sign*count;
}

double getPointsEnclosed(TGraph* graph)
{
    double points = 0;
    for (int i = 0; i < graph->GetN()-1; i++)
    {
        double sX, sY, eX, eY;
        graph->GetPoint(i, sX, sY);
        graph->GetPoint(i+1, eX, eY);

        //stats[0] += (eX - sX) *((sY + eY)/2);//trapezoidal area      
        //stats[1] 
            points += countPointsUnderLine(sX, sY, eX, eY); //count
    }
    return points;
}
double distX, distY;
Bool_t distanceComparator(const TGraph* gr, Int_t i, Int_t j)
{
    double fX, fY, sX, sY;
    gr->GetPoint(i, fX, fY);
    gr->GetPoint(j, sX, sY);
    return sqrt(pow(distX-fX, 2) + pow(distY - fY, 2)) > sqrt(pow(distX-sX, 2) + pow(distY - sY, 2)); 
}


TGraph* getNearestPoints(TGraph* data, double cX, double cY, int k)
{
    distX = cX;
    distY = cY;
    k = (k < data->GetN())? k : data->GetN();
    TGraph* nearest = new TGraph(*data);
    nearest->Sort(&distanceComparator);
    
    while (nearest->GetN() > k)
    {
        nearest->RemovePoint(nearest->GetN()-1);
    }
    return nearest;
}


bool IntersectsQ(TGraph* g1, int pid1, TGraph* g2, int pid2, TGraph* g3, int pid3, TGraph* g4, int pid4)
{
    double p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y;
    g1->GetPoint(pid1, p1x, p1y);
    g2->GetPoint(pid2, p2x, p2y);
    g3->GetPoint(pid3, p3x, p3y);
    g4->GetPoint(pid4, p4x, p4y);     
  

    double s1x = p2x-p1x; double s1y = p2y-p1y;
    double s2x = p4x-p3x; double s2y = p4y-p3y;

    
    double snum = (-s1y * (p1x - p3x) + s1x * (p1y - p3y));
    double tnum = (s2x * (p1y - p3y) - s2y * (p1x - p3x));
    double den = (-s2x * s1y + s1x * s2y);

    
    //FIXME
    double s = snum/den;
    double t = tnum/den;
    double ix, iy;
        
    /*std::cout << "Collision?"<< std::endl;
    std::cout << "p1x: " << p1x <<std::endl;
    std::cout << "p1y: " << p1y <<std::endl;
    std::cout << "p2x: " << p2x <<std::endl;
    std::cout << "p2y: " << p2y <<std::endl;
    std::cout << "p3x: " << p3x <<std::endl;
    std::cout << "p3y: " << p3y <<std::endl;
    std::cout << "p4x: " << p4x <<std::endl;
    std::cout << "p4y: " << p4y <<std::endl;
    
    
    std::cout << "snum: " << snum << std::endl;
    std::cout << "s: " << s << std::endl;
    std::cout << "tnum: " << tnum << std::endl;
    std::cout << "t: " << t << std::endl;
    std::cout << "den: " << den << std::endl;
    */
    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        //Collision detected
        ix = p1x + (t * s1x);
        iy = p1y + (t * s1y);
        /*std::cout << "Collision found at " << std::endl;
        std::cout << "ix: " << ix << std::endl;
        std::cout << "iy: " << iy << std::endl;
        */
        //if (t == 0) return false; //line segments are allow to connect to form a hull
        return true;
    }
    return false;
    /*
    double ax = p2x - p1x;     // direction of line a
    double ay = p2y - p1y;     // ax and ay as above
    double bx = p3x - p4x;     // direction of line b, reversed
    double by = p3y - p4y;     // really -bx and -by as above
    double dx = p3x - p1x;   // right-hand side
    double dy = p3y - p1y;
    double det = ax * by - ay * bx;
    std::cout << "Intersect- " << (dx * by - dy * bx) << std::endl;
    std::cout << "Intersect2- " << (ax * dy - ay * dx) << std::endl;
    std::cout << "Intersect det- " << det << std::endl;
    if (det == 0) return false;
    double r = (dx * by - dy * bx) / det;
    double s = (ax * dy - ay * dx) / det;
    std::cout << "Intersect r- " << r << std::endl;
    std::cout << "Intersect s- " << s << std::endl;
    return !(r <= 0 || r > 1 || s < 0 || s > 1);
*/
}

const double pi = 3.1415926535897;
double getAngle(double x1, double y1, double x2, double y2)
{
    double res = - atan2(y2, x2) + atan2(y1, x1);
    return ((res < 0) ? res + 2*pi : res);
}

double cX, cY;
double prevX, prevY;
double getAngle(int x, int y)
{
    return getAngle(prevX-cX, prevY-cY, x-cX, y-cY);
    //return getAngle(x1, y1, x2, y2);
}


Bool_t AngleComparator(const TGraph* gr, Int_t i, Int_t j)
{
    double tX, tY, nX, nY;
    gr->GetPoint(i, tX, tY);
    gr->GetPoint(j, nX, nY);
    return getAngle(prevX-cX, prevY-cY, tX-cX, tY-cY) > getAngle(prevX-cX, prevY-cY, nX-cX, nY-cY);   
    //return getAngle(prevX, prevY, tX, tY) > getAngle(prevX, prevY, nX, nY);   
}

void RemovePoint(TGraph* data, double cX, double cY)
{
    for (int i = 0; i < data->GetN(); i++)
    {
        double tX, tY;
        data->GetPoint(i, tX, tY);
        if (cX == tX && cY == tY)
        {
            data->RemovePoint(i);
            return;
        }
    }
    std::cerr << "Unable to find point placed?" << std::endl;
    exit(1);
}

TGraph* orderPoints(TGraph* pointList, int k)
{
    std::cout << "Ordering with k=" << k <<  std::endl;
    TGraph *kNearestPoints;
    hull = new TGraph();
    TGraph *data = new TGraph(*pointList);
    k = (k< 3)? 3 : k; //k must be >= 3
    if (data->GetN() < 3) return hull;
    if (data->GetN() == 3) {hull = data; return hull;}

    if (k > data->GetN()) return hull;
    //k = (k>data->GetN()-1)?data->GetN()-1 : k;

    //first point is the lowest
    data->Sort(TGraph::CompareY);
    double tX, tY;
    data->GetPoint(0,  tX, tY);
    hull->SetPoint(0, tX, tY);
    data->RemovePoint(0);
    prevX = tX-1; prevY = tY; 
    cX = tX; cY = tY;
    double step = 2;


    while(((cX != tX || cY != tY) || step ==2) && (data->GetN() > 0))
    {
        if (step == 5) //add the first point back in to be able to close the hull
        {
            data->SetPoint(data->GetN(), tX, tY);
        }
        
        //std::cout << "check 1" << std::endl;
        //get and sort neighbors descending by right hand turn
        kNearestPoints = getNearestPoints(data, cX, cY, k);
        //std::cout << "check 2 " << kNearestPoints->GetN() << std::endl;
        kNearestPoints->Sort(&AngleComparator, kFALSE); 

        /*if (step >= 500) //FIXME
        {
            //kNearestPoints->Draw("AL");
            
            for (int it = 0; it < kNearestPoints->GetN(); it++)
            {
                double bX, bY;
                kNearestPoints->GetPoint(it, bX, bY);
                std::cout << "bp " << it << ": " << bX << ", " << bY << std::endl;
                std::cout << "@ " << getAngle(bX, bY) << std::endl;
                  
            }
                TGraph * test = new TGraph();
                test->SetPoint(0, -1, 0);
                test->SetPoint(1, 1, 0);

                test->SetPoint(2, 1, 0);
                test->SetPoint(3, 2, 0);
                
                std::cout << "Intersect Test" << IntersectsQ(test, 2, test, 3, test, 0, test, 1) << std::endl;
                std::cout << "angle test" << getAngle(0, 1, -1, 0) << std::endl;
                  std::cout << "angle test" << getAngle(0, 1, 0, 1) << std::endl;
                  std::cout << "angle test" << getAngle(0, 1, 1, 0) << std::endl;
                  std::cout << "angle test" << getAngle(0, 1, 0, -1) << std::endl;
                    
                return hull; 
        }*/

        bool intersects = true;
        int i = 0;;
        for (i = 0; i < kNearestPoints->GetN(); i++)
        {
            double pX, pY;
            kNearestPoints->GetPoint(i, pX, pY);
            //std::cout << pX << ", " << pY << "?" << std::endl;
            int lastPoint;
            if (pX == tX && pY == tY)
                lastPoint = 1;
            else
                lastPoint = 0;

            int j = 3;
            intersects = false;
            while (!intersects && j <= hull->GetN() - lastPoint)
            {
                /*std::cout << "info " << hull->GetN() << std::endl;
                std::cout << "hull " << step-2 << std::endl;
                std::cout << "kNear " << i << std::endl;
                std::cout << "hull " << step-1-j << std::endl;
                std::cout << "hull " << step-j << std::endl;
                */intersects = IntersectsQ(hull, step-2, kNearestPoints, i, hull, step-1-j, hull, step-j);
                //std::cout << "Intersects: " << intersects << std::endl;
                j++;
            }
            if (!intersects)
                break;
        }
        if (intersects){ //all intersections. Must increase k
            std::cout << "Too many intersections." << std::endl;   
            //return hull ; //FIXME
            return orderPoints(pointList, k+1);
        }
        kNearestPoints->GetPoint(i, cX, cY);
        //std::cout << "check" << cX << " " << cY << " " << tX << " " << tY << std::endl;
        hull->SetPoint(hull->GetN(), cX, cY);
        
        //prevAngle = getAngle(hull, step, step-1);
        hull->GetPoint(hull->GetN()-2, prevX, prevY); 
        
        RemovePoint(data, cX, cY);
        step++;
    }
    //std::cout << "check 3" << std::endl;
    //FIXME 95% of points?
    bool allInside = !(abs(getPointsEnclosed(hull)) <= (0.94 * goodParticles));
    std::cout << getPointsEnclosed(hull) << "/" << goodParticles << std::endl;
    if (!allInside){
        std::cout << "Enclose more points" << std::endl;
        return orderPoints(pointList, k+1);
    }
    return hull;
}

/*void DynamicCoordinates() {
	gPad->GetCanvas()->FeedbackMode(kTRUE);
	if (!gPad) {
    std::cout << "gPad is null" << endl;
		return;
	}	
	//checkQuadrant(0, -1500, 1000, 0);
	//orderPoints();
    std::cout << "finished" << endl;
	int event = 0;
	event = gPad->GetEvent();
	
	if (( i == maxv ) || ( event == 24 )){ //keypresses trigger exit
		if (i == maxv) {
        std::cout << "Too many points (keep below " << maxv << ")\n";      
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
		//g2->SetPoint(i,x,y);
		//g2->Draw("+L");
		xVertices[i] = x;
		yVertices[i] = y;
		//outfile << x << "\t" << y << endl;
		i = i + 1;
        std::cout << i << " vertices collected, stop with keypress (avoid double clicks)\n\n";
        std::cout << "Selected: " << x << "\t" << y << "\n";
	}
}
*/
int main(int argc, char **argv){
	//gROOT->SetBatch(kFALSE);
    if (argc < 2)
    {
        std::cerr << "Usage: ./reader file_name" << std::endl;
        exit(0);
    }

    char* fileName = argv[1];

	goodParticles = 0;
	char ans = 'y'; //default answer is yes
	TPad* pad1;
	
	while (ans=='y')
    {
		//gROOT->Reset();
		//i = 0;	
		g2 = new TGraph();
		g2->SetFillColor(6);
        std::cout << "\nWould you like to choose vertex points? \t y/n: ";
        std::cin >> ans;
	  		

		if (ans=='y')
        {
            std::cout << "What z position would you like to examine?" << std::endl;
            std::cin >> z_pos;
			
			TCanvas* c2 = new TCanvas("c2","points display", 10, 10, 1600, 900);
			c2->ToggleEventStatus();
			pad1 = new TPad("pad1","pad1",.1,.1,.9,.9);
			//pad1->Draw();
			goodParticles = countTextFile(z_pos, fileName);
			particlesX = new double[goodParticles];
			particlesY = new double[goodParticles];
			readTextFile(z_pos, fileName);
		
            std::cout << "Number of points displayed: " << goodParticles << "\n";
			if (goodParticles >= 1){
                TGraph *particleGraph = new TGraph(goodParticles,particlesX,particlesY);
				gPad->Modified();
				gPad->Update();
				// do user interactions here:
				if (!gPad) {
                    std::cout << "pad1 is null" << std::endl;
					//break;
				}	
				//g2->SetPoint(g2->GetN(), -9999, 0);
				double xMax, xMin, yMax, yMin;
                particleGraph->ComputeRange(xMin, yMin, xMax, yMax);

                double area = (xMax - xMin * yMax - yMin);
                minArea = area/100000;
                criticalPoints = (goodParticles/area) / 100; //% of the particles/area
                checkQuadrant(xMax, xMin, yMax, yMin);
				//hull = orderPoints(particleGraph, 50);
				hull = orderPoints(g2, 30);
                //hull = new TGraph();
                
                std::cout << "Hull points: " << hull->GetN() << std::endl;
                for (int j = 0; j < hull->GetN(); j++)
                {
                    double x, y;
                    hull->GetPoint(j, x, y);
                    //std::cout << x << ", " << y << std::endl;
                }
				particleGraph->SetMarkerStyle(6);
				particleGraph->SetMarkerColor(4);
                g2->SetMarkerStyle(6);
                g2->SetMarkerColor(6);
                hull->SetLineColor(2);
				
                //FIXME
                hull->Draw("AL");
                particleGraph->Draw("AP");
                //g2->Draw("Psame");
				//particleGraph->Draw("Psame");
                hull->Draw("Lsame");

                std::cout << "Considered points: " << g2->GetN() << std::endl;
                for (int j = 0; j < g2->GetN(); j++)
                {
                    double x, y;
                    g2->GetPoint(j, x, y);
                    //std::cout << x << ", " << y << std::endl;
                } 
				//g2->SetPoint(g2->GetN(), 9999, 0);
				
                /*
                particleGraph->Draw("A");
                mg->Draw("Fsame");
                particleGraph->Draw("Psame");
                g2->Draw("Psame");
                */
                gPad->Modified();
				gPad->Update();
                
                /*
                    
				char ans2 = 'y';
				while (true) {
                std::cout << "Do you want to save these " << i << " points?\ty/n: ";
					cin >> ans2;
				}
				// DONE
				particleGraph->Delete();
			*/
                std::cout << "Done with z = " << z_pos << std::endl;
                TCanvas *imgCanvas = new TCanvas();
                imgCanvas->cd();
                hull->Draw("AL");
                particleGraph->Draw("Psame");
                TImage *img = TImage::Create();
                img->FromPad(imgCanvas);
                img->WriteImage("test.png");
			}

			else {
                std::cout << "Sorry, the file you have chosen is empty, please try again" << std::endl;
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
            std::cout << "Error, please try again using the characters 'y' and 'n'" << std::endl;
			ans = 'y';
        }
	}
    return 0;
}
