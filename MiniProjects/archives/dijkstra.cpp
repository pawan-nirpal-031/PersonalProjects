#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
typedef unsigned long long int ull;
typedef long long int ll;
typedef long double ld;
#define Mod 1000000007

struct node{
    int id;
    int cost;
};

void OptimizedAlgorithm(int source,vector<bool> &visited,vector<ll> &distance,vector<int> &cost,vector<vector<int>> &graph){
  int nodes = graph.size()-1;
  distance[source] = cost[source];
  priority_queue<pair<ll,int>,vector<pair<ll,int>>,greater<pair<ll,int>>>processing;
  processing.push({distance[source],source});
  while(processing.size()){
    auto tuple = processing.top();
    processing.pop();
    int currNode = tuple.second;
    int srcToCurrMaxDist = tuple.first;
    // relax all edges from this node
    for(auto &succ : graph[currNode]){
        int succCost = cost[succ];
        if(srcToCurrMaxDist + succCost < distance[succ]){
            distance[succ] = srcToCurrMaxDist + succCost;
            processing.push({distance[succ],succ});
        }
    }
  }
}


int main(){
    int nodes,edges;
    cin>>nodes>>edges; 
    vector<vector<int>>graph(nodes+1); 
    vector<bool>visited(nodes+1,0);
    vector<ll>distance(nodes+1,Mod);
    vector<int>cost(nodes+1,0);
    for(int ed = 0;ed<edges;ed++){
        int from,to;
        cin>>from>>to;
        graph[from].push_back(to);
    }
    for(int nd = 1;nd<=nodes;nd++){
     cin>>cost[nd];
     cost[nd] = -cost[nd];
    }
    // for(int i =1;i<=nodes;i++){
    //     cout<<i<<" "<<cost[i]<<"\n";
    // }
    int source;
    cin>>source;
    OptimizedAlgorithm(source,visited,distance,cost,graph);
    for(int i =1;i<=nodes;i++)
        cout<<i<<" "<<distance[i]<<"\n";
}