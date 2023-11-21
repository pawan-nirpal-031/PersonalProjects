#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;

enum cellType{
    EmptyT,
    WallT,
    PelletT,
    PowerPelletT,
    PacmanT,
    GhostT
};

enum class PelletType {
    Regular,
    Power
};

enum class Direction{
    Up,
    Down,
    Left,
    Right
};

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

class GameState{
private:
    GameBoard board;
public:
    GameState(const GameBoard &gboard) : board(gboard) {}
    
    void renderGameState() const {
        board.renderBoard();
    }

    // TODO
    void updateGameState(){

    }

};

class Pacman{
private:
    int row;
    int col;
    int score;
public:
    Pacman(int strow,int stcol){
        row = strow;
        col = stcol;
        score = 0;
    }

    pair<unsigned,unsigned> getPostion() const{
        return make_pair(row,col);
    }

    unsigned int getScore() const {
        return score;
    }

    void move(Direction direction){
        // TODO
    }

    void userInput(){
        char input;
        cin>>input;

        switch(input){
            case 'w': {
                move(Direction::Up);
                break;
            }

            case 's': {
                move(Direction::Down);
                break;
            }

            case 'd': {
                move(Direction::Right);
                break;
            }

            case 'a': {
                move(Direction::Left);
                break;
            }

            default:
                assert("unsupported action");
        }
    }

    void resetPac(GameBoard &board, pair<unsigned,unsigned> position){
        unsigned rowu = position.first;
        unsigned colu = position.second;
        assert((rowu>=0 and rowu<board.getRows()) and (colu>=0 and colu<board.getCols()) and "Invalid Initialization of Pacman...\n");
        row = rowu;
        col = colu;
        board.updateCell(row,col,cellType::PacmanT);
    }

    void resetScore(){
        score = 0;
    }
};

class Ghost{
private:
    int row;
    int col;
public:
    Ghost(int srow,int scol){
        row = srow;
        col = scol;
    }

    pair<unsigned,unsigned> getPostion() const{
        return make_pair(row,col);
    }

    void updatePosition(){

    }

    void resetGhost(GameBoard &board, pair<unsigned,unsigned> position){
        unsigned rowu = position.first;
        unsigned colu = position.second;
        assert((rowu>=0 and rowu<board.getRows()) and (colu>=0 and colu<board.getCols()) and "Invalid Initialization of Ghost...\n");
        row = position.first;
        col = position.second;
        board.updateCell(row,col,cellType::GhostT);
    }
};

class Pellet {
private:
    int row;
    int col;
    PelletType pty;
public:
    
    Pellet(int row,int col, PelletType pty){
        this->row = row;
        this->col = col;
    }

    pair<unsigned,unsigned> getPostion() const{
        return make_pair(row,col);
    }

    PelletType getType() const {
        return pty;
    }

    void consume(){

    }

    void reset(pair<unsigned,unsigned> position, PelletType pty){
        row = position.first;
        col = position.second;
        this->pty = pty;
    }
};

void initalizeGameSimplestPolicy(){
    GameBoard board(6,6);
    for(int i =0;i<6;i++){
        board.updateCell(0,i,cellType::WallT);
        board.updateCell(5,i,cellType::WallT);
        board.updateCell(i,0,cellType::WallT);
        board.updateCell(i,5,cellType::WallT);
    }
    board.updateCell(2,2,cellType::WallT);
    board.updateCell(2,3,cellType::WallT);
    board.updateCell(3,2,cellType::WallT);
    board.updateCell(3,3,cellType::WallT);
    board.updateCell(1,1,cellType::PelletT);
    board.updateCell(1,4,cellType::PelletT);
    board.updateCell(4,1,cellType::PelletT);
    board.updateCell(4,4,cellType::PelletT);
    Pacman pac(1,2);
    pac.resetPac(board,pac.getPostion());
    board.updateCell(2,1,cellType::GhostT);
    board.updateCell(3,4,cellType::GhostT);
    for(int i =0;i<6;i++){
        for(int j =0;j<6;j++)
            if(board.getEntityAt(i,j)==cellType::EmptyT)
                board.updateCell(i,j,cellType::PelletT);
    }
    board.renderBoard();
}


int main(){
   initalizeGameSimplestPolicy();
}