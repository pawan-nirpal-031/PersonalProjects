namespace Tests{ 
    
    void DependencyGraphPrinter(set<int> inst_dependency_graph[],vector<vector<string>>& block){
        for(int i =0;i<block.size();i++){
            cout<<i<<" "<<block[i][0]<<" : ";
            for(auto &dec : inst_dependency_graph[i]){
                cout<<dec<<"("<<block[dec][0]<<")"<<" "; 
            }
            cout<<'\n';
        }
    } 

    void DependencyTreesTest(vector<pair<int,vector<int>>> &inst_tree,int tree_indx){
        int num_nodes = inst_tree.size(); 
        cout<<"tree index : "<<tree_indx<<" Nodes in the tree : "<<num_nodes<<"\n";  
        for(int i =0;i<num_nodes;i++){
           auto tuple = inst_tree[i]; 
           cout<<tuple.first<<" : "; 
           for(auto it = tuple.second.begin();it!=tuple.second.end();it++){
                cout<<*(it)<<' ';
           }
           cout<<'\n';
        } 
        cout<<"\n\n";
    }
}