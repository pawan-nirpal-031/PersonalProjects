#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long int ull;
typedef long long int ll;
typedef long double ld;
#define Mod 1000000007
#define Infinity 18446744073709551615
#define Pair(a,b) make_pair(a,b)
#define IncresePrecision(x) std::fixed<<setprecision(8)<<x
#define SetBits(x) __builtin_popcount(x);
#define Case(i,x) cout<<"Case #"<<i<<": "<<x<<'\n'
#define FastIO ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
#define Status(b) (cout<<(b?"YES\n":"NO\n"));
#define Debug 0


struct point{
  ll x,y;
};

ull square(const ll &x){
  return x*x;
}

ld GetDistance(const point &p1,const point &p2){
  ld dist = sqrt(square(p1.x-p2.x)+square(p1.y-p2.y)); 
  return dist;
}

bool sort_x(const point &p1,const point &p2){
  return p1.x<p2.x;
}

bool sort_y(const point &p1,const point &p2){
  return p1.y<p2.y;
}



double ClosestPoints(const vector<point> &points,int l,int r){ 
  if(l>=r) return LDBL_MAX;
  int n = r - l + 1; 
  if(n==2) return GetDistance(points[l],points[r]); 
  int m = (l+r)/2;
  ld dleft = ClosestPoints(points,l,m);
  ld dright = ClosestPoints(points,m+1,r);
  ld d = min(dleft,dright);
  vector<point> strip; 
  ll line = points[m].x;
  for(int i =l;i<=m;i++){
    if(points[i].x>line-d) strip.push_back(points[i]);
  }
  for(int i =m+1;i<=r;i++){
    if(points[i].x<line+d) strip.push_back(points[i]);
  }
  sort(strip.begin(),strip.end(),sort_y); 
  ld min_dist = LDBL_MAX;  
  for(int i =0;i<strip.size();i++){ 
    for(int j = i+1;(j<strip.size() and (abs(strip[i].y-strip[j].y)<d));j++){
      min_dist = min(min_dist,GetDistance(strip[i],strip[j])); 
    }
  }  
  return min(min_dist,d);
}


int main(){
  FastIO;
  int n;
  cin>>n; 
  vector<point> data(n,{0,0});
  for(int i =0;i<n;i++){
    cin>>data[i].x>>data[i].y;
  }
 // auto start = chrono::high_resolution_clock::now();
  sort(data.begin(),data.end(),sort_x);
  ld ans = ClosestPoints(data,0,n-1);  
//  auto end = chrono::high_resolution_clock::now();
  cout<<IncresePrecision(ans)<<'\n';
  // double time_taken = 
  //     chrono::duration_cast<chrono::nanoseconds>(end - start).count();
  
  //   time_taken *= 1e-9;
  
  //   cout << "Time taken by program is : " << fixed 
  //        << time_taken << setprecision(9);
  //   cout << " sec" << endl;
  return 0;
} 
// t(n) = 2T(n/2) + nlogn + n => nlog^2(n)
