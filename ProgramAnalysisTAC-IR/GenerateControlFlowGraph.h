#include "GenerateBasicBlocks.h"

class Node{
    int indx; 
    vector<vector<string>>block;
    vector<int>decendants;

public:



    void FillNode(int id,int st,int end,const vector<vector<string>> &tree){
        Node newnode;
        this->indx = id; 
        for(int i =st;i<=end;i++){
            this->block.push_back(tree[i]);
        }
    } 
    
    void InsertDecendent(int dec_node){
        this->decendants.push_back(dec_node);
    }

    void ShowDecendends(){
        for(const int &node : this->decendants){
            cout<<node<<' ';
        }
    }

    void ShowNodeBlockContents(){  
        for(const vector<string>&stmt : block){
            for(const string &token : stmt) cout<<token<<' '; 
            cout<<'\n';
        } 
        cout<<"\n\n";
    }  

    vector<int>& GetDecendends(){
        return this->decendants;
    }

    vector<string> GetStartStatement(){
        return block[0];
    }

    vector<string> GetEndStatement(){
        return block.back();
    } 

    vector<vector<string>>& BlockAccess(){
        return this->block;
    }
};


class ControlFlowGraph{
    vector<Node>graph;

    void GenerateControlFlowGraph(const vector<pair<int,int>> &basic_blocks,const vector<vector<string>> &tree,const unordered_set<string>&jmp_labels){
        unordered_map<string,int>label_to_node; 
        for(int node =0;node<basic_blocks.size();node++){
            int st = basic_blocks[node].first; 
            int end = basic_blocks[node].second; 
            graph[node].FillNode(node,st,end,tree);
            if(jmp_labels.find(tree[st][0])!=jmp_labels.end()){
                label_to_node[tree[st][0]] = node;
            } 
            //graph[node].ShowNodeBlockContents(); 
        } 

        for(int node = 0;node<graph.size();node++){
            vector<string>end = graph[node].GetEndStatement(); 
            if((end[0]=="goto") or (end[0]=="if")){
               string jlabel = end.back(); 
               int dec_node = label_to_node[jlabel];  
               graph[node].InsertDecendent(dec_node);
               if(end[0]=="goto") continue;
            }
            if(node!=graph.size()-1) graph[node].InsertDecendent(node+1);
        }
    }

    void DepthFirstTraverseCFG(int node,vector<bool>&vis){
        vis[node] = 1;  
        // do something 
        graph[node].ShowNodeBlockContents();
        const vector<int>&decendends = graph[node].GetDecendends(); 
        for(int dec : decendends) if(not vis[dec]) DepthFirstTraverseCFG(dec,vis);
    }

public:
    ControlFlowGraph(const string &irfile){
        GenerateBasicBlocks obj(irfile);
        const vector<pair<int,int>> &basic_blocks = obj.GetBasicBlocks();
        const vector<vector<string>>&tree = obj.GetAbstractTree(); 
        const unordered_set<string>&jmp_labels = obj.GetJumpLabels();
        graph.resize(basic_blocks.size()); 
        GenerateControlFlowGraph(basic_blocks,tree,jmp_labels);  
    } 

    vector<Node>& GetFlowGraph(){
        return this->graph;
    }
    
    void ViewAdjecentcyList(){
        for(int i =0;i<graph.size();i++){
            cout<<i<<" : "; 
            graph[i].ShowDecendends();
            cout<<'\n';
        }
    }

    void DepthTraverseCFG(){
        vector<bool>vis(graph.size(),0);
        DepthFirstTraverseCFG(0,vis);
    }
    
};



