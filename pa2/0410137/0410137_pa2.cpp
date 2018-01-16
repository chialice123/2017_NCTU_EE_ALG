#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include <time.h>
#include <vector>

using namespace std;

int INT_MAX = 1000000;

//use adjacent matrix to implement graph
struct node{
	int num;
	int x;
	int y;
	int v;
	bool source;
};

bool cmpNode(node a, node b)
{
	if (a.x==b.x)
		return a.y < b.y;
	else
		return a.x < b.x;
}

struct edge {
	//node start; 
	int dest; // use node's num to identify
	int cost; // wirelength
	int resFlow; // the residual amount of flow
	int capacity;
	edge* counterEdge; // for residual graph
};

struct graph {
	int V;	// total number of nodes in the graph
	vector<edge*> *adj; // adjacency list for each vertex
};

graph G, resG;

edge* addEdge(int dest, int capacity, int resFlow, int cost){
    edge* e = new edge;
    e->dest = dest;
    e->capacity = capacity;
    e->resFlow = resFlow;
    e->cost = cost;
	return e;
}

int BFCycleDetection(graph resG, int source, int parentVertex[], edge* parentEdge[])
{
    // Initialize variables that will be needed
    int cycle_node = -1; // no negative cycle
    int numVertices = resG.V;
    vector<edge*> *adj = resG.adj;
    int distance[numVertices];
    // Initialize visited, parentVertex and distance
    for(int i=0; i<numVertices; ++i){
        parentVertex[i] = -1;
        distance[i] = INT_MAX;
    }
    // BF - Relax edges repeatedly
    distance[source] = 0;
    for(int i=0; i<numVertices-1; i++){
        // loop on all edges
        for(int u=0; u<numVertices; u++){
            for(int e=0; e<adj[u].size(); e++){
                if(adj[u][e]->resFlow > 0){
                    int v = adj[u][e]->dest;
                    int w = adj[u][e]->cost;
                    if(distance[v]>distance[u]+w){
                        distance[v] = distance[u]+w;
                        parentVertex[v] = u;
                        parentEdge[v] = adj[u][e];
                    }
                }
            }
        }
    }
    // Check for negative weight cycle - loop on all edges
    for(int u=0; u<numVertices; u++){
        for(int e=0; e<adj[u].size(); e++){
            if(adj[u][e]->resFlow > 0){
                int v = adj[u][e]->dest;
                int w = adj[u][e]->cost;
                if(distance[v]>distance[u]+w){
                    return v;       // Negative cycle detected!
                }
            }
        }
    }
    return cycle_node;
}

void cancelNegativeCycles(graph resGraph)
{
    int costSaved=0, u, v;
	bool cyclePossible = true;
    edge *te1, *te2;
    int numVertices = resGraph.V;
    while(cyclePossible){
        cyclePossible = false;
        for(int i=0; i<numVertices; i++){
            int parent[G.V];
            edge* parentEdge[G.V];
            int node_num = BFCycleDetection(resGraph, i, parent, parentEdge);
            if(node_num!=-1){       // A cycle is detected
                cyclePossible = true;
                // Calculate path flow
                int path_flow = INT_MAX;
                do { // calculate path flow
					v = u;
                    u = parent[v];
                    te1 = parentEdge[v];
                    path_flow = min(path_flow, te1->resFlow);
                } while (v!=node_num);
				
                // Removing the cycle and Updating the graph				
                do {
					v = u;
                    u = parent[v];
                    te1 = parentEdge[v];
                    te2 = te1->counterEdge;
                    te1->resFlow -= path_flow;
                    te2->resFlow += path_flow;
                } while (v!=node_num);
            }
        }
    }
}

bool BF(graph resGraph, int source, int sink, int parentVertex[], edge* parentEdge[])
{
    // Initialize variables that will be needed
    vector<edge*> *adj = resGraph.adj;
    int distance[G.V];
    // Initialize visited, parentVertex and distance
    for(int i=0; i<G.V; i++){
        parentVertex[i] = -1;
        distance[i] = INT_MAX;
    }
    // Bellman-Ford
    distance[source] = 0;
    for(int i=0; i<G.V-1; i++){
        // loop on all edges
        for(int u=0; u<G.V; u++){
            for(int e=0; e<adj[u].size(); e++){
                if(adj[u][e]->resFlow > 0){
                    int v = adj[u][e]->dest;
                    int w = adj[u][e]->cost;
                    if(distance[v]>distance[u]+w){
                        distance[v] = distance[u]+w;
                        parentVertex[v] = u;
                        parentEdge[v] = adj[u][e];
                    }
                }
            }
        }
    }
    if(parentVertex[sink] == -1)
        return false;
    else
        return true;
}

int MinCostFlow(int s, int t, int requiredFlow)
{
    int v, currFlow=0, runningCost=0;
    edge *te1, *te2;
	cancelNegativeCycles(resG);

    // Run shortest path
    int parent[G.V];
    edge* parentEdge[G.V];
    while ( BF(resG, s, t, parent, parentEdge) ){
        int path_flow = INT_MAX;
		v = t;
        while(v!=s){
            te1 = parentEdge[v];
            path_flow = min(path_flow, te1->resFlow);
			v = parent[v];
        }
        path_flow = min(path_flow, requiredFlow-currFlow);
		v = t;
        while(v!=s){
            te1 = parentEdge[v];
            te2 = te1->counterEdge;
            te1->resFlow -= path_flow;
            te2->resFlow += path_flow;
            runningCost += path_flow*(te1->cost);
			v = parent[v];
        }
        currFlow += path_flow;
        if(currFlow == requiredFlow)
            break;
    }
    return runningCost;
}

inline void assign_node(node& n, int num, int x, int y, int v, bool source)
{
	n.num = num;
	n.x = x;
	n.y = y;
	n.v = v;
	n.source = source;
}

inline int dist(node a, node b){
	return abs(a.x-b.x)+abs(a.y-b.y);
}

int main(int argc, char** argv)
{
	clock_t timer_begin;
	timer_begin=clock();
	
	/*--------------- read input file ---------------*/
	int n; // # nodes
	ifstream ifs(argv[1]);
	if(!ifs)
		cout<<"input file open fail\n";
	ifs >> n;
	node* all_nodes;
	all_nodes = new node[n+2]; // numVertices + Ss + Tt
	// 0 is for Ss, 1 is for Tt, start from 2
	int total_v;
	for (int i=0; i<n; ++i){
		// all_nodes[i+2].num = i+2;
		ifs >> all_nodes[i+2].x >> all_nodes[i+2].y >> all_nodes[i+2].v;
		if (all_nodes[i+2].v>=0){
			all_nodes[i+2].source = true; //source
			total_v += all_nodes[i+2].v;
		} else
			all_nodes[i+2].source = false; //sink
	}
	ifs.close();
	/*--------------- Initialize graph ---------------*/
	// add Ss and Tt
	assign_node(all_nodes[0],0,-1,-1,total_v,true); //Ss
	assign_node(all_nodes[1],1,-1,-1,0-total_v,false); //Tt
	stable_sort(all_nodes, all_nodes+(n+2), cmpNode);

	// initialize the graph
	G.V = n+2;
	G.adj = new vector<edge*>[G.V];
	resG.V = n+2;
	resG.adj = new vector<edge*>[resG.V];
	
	// initialize edges of Ss and Tt
	for (int j=2; j<n+2; ++j){
		if (all_nodes[j].source){
			// Ss connects to all sources
			edge* tmpEdge1 = addEdge(j, all_nodes[j].v, all_nodes[j].v, 0);
			edge* tmpEdge2 = addEdge(0, all_nodes[j].v, 0, 0);
			tmpEdge1->counterEdge = tmpEdge2;
			tmpEdge2->counterEdge = tmpEdge1;
			G.adj[0].push_back(tmpEdge1);
			resG.adj[0].push_back(tmpEdge1);
			resG.adj[j].push_back(tmpEdge2);
		} else {
			// all sinks connect to Tt
			edge* tmpEdge1 = addEdge(1, 0-all_nodes[j].v, 0-all_nodes[j].v, 0);
			edge* tmpEdge2 = addEdge(j, 0-all_nodes[j].v, 0, 0);
			tmpEdge1->counterEdge = tmpEdge2;
			tmpEdge2->counterEdge = tmpEdge1;
			G.adj[j].push_back(tmpEdge1);
			resG.adj[j].push_back(tmpEdge1);
			resG.adj[1].push_back(tmpEdge2);
		}
	}
	
	// initialize all sources to all sinks
	edge temp, min_dist;
	for (int i=2; i<n+2; ++i){
		if (all_nodes[i].source){ // i is source --> find sink
			for (int j=2; j<n+2; ++j){
				if (!all_nodes[j].source){ // j is sink
					int cap = min(all_nodes[i].v,0-all_nodes[j].v);
					int cost = dist(all_nodes[i],all_nodes[j]);
					// cout << j << " ";
					edge* tmpEdge1 = addEdge(j, cap, cap, cost);
					// cout << i << " ";
					edge* tmpEdge2 = addEdge(i, cap, 0, 0-cost);
					tmpEdge1->counterEdge = tmpEdge2;
					tmpEdge2->counterEdge = tmpEdge1;
					G.adj[i].push_back(tmpEdge1);
					resG.adj[i].push_back(tmpEdge1);
					resG.adj[j].push_back(tmpEdge2);
				}
			}
		}
	}
	
	/*--------------- min cost max flow ---------------*/
	int mincost = MinCostFlow(0, 1, total_v);
	// cout << mincost << endl;
	
	/*--------------- output file ---------------*/
	FILE* ofs;
	ofs = fopen(argv[2],"w");
	fprintf(ofs,"%d\n", mincost);
	for(int i=2; i<G.V; i++){
		if (all_nodes[i].source){
			for(int j=0; j<G.adj[i].size(); j++){
				if ((G.adj[i][j]->capacity - G.adj[i][j]->resFlow)){
					fprintf(ofs,"%d %d ", all_nodes[i].x, all_nodes[i].y);
					fprintf(ofs,"%d %d ", all_nodes[G.adj[i][j]->dest].x, all_nodes[G.adj[i][j]->dest].y);
					fprintf(ofs,"%d\n", (G.adj[i][j]->capacity - G.adj[i][j]->resFlow));
				}
			}
		}
    }
	fclose(ofs);
	/*--------------- free memory ---------------*/
	delete[] all_nodes;

	printf("Time taken: %.2fs\n", (double)(clock() - timer_begin)/CLOCKS_PER_SEC);
	return 0;
}