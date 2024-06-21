#include "GenerateBasicBlocks.h"

class Node {
  int indx;
  vector<vector<string>> block;
  vector<int> decendants;
  vector<int> predecessors;
  unsigned numInsts;

public:
  void FillNode(int id, int st, int end, const vector<vector<string>> &tree) {
    Node newnode;
    this->indx = id;
    for (int i = st; i <= end; i++) {
      this->block.push_back(tree[i]);
    }
    numInsts = block.size();
  }

  unsigned getNumInstructions() { return numInsts; }

  void InsertDecendent(int dec_node) { this->decendants.push_back(dec_node); }

  void InsertPredecessor(int preNode) { predecessors.push_back(preNode); }

  void ShowDecendends() {
    for (const int &node : this->decendants) {
      cout << node << ' ';
    }
  }

  void ShowPredecessors() {
    for (const int &node : this->predecessors) {
      cout << node << ' ';
    }
  }

  void ShowNodeBlockContents() {
    for (const vector<string> &stmt : block) {
      for (const string &token : stmt)
        cout << token << ' ';
      cout << '\n';
    }
    cout << "\n\n";
  }

  vector<int> &GetDecendends() { return this->decendants; }

  vector<string> GetStartStatement() { return block[0]; }

  vector<string> GetEndStatement() { return block.back(); }

  vector<vector<string>> &getBlock() { return this->block; }
};

class ControlFlowGraph {
  vector<Node> graph;
  unsigned nodeCount;

  void GenerateControlFlowGraph(const vector<pair<int, int>> &basic_blocks,
                                const vector<vector<string>> &tree,
                                const unordered_set<string> &jmp_labels) {
    unordered_map<string, int> label_to_node;
    nodeCount = basic_blocks.size();
    for (int node = 0; node < basic_blocks.size(); node++) {
      int st = basic_blocks[node].first;
      int end = basic_blocks[node].second;
      graph[node].FillNode(node, st, end, tree);
      if (jmp_labels.find(tree[st][0]) != jmp_labels.end()) {
        label_to_node[tree[st][0]] = node;
      }
      // graph[node].ShowNodeBlockContents();
    }

    for (int node = 0; node < graph.size(); node++) {
      vector<string> end = graph[node].GetEndStatement();
      if ((end[0] == "goto") or (end[0] == "if")) {
        string jlabel = end.back();
        int dec_node = label_to_node[jlabel];
        graph[node].InsertDecendent(dec_node);
        if (end[0] == "goto")
          continue;
      }
      if (node != graph.size() - 1)
        graph[node].InsertDecendent(node + 1);
    }
    ComputePredecessors();
    ViewAdjecentcyList();
    cout << "Showing Pre list\n\n";
    ViewPredecessorList();
  }

  void DepthFirstTraverseCFG(int node, vector<bool> &vis) {
    vis[node] = 1;
    // do something
    graph[node].ShowNodeBlockContents();
    const vector<int> &decendends = graph[node].GetDecendends();
    for (int dec : decendends)
      if (not vis[dec])
        DepthFirstTraverseCFG(dec, vis);
  }

  void performPredecessorComputation(int node, vector<bool> &vis) {
    if (vis[node])
      return;
    vis[node] = true;
    vector<int> &decendends = graph[node].GetDecendends();
    for (int &dec : decendends) {
      graph[dec].InsertPredecessor(node);
      if (not vis[dec])
        performPredecessorComputation(dec, vis);
    }
  }

  void ComputePredecessors() {
    vector<bool> vis(graph.size(), 0);
    for (int i = 0; i < nodeCount; i++)
      performPredecessorComputation(i, vis);
  }

public:
  ControlFlowGraph(const string &irfile) {
    GenerateBasicBlocks obj(irfile);
    const vector<pair<int, int>> &basic_blocks = obj.GetBasicBlocks();
    const vector<vector<string>> &tree = obj.GetAbstractTree();
    const unordered_set<string> &jmp_labels = obj.GetJumpLabels();
    graph.resize(basic_blocks.size());
    GenerateControlFlowGraph(basic_blocks, tree, jmp_labels);
  }

  vector<Node> &GetFlowGraph() { return this->graph; }

  void ViewAdjecentcyList() {
    for (int i = 0; i < graph.size(); i++) {
      cout << i << " : ";
      graph[i].ShowDecendends();
      cout << '\n';
    }
  }

  void ViewPredecessorList() {
    for (int i = 0; i < graph.size(); i++) {
      cout << i << " : ";
      graph[i].ShowPredecessors();
      cout << '\n';
    }
  }

  void DepthTraverseCFG() {
    vector<bool> vis(graph.size(), 0);
    for (int i = 0; i < nodeCount; i++)
      DepthFirstTraverseCFG(i, vis);
  }
};
