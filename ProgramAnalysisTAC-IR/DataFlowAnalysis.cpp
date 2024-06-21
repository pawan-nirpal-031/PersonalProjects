#include "GenerateControlFlowGraph.h" 
#include "Helpers.h" 
#include "Instructions.h" 
#include "SystemIO.h" 
#include "Testing.h"
#include <cassert>

/*
    common subexpression elimination 
    live variable analysis 
    constant folding 
    constant propogration 
    use-define chain  
    available expression 
    dead store elimination 
    strength reduction 
    local value numbering  
*/
 
class InstructionLevelParallelismAnalysisAndTransforms{
public:
    static void InstructionSchedulingAndSingleCycleIssue(){


       
        
    } 

    static void BlockReordering(vector<vector<string>>& block,vector<vector<pair<int,vector<int>>>> &instruction_issue_forest){
        
    }

    static void GenerateInstructionStream(vector<pair<int,vector<int>>>& instruction_dep_tree,const set<int> inst_dependency_graph[],vector<bool> &visited,int node){
        pair<int,vector<int>>branches; 
        visited[node] = 1; 
        branches.first = node; 
        for(const int &child : inst_dependency_graph[node]){
            if(not visited[child]){
                branches.second.push_back(child); 
                GenerateInstructionStream(instruction_dep_tree,inst_dependency_graph,visited,child);
            }
        } 
        instruction_dep_tree.push_back(branches);
    }

    static void GenerateInstructionIssueForest(const int &block_size,const set<int> inst_dependency_graph[],vector<vector<pair<int,vector<int>>>> &issue_forest){
        vector<bool>visited(block_size,0);  
        for(int node =0;node<block_size;node++){
            if(not visited[node]){  
                vector<pair<int,vector<int>>>instruction_dep_tree; 
                GenerateInstructionStream(instruction_dep_tree,inst_dependency_graph,visited,node); 
                issue_forest.push_back(instruction_dep_tree);
            }
        } 
        for(int tree = 0;tree<issue_forest.size();tree++) Tests::DependencyTreesTest(issue_forest[tree],tree);
    }

    static void ParallelIssueInstructionStreamSearch(vector<vector<string>>& block){ 
        /* 
            Look for parallization opportunities and independent instructions that can be issued in the same cycle ie improved ILP. 
            It also enables for block reordering as independent instruction streams can be put togather improving working set size, 
            spatial locality and imporved cache hit ratio. It will also allow variable Live range compression which helps in 
            efficent register allocation and code generation.
        */ 
       unordered_map<string,int> oprnd_to_inst; 
       for(int i =0;i<block.size();i++){
            vector<string> &stmt = block[i]; 
            if(stmt[1]=="=") oprnd_to_inst[stmt[0]] = i;
       } 
       
       set<int> inst_dependency_graph[block.size()]; 
       for(int i =0;i<block.size();i++){
            vector<string> &inst = block[i]; 
            if(inst[1]=="="){
                assert(inst.size()>=3);
                if(IsAssignment(inst)){ 
                    if(oprnd_to_inst.find(inst[2])==oprnd_to_inst.end()) continue;
                    int inst_from = oprnd_to_inst[inst[2]]; 
                    int inst_to = i;  
                    inst_dependency_graph[inst_from].insert(inst_to);
                }else if(IsMemoryAccess(inst)){ 
                    if(oprnd_to_inst.find(inst[3])==oprnd_to_inst.end()) continue;
                    int inst_from = oprnd_to_inst[inst[3]]; 
                    int inst_to = i;  
                    inst_dependency_graph[inst_from].insert(inst_to);
                }else if(inst.size()==5 and IsArithmaticOrLogicalOpreation(inst[3])){ 
                    int inst_to = i; 
                    if(oprnd_to_inst.find(inst[2])!=oprnd_to_inst.end()){
                        int inst_from_1 = oprnd_to_inst[inst[2]];  
                        inst_dependency_graph[inst_from_1].insert(inst_to); 
                    }
                    if(oprnd_to_inst.find(inst[4])!=oprnd_to_inst.end()){
                        int inst_from_2 = oprnd_to_inst[inst[4]]; 
                        inst_dependency_graph[inst_from_2].insert(inst_to);
                    }
                }
            }else if(inst[0]==SystemIO::read or inst[0]==SystemIO::write){
                assert(inst.size()==2);
                if(oprnd_to_inst.find(inst[1])==oprnd_to_inst.end()) continue; 
                int inst_from = oprnd_to_inst[inst[1]]; 
                int inst_to = i;  
                inst_dependency_graph[inst_from].insert(inst_to);
            }else{
                if(inst.size()==1 or inst[0]=="goto") continue; 
                else if(inst[0]=="if"){
                    assert(inst.size()==4 or inst.size()==6); 
                    if(inst.size()==4){ 
                        if(oprnd_to_inst.find(inst[1])==oprnd_to_inst.end()) continue;
                        int inst_from = oprnd_to_inst[inst[1]]; 
                        int inst_to = i;  
                        inst_dependency_graph[inst_from].insert(inst_to);
                    }else{
                        int inst_to = i; 
                        if(oprnd_to_inst.find(inst[1])!=oprnd_to_inst.end()){
                            int inst_from_1 = oprnd_to_inst[inst[1]];  
                            inst_dependency_graph[inst_from_1].insert(inst_to); 
                        }
                        if(oprnd_to_inst.find(inst[3])!=oprnd_to_inst.end()){
                            int inst_from_2 = oprnd_to_inst[inst[3]]; 
                            inst_dependency_graph[inst_from_2].insert(inst_to);
                        }
                    }
                }else if(inst[0]=="return"){ 
                    if(oprnd_to_inst.find(inst[1])==oprnd_to_inst.end()) continue;
                    int inst_from = oprnd_to_inst[inst[1]]; 
                    int inst_to = i;  
                    inst_dependency_graph[inst_from].insert(inst_to);
                }
            }
        } 
        //Tests::DependencyGraphPrinter(inst_dependency_graph,block);
        vector<vector<pair<int,vector<int>>>> instruction_issue_forest;  
        GenerateInstructionIssueForest(block.size(),inst_dependency_graph,instruction_issue_forest); 
    }
};





class BlockLevelAnalysis{
    unordered_map<string,int>VirtualArithLogicRelnOpcodetable; 
    unordered_set<string>relationOpreators; 
    unordered_map<string,long long int>propvalue; 

    void MultiplicationTransforms(vector<string>& stmt){
        if(stmt[2]=="0" or stmt[4]=="0"){
            RemoveLastKTokens(stmt,3);
            stmt.push_back("0");
        }else if(stmt[2]=="1" or stmt[4]=="1"){
            if(stmt[4]!="1") swap(stmt[2],stmt[4]); 
            RemoveLastKTokens(stmt,2);
        }else{
            bool isOp1Num = IsANumber(stmt[2]);
            bool isOp2Num = IsANumber(stmt[4]);
            long long int num1,num2; 
            if(isOp1Num) num1 = ExtractNumber(stmt[2]);
            if(isOp2Num) num2 = ExtractNumber(stmt[4]);
            if(isOp1Num or isOp2Num){
                if(isOp1Num and IsAPowerOf2(num1)){ // x = 2^p * y
                    swap(stmt[2],stmt[4]); // x = y * 2^p
                    int power = WhatPowerOf2(num1); // p 
                    RemoveLastKTokens(stmt,2);
                    stmt.push_back("<<");
                    stmt.push_back(to_string(power));
                }else if(isOp2Num and IsAPowerOf2(num2)){
                    int power = WhatPowerOf2(num2); 
                    RemoveLastKTokens(stmt,2);
                    stmt.push_back("<<");
                    stmt.push_back(to_string(power));
                }
            }
        }
    } 

    void DivisonTransforms(vector<string>& stmt){
        assert(stmt[4]!="0");
        if((stmt[2]=="0") or (stmt[2]=="1" or stmt[4]=="1")){// x = y / z
            RemoveLastKTokens(stmt,2);
        }else{
            bool isOp1Num = IsANumber(stmt[2]);
            bool isOp2Num = IsANumber(stmt[4]);
            long long int num1,num2; 
            if(isOp1Num) num1 = ExtractNumber(stmt[2]);
            if(isOp2Num) num2 = ExtractNumber(stmt[4]);
            if(isOp1Num or isOp2Num){
                if(isOp1Num and IsAPowerOf2(num1)){ // x = 2^p / y
                    swap(stmt[2],stmt[4]); // x = y / 2^p
                    int power = WhatPowerOf2(num1); // p 
                    RemoveLastKTokens(stmt,2);
                    stmt.push_back(">>");
                    stmt.push_back(to_string(power));
                }else if(isOp2Num and IsAPowerOf2(num2)){
                    int power = WhatPowerOf2(num2); 
                    RemoveLastKTokens(stmt,2);
                    stmt.push_back(">>");
                    stmt.push_back(to_string(power));
                }
            }
        }
    }

    void StrengthReductionAndArithmaticTransforms(vector<vector<string>>& block){
        for(int i =0;i<block.size();i++){
            vector<string>&stmt = block[i];
            if(IsArithmaticStatement(stmt)){
                int vopcode = VirtualArithLogicRelnOpcodetable[stmt[3]];
                switch (vopcode){
                    case(ADD):{// x = 0 + 0
                        if(stmt[2]=="0") swap(stmt[2],stmt[4]);
                        if(stmt[4]=="0") RemoveLastKTokens(stmt,2);
                    }break;
                    case(SUB):{
                        if(stmt[4]=="0") RemoveLastKTokens(stmt,2);
                        else if(stmt[2]=="0"){
                            string num2 = stmt[4]; 
                            RemoveLastKTokens(stmt,3); 
                            if(num2=="0"){
                                stmt.push_back("0");
                            }else if(num2[0]=='-') stmt.push_back(to_string(abs(ExtractNumber(num2))));
                            else stmt.push_back("-"+num2);
                        }
                    }break;
                    case(MUL):{// strength reduction for powers of 2, mul with 0, mul with 1 
                        MultiplicationTransforms(stmt);
                    }break;
                    case(DIV):{// strength reduction for powers of 2, div with 0, div with 1 
                       DivisonTransforms(stmt);
                    }break;
                    default: break;
                }
            }else if(stmt[0]=="if" and stmt.size()==6){ // if x op y goto L
                int vopcode = VirtualArithLogicRelnOpcodetable[stmt[2]];
                switch(vopcode){
                    case(ADD):{
                        //
                    }break;
                }
            }
        }
    }   

    
    void ConstantFolding(vector<vector<string>>& block){
        for(int i = 0;i<block.size();i++){
            vector<string>& stmt = block[i]; 
            if(IsArithmaticStatement(stmt)){ 
               bool isnum1 = IsANumber(stmt[2]); 
               bool isnum2 = IsANumber(stmt[4]); 
               if(isnum1 and isnum2){
                   long long int num1 = ExtractNumber(stmt[2]); 
                   long long int num2 = ExtractNumber(stmt[4]);
                   int Vopcode = VirtualArithLogicRelnOpcodetable[stmt[3]];
                   long long int res = ConstantFoldingUtil(num1,num2,Vopcode);
                   RemoveLastKTokens(stmt,3);  
                   stmt.push_back(to_string(res)); 
               }
            }else if(stmt[0]=="if" and stmt.size()==6){
                bool isnum1 = IsANumber(stmt[1]); 
                bool isnum2 = IsANumber(stmt[3]); 
                if(isnum1 and isnum2){
                    long long int num1 = ExtractNumber(stmt[1]); 
                    long long int num2 = ExtractNumber(stmt[3]);
                    int Vopcode = VirtualArithLogicRelnOpcodetable[stmt[2]];
                    long long int res = ConstantFoldingUtil(num1,num2,Vopcode);
                    string jmplabel = stmt.back(); 
                    RemoveLastKTokens(stmt,5);  
                    stmt.push_back(to_string(res));
                    stmt.push_back("goto"); 
                    stmt.push_back(jmplabel);
                }
            }
        }
    }

    void ConstantPropagation(vector<vector<string>>& block){
        for(int i =0;i<block.size();i++){
            vector<string>& stmt = block[i];  
            if(stmt.size()==3 and stmt[1]=="=" and IsANumber(stmt[2]) and propvalue.find(stmt[0])==propvalue.end()){ 
                long long int val = ExtractNumber(stmt[2]);  
                propvalue[stmt[0]] = val; 
                continue;
            }
            if(IsArithmaticStatement(stmt)){
                auto v1 = propvalue.find(stmt[2]);
                auto v2 = propvalue.find(stmt[4]);
                if(v1!=propvalue.end()) stmt[2] = to_string(v1->second); 
                if(v2!=propvalue.end()) stmt[4] = to_string(v2->second);
            }else if(IsMemoryAccess(stmt)){// x = * z
                auto v = propvalue.find(stmt[3]);
                if(v!=propvalue.end()) stmt[3] = to_string(v->second);
            }else if(stmt[0]=="print" or stmt[0]=="scan"){
                auto v = propvalue.find(stmt[1]);
                if(v!=propvalue.end()) stmt[1] = to_string(v->second);
            }else if(IsAssignment(stmt)){
                auto v = propvalue.find(stmt[2]);
                if(v!=propvalue.end()) stmt[2] = to_string(v->second);
            }else{// control flow
                if(stmt[0]=="if"){
                    if(stmt.size()==6){// if x op y goto L
                        auto v1 = propvalue.find(stmt[1]);
                        auto v2 = propvalue.find(stmt[3]);
                        if(v1!=propvalue.end()) stmt[1] = to_string(v1->second); 
                        if(v2!=propvalue.end()) stmt[3] = to_string(v2->second); 
                    }else if(stmt.size()==4){// if x goto L
                        auto v = propvalue.find(stmt[1]);
                        if(v!=propvalue.end()) stmt[1] = to_string(v->second);
                    }
                }else if(stmt[0]=="return"){
                    auto v = propvalue.find(stmt[1]);
                    if(v!=propvalue.end()) stmt[1] = to_string(v->second);
                }
            }
        }
    }

   void InitVirtualTables(){
        vector<string>opcodes = {"+","-","*","/","%","|","&","^","||","&&",">>","<<",">=",">","<=","<","!=","=="};
        for(int i =0;i<opcodes.size();i++) VirtualArithLogicRelnOpcodetable[opcodes[i]] = i+1;
    }

public:
    BlockLevelAnalysis(){
        InitVirtualTables();
    }

    void RunPassOnBlock(vector<vector<string>>& block,int passes){
       for(int p = 0;p<passes;p++){
            //ConstantFolding(block); 
           // ConstantPropagation(block); 
           InstructionLevelParallelismAnalysisAndTransforms::ParallelIssueInstructionStreamSearch(block);
       }
    }
};

class FlowGraphLevelAnalysis{

};

class DataFlowAnalysis{

};

int main(){
    const string irfile = "IR.txt";
    ControlFlowGraph graph_obj(irfile);
    vector<Node>&graph = graph_obj.GetFlowGraph();
    int node = 0;
    BlockLevelAnalysis analyser; 
    analyser.RunPassOnBlock(graph[node].BlockAccess(),1);
   // graph[node].ShowNodeBlockContents(); 
    return 0;
}