#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long int ull;
typedef long long int ll;
typedef long double ld;
#define Mod 1000000007
#define Infinity (ll)1e18
#define Append(a) push_back(a)
#define Pair(a,b) make_pair(a,b)
#define Clear(a) for(ll &x : a){x=0;}
#define Point(x) std::fixed<<setprecision(15)<<x
#define SetBits(x) __builtin_popcount(x);
#define DebugCase(i,x) cout<<"Case #"<<i<<": "<<x<<'\n'
#define FastIO ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
#define Status(b) (cout<<(b?"YES\n":"NO\n"));
#define DEBUG false

double getDistanceBetween(vector<pair<int,int>> &points,int i,int j){
  auto point1 = points[i];// x1 y1
  auto point2 = points[j];// x2 y2
  double dist = sqrt((point1.first - point2.first)*(point1.first - point2.first) + (point1.second - point2.second)*(point1.second - point2.second));
  return dist;
}

bool possibleToFormKClusters(vector<pair<int,int>> &points,int k, double d){
  vector<int> component(points.size(),0);
  int n = points.size();
  // Label all points in components k as k. [1 1 2 2 3 ... k k k ] if there remains a 0 means it was not part of any component.
  int comp = 1;
  for(int i =0;i<n;i++){
    if(component[i]==0){
      component[i] = comp;
      // Try to fill all the nodes that satisfy the distance <= d in component i+1.
      for(int j = i+1;j<n;j++){
        if(component[j]>0)
          continue;
        double dij = getDistanceBetween(points,i,j);
        if(dij < d)
          component[j] = comp;
      }
      comp+=1;
    }
  }

  #if DEBUG
    for(int i =0;i<n;i++)
      cout<<i <<" "<<component[i]<<"\n";
  #endif

  // Now check if we have annotated each node with component number from [1 to k] if component[x] = 0 means this node came under no component and if component[x] > k means we need more than k clusters hence return false.
  for(int i =0;i < n;i++)
    if(!(component[i]>=1 and component[i]<=k))
      return false;
  return true;
}

double clusteringBinarySearchApproach(vector<pair<int,int>> &points,int k){
  /*
      Fails for cases like FIXME. 
      4
      0 0
      0 1
      1 0
      1 1
      3
  */
  int n = points.size();
  vector<double> edges;
  for(int i =0;i<n;i++){
    for(int j = i+1;j<n;j++){
      double distOfIJ = getDistanceBetween(points,i,j);
      edges.push_back(distOfIJ);
    }
  }
  sort(edges.begin(),edges.end());
  #if DEBUG
    for(int i =0;i<edges.size();i++)
      cout<<edges[i]<<" : \n"<<possibleToFormKClusters(points,k,edges[i])<<"\n\n";
  #endif
  int low = -1;
  int high = edges.size();
  while(low + 1 < high){
    int m = ( high + low )/2;
    double d = edges[m];
    if(!possibleToFormKClusters(points,k,d))
      low = m;
    else
      high = m;
  }
  return edges[high];
}

int findLeader(vector<int> &components,int u){
  if(components[u]==u)
    return u;
  return components[u] = findLeader(components,components[u]);  
}

void mergeComponents(vector<int> &components,int point1,int point2){
  int leader1 = findLeader(components,point1);
  int leader2 = findLeader(components,point2);
  if(leader1==leader2)
    return;
  components[leader1] = components[leader2];
}

float clusteringUsingKruskals(vector<pair<int,int>> &points,int k){
  int n = points.size();
  vector<pair<double,pair<int,int>>> edges;
  for(int i =0;i<n;i++){
    for(int j = i+1;j<n;j++){
      double distOfIJ = getDistanceBetween(points,i,j);
      edges.push_back({distOfIJ,{i,j}});
    }
  }
  sort(edges.begin(),edges.end());
  if(n==k){
    return edges.back().first;
  }
  // 0 is no component. components are 1 to k
  vector<int> components(n,0);
  for(int i =0;i<n;i++)
    components[i] = i;
  
  int edgeCount = 0;
  double ans = 0.0;
  int i;
  for(i =0;i<edges.size();i++){
    auto tuple = edges[i];
    int point1Indx = tuple.second.first;
    int point2Indx = tuple.second.second;
    int p1Leader = findLeader(components,point1Indx);
    int p2Leader = findLeader(components,point2Indx);
    if(p1Leader!=p2Leader){
      mergeComponents(components,p1Leader,p2Leader);
      edgeCount+=1;
    }
    if(edgeCount == n-k){
      #if DEBUG
      for(int x=0;x<n;x++)
        for(int j =0;j<n;j++)
          findLeader(components,j);
      
      for(int x =0;x<n;x++)
          cout<<x<<" : "<<components[x]<<"\n";
      #endif
    }
    if(edgeCount > n-k){
      ans = tuple.first;
      break;
    }
  }
  return ans;
}

int main(){
  FastIO;
  int numPoints;
  cin>>numPoints;
  vector<pair<int,int>> points;
  for(int i =0;i<numPoints;i++){
    int x,y;
    cin>>x>>y;
    points.push_back(make_pair(x,y));
  }
  int k;
  cin>>k;
  double mindist = clusteringUsingKruskals(points,k);
  cout<<fixed<<setprecision(15)<<mindist;
  return 0;
}