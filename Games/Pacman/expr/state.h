#ifndef STATE
#define STATE
#include "utils.h"
#include "pacman.h"
#include "ghost.h"

// The GameState represents the current game context, it encompasses Board, the Pacman and the ghosts. 
// Control the core game logic from this game state and also render the board. The Board does not have objects on it, it only has textual representation of the enity/object like Pac or ghost or pellet. 
class GameState{
private:
public:
    GameBoard *board;
    Pacman *pac;
    vector<Ghost> *ghosts;

   // GameState(GameBoard gboard, Pacman *spac, vector<Ghost>sghosts) : board(gboard), pac(spac), ghosts(sghosts) {}
    GameState(GameBoard *gboard, Pacman *spac, vector<Ghost> *sghosts) {
        ghosts = sghosts;
        pac = spac;
        board = gboard;
    }

    
    void renderGameState() {
        auto pos = pac->getPostion();
        cout<<"LIVES LEFT : "<<pac->lifeLeftCount()<<" | X = "<<pos.first<<" Y = "<<pos.second<<" |  Score : "<<pac->getScore()<<"\n";
        this->board->renderBoard();
        cout<<"\n\n\n";
    }

    // TODO
    void updateGameState(){
        //this->renderGameState();
    }

};


#endif