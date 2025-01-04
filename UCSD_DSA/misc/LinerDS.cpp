#include <iostream>
#include <vector>
#include <map>
#include <assert.h>
#include <queue>
using namespace std;

int lastStoneWeight(vector<int> &stones){
    priority_queue<int> pq;
    for(int x : stones)
        pq.push(x);
    
    while(pq.size()>1){
        int mx = pq.top();
        pq.pop();
        int smx = pq.top();
        pq.pop();
        pq.push(mx-smx);
    }
    return pq.top();
}

struct pointNode{
    int p;
    pair<int,int> payload;
    pointNode(int x,pair<int,int> pl){
        p = x;
        payload = pl;
    }
};

struct Compare {
    bool operator()(const pointNode& a, const pointNode& b) {
        return a.p > b.p;
    }
};

vector<vector<int>> kClosest(vector<vector<int>>& points, int k) {
    priority_queue<pointNode, std::vector<pointNode>, Compare> queue;
    for(vector<int> &point : points){
        int x = point[0];
        int y = point[1];
        int pq = x*x + y*y;
        pointNode node(pq,{x,y});
        queue.push(node);
    }
    vector<vector<int>> ans;
    for(int i =0;i<k;i++){
        pointNode node = queue.top();
        vector<int> sub = {node.payload.first, node.payload.second};
        queue.pop();
        ans.push_back(sub);
    }
    return ans;
}

int main(){
   
    return 0;
}