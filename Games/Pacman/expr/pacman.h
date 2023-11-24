#ifndef PACMAN
#define PACMAN
#include "utils.h"
#include "board.h"
using std::vector;
using std::cout;
using std::pair;

class GameState;

class Pacman{
private:
    unsigned row;
    unsigned col;
    unsigned score;
    // Default number of lives
    unsigned lives = 3;
    bool isAlive = true;
    GameState *state;

    void updatePosition(int urow,int ucol) {
        this->row = urow;
        this->col = ucol;
    }

    // A kill request raised when paccy has ran into a ghost or a ghost hunted down the Pac.
    Signal killPacman(){
        assert(lives>=1 && "Cannot be killed without a life...\n");
        if(lives==1){
            lives = 0;
            isAlive = false;
            std::cout<<"GAME OVER :(\n";
            return Signal::GameOver;
        }else{
            // Giving it another life.
            lives-=1;
            isAlive = false;
            std::cout<<"YOU JUST GOT KILLED STARTING OVER, LIFE : "<<lives<<"\n";
            return Signal::StartNextLife;
        }
    }

    void handleUpMovement(GameBoard &board){
        assert(pacLifeStatus() && "Pac not live...\n");
        pair<unsigned,unsigned> currPos = getPostion();
        int currRow = currPos.first;
        int currCol = currPos.second;
        if(currRow==0)
            return;
        char entity = board.getEntityAt(currRow-1,currCol);
        switch(entity){
            // Just move up.
            case(cellType::EmptyT):{
                currRow-=1;
                board.updateCell(currRow,currCol,cellType::PacmanT);
                board.updateCell(currRow+1,currCol,cellType::EmptyT);
                updatePosition(currRow,currCol);
                break;
            }
            
            case(cellType::GhostT):{
                // TODO : Kill Pac and exit the game. Make this more sophisticated.
                Signal s = killPacman();
                if(s==Signal::GameOver){
                    assert(lives==0 and "lives non zero and kill signal");
                    isAlive = false;
                    return;
                }
                isAlive = true;
                break;
            }
            // consume pellet
            case(cellType::PelletT):{
                score+=1;
                currRow-=1;
                board.updateCell(currRow,currCol,cellType::PacmanT);
                board.updateCell(currRow+1,currCol,cellType::EmptyT);
                updatePosition(currRow,currCol);
                break;
            }
            // Consume power pellet.
            case(cellType::PowerPelletT):{
                score+=50;
                currRow-=1;
                board.updateCell(currRow,currCol,cellType::PacmanT);
                board.updateCell(currRow+1,currCol,cellType::EmptyT);
                updatePosition(currRow,currCol);
                break;
            }
            //  Nothing happens
            case(cellType::WallT):{
                break;
            }
            assert("Unexpected request for up movement of pac...\n");
        }
    }

    void handleDownMovement(GameBoard &board){
        assert(pacLifeStatus() && "Pac not live...\n");
        pair<unsigned,unsigned> currPos = getPostion();
        int currRow = currPos.first;
        int currCol = currPos.second;
        // Cannot move down from the last row. TODO : can interpass from last to first row, if valid.
        if(currRow==board.getRows()-1)
            return;
        char entity = board.getEntityAt(currRow+1,currCol);
        switch(entity){
            // Just move up.
            case(cellType::EmptyT):{
                currRow+=1;
                board.updateCell(currRow,currCol,cellType::PacmanT);
                board.updateCell(currRow-1,currCol,cellType::EmptyT);
                updatePosition(currRow,currCol);
                break;
            }
            
            case(cellType::GhostT):{
                // TODO : Kill Pac and exit the game. Make this more sophisticated.
                Signal s = killPacman();
                if(s==Signal::GameOver){
                    assert(lives==0 and "lives non zero and kill signal");
                    isAlive = false;
                    return;
                }
                isAlive = true;
                break;
            }
            // consume pellet
            case(cellType::PelletT):{
                score+=1;
                currRow+=1;
                board.updateCell(currRow,currCol,cellType::PacmanT);
                board.updateCell(currRow-1,currCol,cellType::EmptyT);
                updatePosition(currRow,currCol);
                break;
            }
            // Consume power pellet.
            case(cellType::PowerPelletT):{
                score+=50;
                currRow+=1;
                board.updateCell(currRow,currCol,cellType::PacmanT);
                board.updateCell(currRow-1,currCol,cellType::EmptyT);
                updatePosition(currRow,currCol);
                break;
            }
            //  Nothing happens
            case(cellType::WallT):{
                break;
            }
            assert("Unexpected request for down movement of pac...\n");
        }
    }

    void handleLeftMovement(GameBoard &board){
        assert(pacLifeStatus() && "Pac not live...\n");
        pair<unsigned,unsigned> currPos = getPostion();
        int currRow = currPos.first;
        int currCol = currPos.second;
        // already at first col cannot move furthur leftwards
        if(currCol==0)
            return;
        char entity = board.getEntityAt(currCol-1,currRow);
        switch(entity){
            case(cellType::EmptyT):{
                currCol-=1;
                board.updateCell(currRow,currCol,cellType::PacmanT);
                board.updateCell(currRow,currCol+1,cellType::EmptyT);
                updatePosition(currRow,currCol);
                break;
            }
            
            case(cellType::GhostT):{
                // TODO : Kill Pac and exit the game. Make this more sophisticated.
                Signal s = killPacman();
               if(s==Signal::GameOver){
                    assert(lives==0 and "lives non zero and kill signal");
                    isAlive = false;
                    return;
                }
                isAlive = true;
                break;
            }
            // consume pellet
            case(cellType::PelletT):{
                score+=1;
                currCol-=1;
                board.updateCell(currRow,currCol,cellType::PacmanT);
                board.updateCell(currRow,currCol+1,cellType::EmptyT);
                updatePosition(currRow,currCol);
                break;
            }
            // Consume power pellet.
            case(cellType::PowerPelletT):{
                score+=50;
                currCol-=1;
                board.updateCell(currRow,currCol,cellType::PacmanT);
                board.updateCell(currRow,currCol+1,cellType::EmptyT);
                updatePosition(currRow,currCol);
                break;
            }
            //  Nothing happens
            case(cellType::WallT):{
                break;
            }
            assert("Unexpected request for left movement of pac...\n");
        }
    }

    void handleRightMovement(GameBoard &board){
        assert(pacLifeStatus() && "Pac not live...\n");
        pair<unsigned,unsigned> currPos = getPostion();
        int currRow = currPos.first;
        int currCol = currPos.second;
        // already at last col cannot move furthur rightwards
        if(currCol==board.getCols()-1)
            return;
        char entity = board.getEntityAt(currCol+1,currRow);
        switch(entity){
            case(cellType::EmptyT):{
                currCol+=1;
                board.updateCell(currRow,currCol,cellType::PacmanT);
                board.updateCell(currRow,currCol-1,cellType::EmptyT);
                updatePosition(currRow,currCol);
                break;
            }
            
            case(cellType::GhostT):{
                // TODO : Kill Pac and exit the game. Make this more sophisticated.
                Signal s = killPacman();
                if(s==Signal::GameOver){
                    assert(lives==0 and "lives non zero and kill signal");
                    isAlive = false;
                    return;
                }
                isAlive = true;
                break;
            }
            // consume pellet
            case(cellType::PelletT):{
                score+=1;
                currCol+=1;
                board.updateCell(currRow,currCol,cellType::PacmanT);
                board.updateCell(currRow,currCol-1,cellType::EmptyT);
                updatePosition(currRow,currCol);
                break;
            }
            // Consume power pellet.
            case(cellType::PowerPelletT):{
                score+=50;
                currCol+=1;
                board.updateCell(currRow,currCol,cellType::PacmanT);
                board.updateCell(currRow,currCol-1,cellType::EmptyT);
                updatePosition(currRow,currCol);
                break;
            }
            //  Nothing happens
            case(cellType::WallT):{
                break;
            }
            assert("Unexpected request for right movement of pac...\n");
        }
    }

public:
    Pacman(int strow,int stcol){
        row = strow;
        col = stcol;
        score = 0;
    }

    pair<unsigned,unsigned> getPostion() const{
        return std::make_pair(row,col);
    }

    unsigned int getScore() const {
        return score;
    }

    bool pacLifeStatus(){
        return (isAlive && lives>=1);
    }

    unsigned lifeLeftCount() {
        return lives;
    }
    // Pass GameState as the GameContext and let this Object flow through code as a base context.
    void move(Direction direction, GameState *state){
        assert(state->pac->pacLifeStatus() && "Pac no longer live...\n");
        switch(direction){
            case(Direction::Up):{
                handleUpMovement(*state->board);
                break;
            }
            case(Direction::Down):{
                handleDownMovement(*state->board);
                break;
            }
            case(Direction::Left):{
                handleLeftMovement(*state->board);
                break;
            }
            case(Direction::Right):{
                handleRightMovement(*state->board);
                break;
            }
        }
    }

    void resetPac(GameBoard &board, pair<unsigned,unsigned> position){
        unsigned rowu = position.first;
        unsigned colu = position.second;
        assert((rowu>=0 and rowu<board.getRows()) and (colu>=0 and colu<board.getCols()) and "Invalid Initialization of Pacman, check location...\n");
        row = rowu;
        col = colu;
        board.updateCell(row,col,cellType::PacmanT);
    }

    void resetScore(){
        score = 0;
    }
};

#endif