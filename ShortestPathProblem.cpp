//#include "ShortestPathProblem.h"
#include <vector>
#include <string>
#include <limits.h>
using namespace std;

void read_graph(int N, vector<vector<long long> >& graph){
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			scanf("%lld", &graph[i][j]);
		}
	}
}

void print_graph(int N, vector<vector<long long> >& graph){
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			printf("%4lld ", graph[i][j]);
		}
		printf("\n");
	}

}

void init_prefix(int N, vector<vector<long long> >& prefixAll){
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			prefixAll[i][j] = i;
		}
	}

}

//start: source point
void print_path(int N, vector<vector<long long> >& prefixAll, long long start){
	for (long long i=0; i<N; i++){
		printf("%lld -> %lld: ", i, start);
		long long j = i;
		while (j!= start && j!=-1){
			printf("%lld -> %lld ", j, prefixAll[start][j]);
			j = prefixAll[start][j];
		}
		printf("\n");
	}
}

bool check_graph(int N, vector<vector<long long> >& graph){
	if ( N<=0 || N!=graph.size() || N!=graph[0].size()){
		printf("N<=0 || N!=graph.size() || N!=graph[0].size()");
		return false;
	}
	for (int i = 0; i<N; i++){
		for (int j= i+1; j<N; j++){
			if (graph[i][j] != graph[j][i]){
				printf("check_graph failed. graph[i][j] != graph[j][i], i=%d, j=%d\n", i, j);
				return false;
			}
		}
	}
	return true;
}

//Floyd-Warshall
//Could Not handle M
vector<vector<vector<long long> > > AllShortestPath_Floyd(int N, int M, vector<vector<long long> >& graph){
	vector<vector<vector<long long> > > ret(2, graph);

	vector<vector<long long> > distAll(graph);
	vector<vector<long long> > prefixAll(N, vector<long long>(N, -1));
	init_prefix(N, prefixAll);

	for (int k = 0; k<M; k++){
		for (int i=0; i<N; i++){
			for (int j=0; j<N; j++){
				if (distAll[i][k] + distAll[k][j] < distAll[i][j]){
					distAll[i][j] = distAll[i][k] + distAll[k][j];
					if (k==0){
						prefixAll[i][j]  = k;
					}
					else{
						prefixAll[i][j] = prefixAll[k][j];
					}
				}
			}
		}
	}


	ret[0] = distAll;
	ret[1] = prefixAll;
	return ret;
}

//Bellman-Ford
vector<vector<vector<long long> > > AllShortestPath_BellmanFord(int N, int M, vector<vector<long long> >& graph){
	vector<vector<vector<long long> > > ret(2, graph);

	vector<vector<long long> > distAll(graph);
	vector<vector<long long> > prefixAll(N, vector<long long>(N, -1));
	init_prefix(N, prefixAll);

	for (int start = 0; start < N; start ++){
		for (int k=0; k< M-1; k++){
			for (int i=0; i<N; i++){
				for (int j=0; j<N; j++){
					if (distAll[start][i] + graph[i][j] < distAll[start][j]){
						distAll[start][j] = distAll[start][i] + graph[i][j];
						prefixAll[start][j] = i;
					}
				}
			}
		}
	}


	ret[0] = distAll;
	ret[1] = prefixAll;
	return ret;
}


//Dijkstra_v0, No PriorityQueue; O(V^3)
vector<vector<vector<long long> > > AllShortestPath_Dijkstra_v0(int N, int M, vector<vector<long long> >& graph){
	vector<vector<vector<long long> > > ret(2, graph);

	vector<vector<long long> > distAll(graph);
	vector<vector<long long> > prefixAll(N, vector<long long>(N, -1));
	init_prefix(N, prefixAll);


	for (int start = 0; start < N; start ++){
		vector<bool> S(N, false);
		S[start] = true;
		
		for (int k=0; k<N; k++){
			for (int i=0; i<N; i++){
				if (S[i]){
					continue;
				}
				int min_v = i;
				for (int j=0; j<N; j++){
					if ( !S[j] && distAll[start][j] < distAll[start][min_v]){
						min_v = j;
					}
				}
				S[min_v] = true;
				for (int j=0; j<N; j++){
					if (distAll[start][min_v] + graph[min_v][j] < distAll[start][j]){
						distAll[start][j] = distAll[start][min_v] + graph[min_v][j];
						prefixAll[start][j] = min_v;
					}
				}
			}
		}
	
	}


	ret[0] = distAll;
	ret[1] = prefixAll;
	return ret;
}


vector<vector<vector<long long> > > AllShortestPath_Dijkstra(int N, int M, vector<vector<long long> >& graph){
	return AllShortestPath_Dijkstra_v0(N, M, graph);
}


vector<vector<vector<long long> > > AllShortestPath(int N, int M, vector<vector<long long> >& graph, const string& algo_name){
	//distance matrix between every pair of nodes
	//vector<vector<vector<long long> > > ret(2, graph);
	if (algo_name == "Floyd-Warshall"){
		return AllShortestPath_Floyd(N, M, graph);
	}
	else if (algo_name == "Bellman-Ford"){
		return AllShortestPath_BellmanFord(N, M, graph);
	}
	else if (algo_name == "Dijkstra"){
		return AllShortestPath_Dijkstra(N, M, graph);
	}
	else{
		vector<vector<vector<long long> > > ret(2, graph);
		printf("not supported: %s\n", algo_name.c_str());
		return ret;
	}
	

}



int main(){
	int N, M;
	scanf("%d %d", &N, &M);
	vector<vector<long long> > graph(N, vector<long long>(N));
	read_graph(N, graph);
	print_graph(N, graph);
	if (!check_graph(N, graph)){		
		return 1;
	}



	string algo_name = "Floyd-Warshall";
	vector<vector<vector<long long> > > distAll_prefixAll = AllShortestPath(N, M, graph, algo_name);
	printf("distAll %s:\n", algo_name.c_str());
	print_graph(N, distAll_prefixAll[0]);

	printf("prefixAll %s:\n", algo_name.c_str());
	print_graph(N, distAll_prefixAll[1]);
	for (int i=0; i<N; i++){

		printf("path: start = %d\n", i);
		print_path(N, distAll_prefixAll[1], i);
	}
	
	algo_name = "Dijkstra";
	distAll_prefixAll = AllShortestPath(N, M, graph, algo_name);
	printf("distAll %s:\n", algo_name.c_str());
	print_graph(N, distAll_prefixAll[0]);

	algo_name = "Bellman-Ford";
	distAll_prefixAll = AllShortestPath(N, M, graph, algo_name);
	printf("distAll %s:\n", algo_name.c_str());
	print_graph(N, distAll_prefixAll[0]);


	
	printf("prefixAll %s:\n", algo_name.c_str());
	print_graph(N, distAll_prefixAll[1]);
	for (int i=0; i<N; i++){

		printf("path: start = %d\n", i);
		print_path(N, distAll_prefixAll[1], i);
	}
	return 0;
}