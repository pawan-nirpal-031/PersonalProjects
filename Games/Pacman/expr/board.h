#ifndef BOARD
#define BOARD

#include "utils.h"
using std::vector;
using std::cout;

class GameBoard{
private:
    int rows;
    int cols;
    vector<vector<cellType>> board;
    bool hasBeenInitalized = false;

    char getCellCharByCellType(cellType Cty) const {
        switch (Cty){
            case cellType::EmptyT: 
                return ' ';
            case cellType::WallT:
                return '#';
            case cellType::PacmanT:
                return 'P';
            case cellType::PelletT:
                return '.';
            case cellType::PowerPelletT:
                return 'O';
            case cellType::GhostT:
                return 'G';
            default:
                assert("unknown cell type!!");
        }
        assert("unknown cell type!!");
        return ' ';
    }

public:
    GameBoard(int rows,int cols){
        hasBeenInitalized = true;
        this->rows = rows;
        this->cols = cols;
        board.resize(rows,vector<cellType>(cols,cellType::EmptyT));
    }

    void renderBoard() const {
        for(int i =0;i<rows;i++){
            for(int j =0;j<cols;j++){
                cout<<getCellCharByCellType(board[i][j])<<' ';
            }
            cout<<'\n';
        }
    }

    void updateCell(int row,int col,cellType cty){
        board[row][col] = cty;
    }

    char getEntityAt(int row,int col){
        assert(hasBeenInitalized && "Board was never Initalized...\n");
        return board[row][col];
    }

    unsigned getRows(){
        assert(hasBeenInitalized && "Board was never Initalized...\n");
        return rows;
    }

    unsigned getCols(){
        assert(hasBeenInitalized && "Board was never Initalized...\n");
        return cols;
    }
};


#endif