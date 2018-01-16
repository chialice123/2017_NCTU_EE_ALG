#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>

using namespace std;


typedef struct{
		double x;
		double y;
	}data;

data** b;
data* in;


void dia(int n){
	for(int i=n;i>=0;i--){
		b[i][i].x=in[i].x;
		b[i][i].y=in[i].y;
	}
}

data bp(double t,int n){
    int temp,i=n;
    
	for(int j=n-1;j>=0;j--){
		temp=j;
		for(int k=n;k>n-i;k--,j--){
			b[j][k].x=(t)*b[j+1][k].x + (1-t)*b[j][k-1].x;
			b[j][k].y=(t)*b[j+1][k].y + (1-t)*b[j][k-1].y;
		}
		j=temp;
		i--;
	}
    return b[0][n];
}



int main(int argc,char*argv[])
{
	int num,sample,usec,sec;
	double t,sample1;
	FILE *ofs;
	struct timeval start, end;

	data* ans;
	ifstream ifs(argv[1]);
    ofs=fopen(argv[2],"w");
	ifs>>num;
	in = new data[num];
	for(int i=0;i<num;i++){
		ifs>>in[i].x>>in[i].y;
	}
	ifs>>sample1;
	sample = sample1;
	sample1--;
	t=1/sample1;

	b = new data*[num];
	for(int i=0;i<num;i++){
        b[i] = new data[num];
	}
	for(int i=0;i<num;i++){
		for(int j=0;j<num;j++){
			b[i][j].x = -1;
            b[i][j].y = -1;
		}
	}
	ans = new data[sample];
///init finish


	gettimeofday(&start, 0);
	dia(num-1);
	for(int i=0;i<sample;i++){
        ans[i]=bp(t*(i),num-1);
    }
	gettimeofday(&end, 0);
	
	for(int i=0;i<sample;i++){
		fprintf(ofs,"%.2f\t%.2f\n",ans[i].x,ans[i].y);
	}
	sec = end.tv_sec - start.tv_sec;
	usec = end.tv_usec - start.tv_usec;
	cout<<"time : "<<sec + (usec/1000000.0)<<endl;



//////finish

	for(int i=0;i<num;++i){
        delete[] b[i];
    }
    delete[] b;
	delete[] ans;
	//fclose(ofs);

	return 0;
}
