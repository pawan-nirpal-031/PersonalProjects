#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
#include <sstream>
using namespace std; 
vector<vector<string>>abstract_tree; 

vector<string> TokenizeLine(string line){ // Tokenize a line 
    vector<string>buffer;
    stringstream stream(line);
    string tmp;
    while(stream>>tmp) buffer.push_back(tmp);
    return buffer;
}


void ParseAndFillTree(const string &ir_file){
    ifstream fin(ir_file);
    string statement;
    while(getline(fin,statement)){
        vector<string> buffer = TokenizeLine(statement); 
        abstract_tree.push_back(buffer);
    }; 
}

int main(){
   string ir_file = "IR.txt"; 
   ParseAndFillTree(ir_file); 
    
   return 0;
}