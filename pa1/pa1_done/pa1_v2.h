#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include <time.h>

using namespace std;

void Bezier(int n, double t, double* x, double* y, double &out_x, double &out_y)
{
	// n = #input
	double dptable_x[n][n];
	double dptable_y[n][n];

	// intialize first row
	for (int i=0; i<n; i++){
		dptable_x[0][i] = x[i];
		dptable_y[0][i] = y[i];
	}
	
	// calculate dynamic programming
	for (int i=1; i<n; i++){
		for (int j=0; j<n-i; j++){
			dptable_x[i][j] = dptable_x[i-1][j]*t+dptable_x[i-1][j+1]*(1-t);
			dptable_y[i][j] = dptable_y[i-1][j]*t+dptable_y[i-1][j+1]*(1-t);
		}
	}
	out_x=dptable_x[n-1][0];
	out_y=dptable_y[n-1][0];
}

int main(int argc, char** argv)
{
	clock_t timer_begin;
	timer_begin=clock();
	int n, m;
	ifstream ifs(argv[1]);
	if(!ifs)
		cout<<"input file open fail\n";
	ifs >> n;
	double in_x[n], in_y[n];
	
	double out_x, out_y;
	for (int i=0; i<n; i++){
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
		Bezier(n,t,in_x,in_y,out_x,out_y);
		ofs << out_x << '\t' << out_y << endl;
		t=t-offset;
	}
	// output Pn
	ofs << in_x[n-1] << '\t' << in_y[n-1] << endl;
	ofs.close();
	printf("Time taken: %.2fs\n", (double)(clock() - timer_begin)/CLOCKS_PER_SEC);
	return 0;
}
