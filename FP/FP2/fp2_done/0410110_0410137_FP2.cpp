#include <iostream>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>
#include <math.h>
#include <queue>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


int direct_t[3][9]= {
	{1,	0,	0,	1,	2,	0,	1,	2,	4},
	{2,	0,	0,	2,	3,	0,	0,	0,	0},
	{4,	0,	0,	0,	0,	0,	0,	0,	0}
};

int direct_index[6] = {0};// NE0 NS1 NW2 ES3 EW4 SW5
int Qsize[6] = {0}; // NE0 NS1 NW2 ES3 EW4 SW5
int one_car[4] = {0};
int one_direct[4] = {0};

int main(int argc, char **argv )
{
	clock_t timer_begin;
	timer_begin=clock();
	fstream infile;
	infile.open(argv[1]);
	if(!infile) {
        cout<<"Can't open file!"<<endl;
        return -1;
	}

	vector<int> inN;
	vector<int> inE;
	vector<int> inS;
	vector<int> inW;

	string str;
	int direction;
	int in_rounds = 0;

	//*********************input data****************************
	// read N and count rounds
	infile >> str; // N:
	bool read_N = false;
	while (!read_N) {
		infile >> str;
		if (str=="00"){
			inN.push_back(0);
		} else if (str=="1W"){
			inN.push_back(1);
		} else if (str=="1S"){
			inN.push_back(2);
		} else if (str=="1E"){
			inN.push_back(3);
		} else{
			read_N = true;
			--in_rounds;
		}
		++in_rounds;
	}
	// read E
	for (int i=0; i<in_rounds; ++i){
		infile >> str;
		if (str=="00"){
			inE.push_back(0);
		}else if (str=="1N"){
			inE.push_back(1);
		}else if (str=="1W"){
			inE.push_back(2);
		}else if (str=="1S"){
			inE.push_back(3);
		}
	}
	// read S:
	infile >> str;
	for (int i=0; i<in_rounds; ++i){
		infile >> str;
		if (str=="00"){
			inS.push_back(0);
		}else if (str=="1E"){
			inS.push_back(1);
		}else if (str=="1N"){
			inS.push_back(2);
		}else if (str=="1W"){
			inS.push_back(3);
		}
	}
	// read W:
	infile >> str;
	for (int i=0; i<in_rounds; ++i){
		infile >> str;
		if (str=="00"){
			inW.push_back(0);
		}else if (str=="1S"){
			inW.push_back(1);
		}else if (str=="1E"){
			inW.push_back(2);
		}else if (str=="1N"){
			inW.push_back(3);
		}
	}
	infile.close();

	//**************Create output vectors******************
	vector<int> outN;
	vector<int> outE;
	vector<int> outS;
	vector<int> outW;

	//***************Create Queue**********************
	queue<int> N;
	queue<int> E;
	queue<int> S;
	queue<int> W;

	int timer = 0;
	bool done = false;
	bool input_done = false;

	int Ncur, Ecur, Scur, Wcur;

	while(!done){
		// queue the arrived car
		if(!input_done){
			if(inN[timer]){
				N.push(inN[timer]);
			}
			if(inE[timer])
				E.push(inE[timer]);
			if (inS[timer])
				S.push(inS[timer]);
			if (inW[timer])
				W.push(inW[timer]);
		}
		//insert current arrived cars
		if(N.empty())
			Ncur = -1;
		else
			Ncur = N.front();
		if(E.empty())
			Ecur = -1;
		else
			Ecur = E.front();
		if(S.empty())
			Scur = -1;
		else
			Scur = S.front();
		if(W.empty())
			Wcur = -1;
		else
			Wcur = W.front();


		//***********start managing*********
		if (Ncur ==-1 && Ecur==-1 && Scur==-1 && Wcur==-1){ // no car
			outN.push_back(0);
			outE.push_back(0);
			outW.push_back(0);
			outS.push_back(0);
		} else if (Ncur ==1 && Ecur==1 && Scur==1 && Wcur==1){ //4Rs
			outN.push_back(N.front());
			outE.push_back(E.front());
			outW.push_back(W.front());
			outS.push_back(S.front());
			N.pop();
			E.pop();
			S.pop();
			W.pop();
		} else if (Ncur ==2 && Ecur==1 && Scur==1){ // RRS
			outN.push_back(N.front());
			outE.push_back(E.front());
			outS.push_back(S.front());
			outW.push_back(0);
			N.pop();
			E.pop();
			S.pop();
		} else if (Ncur ==1 && Ecur==1 && Wcur==2){ // RRS
			outN.push_back(N.front());
			outE.push_back(E.front());
			outW.push_back(W.front());
			outS.push_back(0);
			N.pop();
			E.pop();
			W.pop();
		} else if (Ncur ==1  && Scur==2 && Wcur==1){ // RRS
			outN.push_back(N.front());
			outW.push_back(W.front());
			outS.push_back(S.front());
			outE.push_back(0);
			N.pop();
			S.pop();
			W.pop();
		} else if ( Ecur==2 && Scur==1 && Wcur==1){ // RRS
			outE.push_back(E.front());
			outW.push_back(W.front());
			outS.push_back(S.front());
			outN.push_back(0);
			E.pop();
			S.pop();
			W.pop();
		} else if (Ncur ==1 && Ecur==1 && Scur==1){ //3Rs
			outN.push_back(N.front());
			outE.push_back(E.front());
			outS.push_back(S.front());
			outW.push_back(0);
			N.pop();
			E.pop();
			S.pop();
		} else if (Ncur ==1 && Ecur==1 && Wcur==1){ //3Rs
			outN.push_back(N.front());
			outE.push_back(E.front());
			outW.push_back(W.front());
			outS.push_back(0);
			N.pop();
			E.pop();
			W.pop();
		} else if (Ncur ==1  && Scur==1 && Wcur==1){ //3Rs
			outN.push_back(N.front());
			outW.push_back(W.front());
			outS.push_back(S.front());
			outE.push_back(0);
			N.pop();
			S.pop();
			W.pop();
		} else if ( Ecur==1 && Scur==1 && Wcur==1){ //3Rs
			outE.push_back(E.front());
			outW.push_back(W.front());
			outS.push_back(S.front());
			outN.push_back(0);
			E.pop();
			S.pop();
			W.pop();
		} 
		// 2 cars and 1 car
		else {
			if (Ncur!=-1){
				if (Ecur!=-1){
					direct_index[0] = direct_t[Ncur-1][Ecur-1];
					Qsize[0] = N.size() + E.size();
				}
				if (Scur!=-1){
					direct_index[1] = direct_t[Ncur-1][Scur+3-1];
					Qsize[1] = N.size() + S.size();
				}
				if (Wcur!=-1){
					direct_index[2] = direct_t[Ncur-1][Wcur+6-1];
					Qsize[1] = N.size() + S.size();
				}
			}
			if (Ecur!=-1){
				if (Scur!=-1){
					direct_index[3] = direct_t[Ecur-1][Scur-1];
					Qsize[3] = E.size() + S.size();
				}
				if (Wcur!=-1){
					direct_index[4] = direct_t[Ecur-1][Wcur+3-1];
					Qsize[4] = E.size() + W.size();
				}
			}
			if (Scur!=-1){
				if (Wcur!=-1){
					direct_index[5] = direct_t[Scur-1][Wcur-1];
					Qsize[5] = S.size() + W.size();
				}
			}

			// find max direction
			int max_direct = 0;
			bool two_car = false;
			if (direct_index[0]||direct_index[1]||direct_index[2]||
                direct_index[3]||direct_index[4]||direct_index[5]) two_car =true;
            if (two_car){
				for (int i = 0; i<6; i++){
					if (direct_index[max_direct] < direct_index [i])
						max_direct = i;
					 else if (direct_index[max_direct] == direct_index [i])
						 if (Qsize[max_direct] < Qsize[i])
							 max_direct = i;
				}

				if (direct_index[max_direct] != 0){  //check if 2 cars can go 2gether
					switch (max_direct){
						case 0 :
						outN.push_back(N.front());
						outE.push_back(E.front());
						outW.push_back(0);
						outS.push_back(0);
						N.pop();
						E.pop();
						break;
						case 1 :
						outN.push_back(N.front());
						outE.push_back(0);
						outW.push_back(0);
						outS.push_back(S.front());
						N.pop();
						S.pop();
						break;
						case 2 :
						outN.push_back(N.front());
						outE.push_back(0);
						outW.push_back(W.front());
						outS.push_back(0);
						N.pop();
						W.pop();
						break;
						case 3 :
						outN.push_back(0);
						outE.push_back(E.front());
						outW.push_back(0);
						outS.push_back(S.front());
						E.pop();
						S.pop();
						break;
						case 4 :
						outN.push_back(0);
						outE.push_back(E.front());
						outW.push_back(W.front());
						outS.push_back(0);
						E.pop();
						W.pop();
						break;
						case 5 :
						outN.push_back(0);
						outE.push_back(0);
						outW.push_back(W.front());
						outS.push_back(S.front());
						S.pop();
						W.pop();
						break;
					}
				}
            }
			// *****************only one car can go*******************
			else {
				one_car[0] = Ncur;
				one_car[1] = Ecur;
				one_car[2] = Scur;
				one_car[3] = Wcur;

				one_direct[0] = N.size();
				one_direct[1] = E.size();
				one_direct[2] = S.size();
				one_direct[3] = W.size();

				if(one_car[0]>0 || one_car[1]>0 || one_car[2]>0 || one_car[3]>0){
					int max_direct = 0;
					for (int i = 0; i<4; ++i){
						if (one_car[i] > one_car[max_direct])
							max_direct = i;
						else if(one_car[i] == one_car[max_direct] )
							if(one_direct[i] > one_direct[max_direct])
								max_direct = i;
					}

					switch (max_direct){
						case 0 :
						outN.push_back(N.front());
						outE.push_back(0);
						outW.push_back(0);
						outS.push_back(0);
						N.pop();
						break;
						case 1 :
						outN.push_back(0);
						outE.push_back(E.front());
						outW.push_back(0);
						outS.push_back(0);
						E.pop();
						break;
						case 2 :
						outN.push_back(0);
						outE.push_back(0);
						outW.push_back(0);
						outS.push_back(S.front());
						S.pop();
						break;
						case 3 :
						outN.push_back(0);
						outE.push_back(0);
						outW.push_back(W.front());
						outS.push_back(0);
						W.pop();
						break;
					}
				}
			}
		}

		fill(direct_index, direct_index+6, 0); //reset direct_index value to 0
		fill(Qsize, Qsize+6, 0); //reset Qsize value to 0
        ++timer;
		if (timer == in_rounds) input_done = true;
		if ( input_done && N.empty() && E.empty() && S.empty() && W.empty() ) done = true;
	}

    ofstream outfile;
    outfile.open(argv[2]);
    if(!outfile) {
        cout<<"Can't output file"<<endl;
        return -1;
    }

	// **********output data*************************
	int car_num=0;
	outfile<<"N: ";
	for(int i=0; i<outN.size(); i++){
		switch(outN[i]){
			case 0:
			outfile<<"00 ";
			break;
			case 1:
			outfile<<"1W ";
			break;
			case 2:
			outfile<<"1S ";
			break;
			case 3:
			outfile<<"1E ";
			break;
		}
	}
	outfile<<endl;
	outfile<<"E: ";
	for(int i=0; i<outE.size(); i++){
		switch(outE[i]){
			case 0:
			outfile<<"00 ";
			break;
			case 1:
			outfile<<"1N ";
			break;
			case 2:
			outfile<<"1W ";
			break;
			case 3:
			outfile<<"1S ";
			break;
		}
	}
	outfile<<endl;
	outfile<<"S: ";
	for(int i=0; i<outS.size(); i++){
		switch(outS[i]){
			case 0:
			outfile<<"00 ";
			break;
			case 1:
			outfile<<"1E ";
			break;
			case 2:
			outfile<<"1N ";
			break;
			case 3:
			outfile<<"1W ";
			break;
		}
	}
	outfile<<endl;
	outfile<<"W: ";
	for(int i=0; i<outW.size(); i++){
		switch(outW[i]){
			case 0:
			outfile<<"00 ";
			break;
			case 1:
			outfile<<"1S ";
			break;
			case 2:
			outfile<<"1E ";
			break;
			case 3:
			outfile<<"1N ";
			break;
		}
	}
	outfile.close();
	return 0;
}