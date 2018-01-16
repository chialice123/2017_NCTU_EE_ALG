#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include <time.h>

using namespace std;

void Bezier(int n, double t, double* x, double* y, ofstream& ofs)
{
	// n = #input
	double *dptable_x, *dptable_y;
	dptable_x = new double[n];
	dptable_y = new double[n];
	int i, j;

	// intialize first row
	for (i=0; i<n; ++i){
		dptable_x[i] = x[i];
		dptable_y[i] = y[i];
	}
	
	// calculate dynamic programming
	for (i=1; i<n; ++i){
		for (j=0; j<n-i; ++j){
			dptable_x[j] = (dptable_x[j]-dptable_x[j+1])*t+dptable_x[j+1];
			dptable_y[j] = (dptable_y[j]-dptable_y[j+1])*t+dptable_y[j+1];
		}
	}

	ofs << dptable_x[0] << '\t' << dptable_y[0] << endl;
	
	delete [] dptable_x;
	delete [] dptable_y;
}

int main(int argc, char** argv)
{
	// clock_t timer_begin;
	// timer_begin=clock();
	int n, m;
	ifstream ifs(argv[1]);
	if(!ifs)
		cout<<"input file open fail\n";
	ifs >> n;
	double *in_x, *in_y;
	in_x = new double[n];
	in_y = new double[n];
	
	for (int i=0; i<n; ++i){
		ifs >> in_x[i] >> in_y[i];
	}
	ifs >> m;
	ifs.close();
	
	ofstream ofs(argv[2]);
	if(!ofs)
		cout<<"output file open fail\n";
	
	double offset=1.0/(m-1);
	double t=1-offset;
	
	ofs << fixed << setprecision(2);
	
	// output P0
	ofs << in_x[0] << '\t' << in_y[0] << endl;
	// calculate Ps and output them
	while(t>0){
		Bezier(n,t,in_x,in_y,ofs);
		t=t-offset;
	}
	// output Pn
	ofs << in_x[n-1] << '\t' << in_y[n-1] << endl;
	ofs.close();
	
	delete [] in_x;
	delete [] in_y;
	// printf("Time taken: %.2fs\n", (double)(clock() - timer_begin)/CLOCKS_PER_SEC);
	return 0;
}