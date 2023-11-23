#include <iostream>
#include <vector>
#include <assert.h>
#include <chrono>
#include <cstdio>
#include <future>
#include <thread>
#ifdef _WIN32
#include <conio.h>
#elif __linux__
#include <termios.h>
#include <unistd.h>
#endif
#define DEBUG true

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

// Forward delcn.
class GameState;
class GameBoard;
class Pacman;
class Ghost;
class Pellet;
class Game;
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

// The GameState represents the current game context, it encompasses Board, the Pacman and the ghosts. 
// Control the core game logic from this game state and also render the board. The Board does not have objects on it, it only has textual representation of the enity/object like Pac or ghost or pellet. 
class GameState{
private:
public:
    GameBoard board;
    bool isPacLive = true;
    Pacman *pac;
    vector<Ghost> ghosts;

    GameState(GameBoard gboard, Pacman *spac, vector<Ghost>sghosts) : board(gboard), pac(spac), ghosts(sghosts) {}
    
    void renderGameState() {
        this->board.renderBoard();
        cout<<"\n\n\n";
    }

    // TODO
    void updateGameState(){
        //this->renderGameState();
    }

};

class Pacman{
private:
    unsigned row;
    unsigned col;
    unsigned score;
    // Default number of lives
    unsigned lives = 3;
    bool isAlive = true;

    void updatePosition(int urow,int ucol) {
        this->row = urow;
        this->col = ucol;
    }

    bool pacLifeStatus(){
        return (isAlive && lives>=1);
    }

    // A kill request raised when paccy has ran into a ghost or a ghost hunted down the Pac.
    void killPacman(){
        assert(lives>=1 && "Cannot be killed without a life...\n");
        if(lives==1){
            lives = 0;
            isAlive = false;
            cout<<"GAME OVER :(";
            exit(0);
        }else{
            // Giving it another life.
            lives-=1;
            isAlive = true;
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
                killPacman();
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
                killPacman();
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
            assert("Unexpected request for up movement of pac...\n");
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
                killPacman();
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
            assert("Unexpected request for up movement of pac...\n");
        }
    }

    void handleRightMovement(GameBoard &board){
        assert(pacLifeStatus() && "Pac not live...\n");
        pair<unsigned,unsigned> currPos = getPostion();
        int currRow = currPos.first;
        int currCol = currPos.second;
        // already at last col cannot move furthur rightwards
        if(currCol==board.getCols())
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
                killPacman();
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
            assert("Unexpected request for up movement of pac...\n");
        }
    }

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

    // Pass GameState as the GameContext and let this Object flow through code as a base context.
    void move(Direction direction, GameState state){
        switch(direction){
            case(Direction::Up):{
                handleUpMovement(state.board);
                break;
            }
            case(Direction::Down):{
                handleDownMovement(state.board);
                break;
            }
            case(Direction::Left):{
                handleLeftMovement(state.board);
                break;
            }
            case(Direction::Right):{
                handleRightMovement(state.board);
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
        return make_pair(row,col);
    }

    void updatePosition(){

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
        this->pty = pty;
    }

    pair<unsigned,unsigned> getPostion() const{
        return make_pair(row,col);
    }

    PelletType getType() const {
        return pty;
    }
};

/// @brief A Game only has a game state and ability to update it, Initalize it. 
class Game{
private:
    GameState *state;
    
    void setNonBlocking() {
    #ifdef _WIN32
        _setmode(_fileno(stdin), _O_TEXT);
    #elif __linux__
        struct termios ttystate;
        tcgetattr(STDIN_FILENO, &ttystate);
        ttystate.c_lflag &= ~(ICANON | ECHO);
        ttystate.c_cc[VMIN] = 0;
        ttystate.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    #endif
    }

    // Function to restore blocking mode for standard input
    void restoreBlocking() {
    #ifdef _WIN32
        _setmode(_fileno(stdin), _O_TEXT);
    #elif __linux__
        struct termios ttystate;
        tcgetattr(STDIN_FILENO, &ttystate);
        ttystate.c_lflag |= ICANON | ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    #endif
    }

    // Function to check if a key is pressed
    bool keyIsPressed() {
    #ifdef _WIN32
        return _kbhit() != 0;
    #elif __linux__
        fd_set fds;
        struct timeval timeout;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        return select(STDIN_FILENO + 1, &fds, NULL, NULL, &timeout) == 1;
    #endif
    }

    // Function to get a non-blocking key press
    char getKeyPress() {
        char ch = '\0';
        if (keyIsPressed()) {
            cin >> ch;
        }
        return ch;
    }

    // Used for non blocking IO.
    static string getInput(){
        string inp;
        cin>>inp;
        return inp;
    }

    char getUserInput(){
        std::chrono::seconds timeout(5);
        std::string answer; //default to maybe
        std::future<std::string> future = std::async(getInput);
        if (future.wait_for(timeout) == std::future_status::ready)
            answer = future.get();
        char input = ' ';
        if(answer=="w" or answer=="W")
            return 'w';
        else if(answer=="s" or answer=="S")
            return 's';
        else if(answer=="d" or answer=="D")
            return 'd';
        else if(answer=="a" or answer=="A")
            return 'a';
        cout<<"Please enter valid movement...\n";
        return input;
    }


public:
    Game(GameState *sstate){
        state = sstate;
    }

    void initalizeGameSimplestPolicy(){
        GameBoard &board = state->board;
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
        Pacman *pac = state->pac;
        pac->resetPac(board,pac->getPostion());
        board.updateCell(2,1,cellType::GhostT);
        board.updateCell(3,4,cellType::GhostT);
        for(int i =0;i<6;i++){
            for(int j =0;j<6;j++)
                if(board.getEntityAt(i,j)==cellType::EmptyT)
                    board.updateCell(i,j,cellType::PelletT);
        }
        board.renderBoard();
    }

    void getAndProcessInputClassic(){
        char input;
        cin>>input;
        #ifdef DEBUG 
            cout<<">>> Input : "<<input<<'\n';
        #endif
        switch(input){
            case 'w': {
                #ifdef DEBUG 
                    cout<<">>> Moving Up: "<<'\n';
                #endif
                state->pac->move(Direction::Up,*state);
                break;
            }

            case 's': {
                #ifdef DEBUG 
                    cout<<">>> Moving Down: "<<'\n';
                #endif
                state->pac->move(Direction::Down,*state);
                break;
            }

            case 'd': {
                #ifdef DEBUG 
                    cout<<">>> Moving Left: "<<'\n';
                #endif
                state->pac->move(Direction::Right,*state);
                break;
            }

            case 'a': {
                #ifdef DEBUG 
                    cout<<">>> Moving Right: "<<'\n';
                #endif
                state->pac->move(Direction::Left,*state);
                break;
            }

            default:
                assert("unsupported action");
        }
    }

    // void getAndProcessInput(){
    //     char input = getKeyPress();
    //     switch(input){
    //         case 'w': {
    //             pac.move(Direction::Up,board);
    //             break;
    //         }

    //         case 's': {
    //             pac.move(Direction::Down,board);
    //             break;
    //         }

    //         case 'd': {
    //             pac.move(Direction::Right,board);
    //             break;
    //         }

    //         case 'a': {
    //             pac.move(Direction::Left,board);
    //             break;
    //         }

    //         default:
    //             assert("unsupported action");
    //     }
    // }

    void updateGameState(){

    }

    void render(){
        // #ifdef _WIN32
        //     system("cls");
        // #elif __linux__
        //     system("clear");
        // #endif
        // TODO : Add rendering of score, lives left.
        state->renderGameState();
    }

    void runGame(){
        // setNonBlocking();
        initalizeGameSimplestPolicy();
        int devBreak = 0;
        while(true){
            getAndProcessInputClassic();
            state->updateGameState();
            render();
            devBreak++;
            // dev break cycle till the exit logic comes
            if(devBreak==4)
                break;
           this_thread::sleep_for(chrono::microseconds(100));
        }
        // restoreBlocking();
    }

};

int main(){
    Pacman *pac = new Pacman(1,2);
    GameState *state = new GameState(GameBoard(6,6), pac,{Ghost(2,1),Ghost(3,4)});
    Game g(state);
    g.runGame();
}