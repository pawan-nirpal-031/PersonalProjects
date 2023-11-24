#ifndef PELLET 
#define PELLET
#include "utils.h"
using std::pair;


class Pellet {
private:
    int row;
    int col;
    PelletType pty;
public:
    
    Pellet(int row,int col, PelletType pty){
        this->row = row;
        this->col = col;
        this->pty = pty;
    }

    pair<unsigned,unsigned> getPostion() const{
        return std::make_pair(row,col);
    }

    PelletType getType() const {
        return pty;
    }
};

#endif