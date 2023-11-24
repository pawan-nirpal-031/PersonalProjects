#ifndef GHOST
#define GHOST
#include "utils.h"
#include "board.h"
using std::pair;

class Ghost{
private:
    int row;
    int col;

    void resetGhost(GameBoard &board, pair<unsigned,unsigned> position){
        unsigned rowu = position.first;
        unsigned colu = position.second;
        assert((rowu>=0 and rowu<board.getRows()) and (colu>=0 and colu<board.getCols()) and "Invalid Initialization of Ghost, check location...\n");
        row = position.first;
        col = position.second;
        board.updateCell(row,col,cellType::GhostT);
    }

public:
    Ghost(int srow,int scol){
        row = srow;
        col = scol;
    }

    pair<unsigned,unsigned> getPostion() const{
        return std::make_pair(row,col);
    }

    void updatePosition(){

    }
};


#endif