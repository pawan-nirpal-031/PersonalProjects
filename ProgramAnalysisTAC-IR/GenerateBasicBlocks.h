#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;
vector<vector<string>> abstract_tree;

vector<string> TokenizeLine(string line) { // Tokenize a line
  vector<string> buffer;
  stringstream stream(line);
  string tmp;
  while (stream >> tmp)
    buffer.push_back(tmp);
  return buffer;
}

void ParseAndFillTree(const string &ir_file) {
  ifstream fin(ir_file);
  string statement;
  while (getline(fin, statement)) {
    vector<string> buffer = TokenizeLine(statement);
    if (buffer.size() == 0)
      continue;
    abstract_tree.push_back(buffer);
  };
}

class GenerateBasicBlocks {
  vector<pair<int, int>> basic_blocks;
  unordered_set<string> jmp_lables;
  void CreateBasicBlocks() {
    if (abstract_tree.size()) {
      for (int line = 0; line < abstract_tree.size(); line++) {
        vector<string> &toknized_stmt = abstract_tree[line];
        if (toknized_stmt[0].back() == ':')
          toknized_stmt[0].pop_back();
        if (toknized_stmt.size() >= 2 and
            toknized_stmt[toknized_stmt.size() - 2] == "goto") {
          jmp_lables.insert(toknized_stmt.back());
        }
      }
      vector<int> leaders;
      leaders.push_back(0);
      for (int line = 1; line < abstract_tree.size(); line++) {
        vector<string> &toknized_stmt = abstract_tree[line];
        auto label_it = jmp_lables.find(toknized_stmt[0]);
        if ((label_it != jmp_lables.end()) or
            (abstract_tree[line - 1][0] == "if" or
             abstract_tree[line - 1][0] == "goto")) {
          leaders.push_back(line);
        }
      }
      for (int i = 1; i < leaders.size(); i++) {
        basic_blocks.push_back({leaders[i - 1], leaders[i] - 1});
      }
      if (basic_blocks.back().second != abstract_tree.size() - 1)
        basic_blocks.push_back({leaders.back(), abstract_tree.size() - 1});
    }
  }

public:
  GenerateBasicBlocks(const string &ir_file) {
    ParseAndFillTree(ir_file);
    CreateBasicBlocks();
    WriteBasicBlocksToFile();
  }

  unordered_set<string> &GetJumpLabels() { return jmp_lables; }

  void WriteTreeToOutputFile(const string &output_file = "output.txt") {
    ofstream fout(output_file);
    for (int i = 0; i < abstract_tree.size(); i++) {
      vector<string> &toknized_stmt = abstract_tree[i];
      fout << i << "  : ";
      for (const string &token : toknized_stmt)
        fout << token << " ";
      fout << "\n";
    }
    fout.close();
  }

  vector<pair<int, int>> &GetBasicBlocks() { return basic_blocks; }

  void WriteBasicBlocksToFile(const string &output_file = "basic_blocks.txt") {
    ofstream fout(output_file);
    for (const auto &tuple : basic_blocks) {
      int st = tuple.first;
      int end = tuple.second;
      for (int i = st; i <= end; i++) {
        vector<string> &toknized_stmt = abstract_tree[i];
        for (const string &token : toknized_stmt)
          fout << token << " ";
        fout << "\n";
      }
      fout << "\n\n";
    }
    fout.close();
  }

  vector<vector<string>> &GetAbstractTree() { return abstract_tree; }

  void ShowAbstractTree() {
    if (abstract_tree.size()) {
      for (const vector<string> &line : abstract_tree) {
        for (const string &token : line) {
          cerr << token << ' ';
        }
        cerr << "\n";
      }
    } else
      cerr << "Tree empty\n";
  }
};
