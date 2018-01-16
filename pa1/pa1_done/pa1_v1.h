#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include <time.h>

using namespace std;

struct coord
{
	double x, y;
};

void binomialCoefficientTable(int n, int k, long* B)
{
    long C[n+1][k+1];
    int i, j;
 
    // Caculate value of Binomial Coefficient in bottom up manner
    for (i = 0; i <= n; i++){
        for (j = 0; j <= min(i, k); j++){
			if(j>i)
				continue;
            // Base Cases
            if (j == 0 || j == i)
                C[i][j] = 1;
            // Calculate value using previosly stored values
            else
                C[i][j] = C[i-1][j-1] + C[i-1][j];
        }
    }
	
	for (int i=0; i<(n+1)/2+1; i++){
		B[i] = C[n][i];
		B[n-i] = B[i];
	}
}

struct coord Bezier(int n, double t, struct coord* M, long* C)
{
    
	struct coord K={0,0};

	for (int i=0; i<n+1; i++){
		K.x += pow(1-t,i)*pow(t,n-i)*C[i]*M[i].x;
		K.y += pow(1-t,i)*pow(t,n-i)*C[i]*M[i].y;
	}
	
    return K;
}

int main(int argc, char** argv)
{
	time_t timer_begin, timer_end;
	time(&timer_begin);
	int n, m;
	ifstream ifs(argv[1]);
	if(!ifs)
		cout<<"input file open fail\n";
	ifs >> n;
	struct coord in[n];
	for (int i=0; i<n; i++){
		ifs >> in[i].x >> in[i].y;
	}
	ifs >> m;
	ifs.close();
	
	ofstream ofs(argv[2]);
	if(!ofs)
		cout<<"output file open fail\n";
	double t=1;
	double offset=1.0/(m-1);
	
	long C[n];
	binomialCoefficientTable(n-1,n/2+1,C);
	ofs << fixed << setprecision(2);
	struct coord temp;
	while(t>0){
		if(t==1){
			ofs << in[0].x << '\t' << in[0].y << endl;
		} else {
			temp = Bezier(n-1, t, in, C);
			ofs << temp.x << '\t' << temp.y << endl;
		}
		t=t-offset;
	}
	ofs << in[n-1].x << '\t' << in[n-1].y;
	ofs.close();
	time(&timer_end);
	// float sec = difftime(timer_end,timer_begin);
	// cout << "time = "  << sec << "s" << endl;
	return 0;
}
