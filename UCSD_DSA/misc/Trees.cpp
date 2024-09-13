#include <iostream>
#include <vector>
#include <map>
#include <assert.h>
using namespace std;

class Node{
    Node *left,*right;
    int value;
public:
    Node(int val){
        value = val;
        right = left = nullptr;
    }

    int getVal(){
        return value;
    }

    Node* getRightNode(){
        return right;
    }

    Node* getLeftNode(){
        return left;
    }

    void setLeft(Node *n){
        this->left = n;
    }

    void setRight(Node *n){
        this->right = n;
    }

    bool isLeafNode(){
        return getLeftNode()==nullptr and getRightNode()==nullptr;
    }
};

class Tree{
    Node *root;

    void traverseInorderUtil(Node *curr){
        if(curr==nullptr)
            return;
        cout<<curr->getVal()<<" ";
        traverseInorderUtil(curr->getLeftNode());
        
        traverseInorderUtil(curr->getRightNode());
    }

    Node* getNewNode(int val){
        Node *nn = new Node(val);
        return nn;
    }

    void insertNode(int val){
        Node *par = root;
        Node *curr = root;
        while(curr!=nullptr){
            par = curr;
            if(val < curr->getVal())
                curr = curr->getLeftNode();
            else
                curr = curr->getRightNode();
        }
        assert(par!=nullptr && "Null parent to insert");
        Node* insertN = getNewNode(val);
        if(val < par->getVal())
            par->setLeft(insertN);
        else 
            par->setRight(insertN);
    }

    int computeHeightUtil(Node *curr){
        if(curr==nullptr)
            return 0;
        return 1 + max(computeHeightUtil(curr->getLeftNode()), computeHeightUtil(curr->getRightNode()));
    }

    int daimeter(Node *curr, int &d){
        if(curr==nullptr)
            return 0;
        Node *left = curr->getLeftNode();
        Node *right = curr->getRightNode();
        int leftD = daimeter(left,d);
        int rightD = daimeter(right,d);
        // If d passes via this node then l + r + 1 if it does not then d (which is updated throughout the tree)
        d = max(d,1 + leftD + rightD);
        // return depth of the subtree. what you compute and what you return are not the same thing. 
        return 1 + max(leftD,rightD);
    }

    bool checkForBalance(Node *curr,bool bal){
        if(curr==nullptr)
            return 0;
        Node *left = curr->getLeftNode();
        Node *right = curr->getRightNode();
        int leftH = checkForBalance(left,bal);
        int rightH = checkForBalance(right,bal);
        bal = bal & (abs(leftH - rightH) <=1);
        int maxH = 1 + max(leftH,rightH);
        return maxH;
    }

    void computeGoodCount(Node *curr,int &count,int maxV){
        if(curr==nullptr)
            return;
        if(curr->getVal() >= maxV)
            count+=1;
        maxV = max(maxV,curr->getVal());
        computeGoodCount(curr->getLeftNode(),count,maxV);
        computeGoodCount(curr->getRightNode(),count,maxV);
    }
    // https://leetcode.com/problems/count-good-nodes-in-binary-tree/
    int goodNodes(Node* root) {
        int count = 0;
        int maxTillNow = root->getVal();
        computeGoodCount(root,count,maxTillNow);
        return count;
    }

    int getDepthOfNodeInBST(Node *curr,int val,int &d){
        if(curr==nullptr)
            return d;
        if(val==curr->getVal())
            return d;
        Node *left = curr->getLeftNode();
        Node *right = curr->getRightNode();
        d+=1;
        if(left && val < curr->getVal())
           return getDepthOfNodeInBST(left,val,d);
        if(right && val >= curr->getVal())
            return getDepthOfNodeInBST(right,val,d);
        return d;
    }

    Node* getNode(Node *curr,int x){
        if(curr==nullptr)
            return nullptr;
        if(curr->getVal()==x)
            return curr;
        if(x < curr->getVal())
            return getNode(curr->getLeftNode(),x);
        return getNode(curr->getRightNode(),x);
    }

    void buildParentMap(Node *curr, Node *parent,map<Node*,Node*> &parMap){
       if(curr->isLeafNode()){
        parMap[curr] = parent;
        return;
       }
       parMap[curr] = parent;
       buildParentMap(curr->getLeftNode(),curr,parMap);
       buildParentMap(curr->getRightNode(),curr,parMap);
    }

    // Buggy
    Node* lowestCommonAncestor(Node *root,Node *a, Node *b){
        int depthA = 0;
        getDepthOfNodeInBST(root,a->getVal(),depthA);
        int depthB = 0;
        getDepthOfNodeInBST(root,b->getVal(),depthB);
        map<Node*,Node*> parMap;
        buildParentMap(root,nullptr,parMap);
        if(depthA < depthB){
            while(depthB!=depthA){
                b = parMap[b];
                depthB-=1;
            }
        }else if(depthA > depthB){
            while(depthA!=depthB){
                a = parMap[a];
                depthA-=1;
            }
        }
        assert(depthA==depthB && "Unequal depths still");
        while(a!=b){
            a = parMap[a];
            b = parMap[b];
        }
        return a;
    }

public:
    Tree(){
        root = nullptr;
    }

    void buildTree(vector<int> &list){
        if(list.empty())
            return;
        root = getNewNode(list[0]);
        for(int i = 1;i<list.size();i++)
            insertNode(list[i]);
    }

    int computeHeight(){
        return computeHeightUtil(root);
    }

    void traverseInorder(){
        traverseInorderUtil(root);
    }

    int getTreeDaimeter(){
        int d = 0;
        daimeter(root,d);
        return d;
    }

    bool searchNode(int x){
        Node* N = getNode(root,x);
        if(N)
            return true;
        return false;
    }

    int lca(int x, int y){
        Node *Nx = getNode(root,x);
        Node *Ny = getNode(root,y);
        Node *lcaN = lowestCommonAncestor(root,Nx,Ny);
        return lcaN->getVal();
    }

    int getNodeDepth(int x){
        int d = 0;
        return getDepthOfNodeInBST(root,x,d);
    }

    bool checkBalance(){
        bool b = true;
        checkForBalance(root,b);
        return b;
    }
};

int main(){
vector <int> a = {3,9,2,6,4,8,1,5};
    Tree T;
    T.buildTree(a);
    cout<<T.lca(4,1);
    return 0;
}