#include <stack>
#include <math.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;

unsigned cols = 20;
unsigned rows = 10;

enum {
    PATH_N = 0x01,
    PATH_E = 0x02,
    PATH_S = 0x04,
    PATH_W = 0x08,
    VISITED = 0x10,
};

enum CellType { EmptyT, WallT, PelletT, PowerPelletT, PacmanT, GhostT };

char getCellCharByCellType(CellType Cty) {
    switch (Cty) {
    case CellType::EmptyT:
      return ' ';
    case CellType::WallT:
      return '#';
    case CellType::PacmanT:
      return 'P';
    case CellType::PelletT:
      return '.';
    case CellType::PowerPelletT:
      return 'O';
    case CellType::GhostT:
      return 'G';
    default:
      assert("unknown cell type!!");
    }
    assert("unknown cell type!!");
    return ' ';
  }

unsigned visitedCount = 0;
stack<pair<int,int>> process;

void renderBoard(vector<vector<CellType>> &board) {
    for (int i = 0; i < rows; i++) {
        for (int i = 0; i < 7; i++)
          cout << " ";
        for (int j = 0; j < cols; j++) {
          cout<<getCellCharByCellType(board[i][j]);
        }
        cout << '\n';
    }
}

void generateMaze(vector<vector<CellType>> &board,vector<vector<unsigned>> &mazeTracker){
    for(int i =0;i<rows;i++){
        for(int j =0;j<cols;j++){
            if(mazeTracker[i][j])
                board[i][j] = CellType::WallT;
            
        }
    }
}

int main(){
    vector<vector<CellType>> board(rows,vector<CellType>(cols,CellType::EmptyT));
    vector<vector<unsigned>> mazeTracker(rows,vector<unsigned>(cols,0));
    process.push(make_pair(0,0));
    board[0][0] = CellType::WallT;
    mazeTracker[0][0] = 1;
    visitedCount = 1;
    generateMaze(board,mazeTracker);
    renderBoard(board);

    return 0;
}


/*
g++ -g -O3 maze.cpp   -o maze

*/