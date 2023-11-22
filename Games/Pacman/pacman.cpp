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

    // Used for non blocking IO.
    static string getInput(){
        string inp;
        cin>>inp;
        return inp;
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

    void move(Direction direction){
        // TODO
    }

    void getUserInputAndProcess(){
        std::chrono::seconds timeout(5);
        std::string answer; //default to maybe
        std::future<std::string> future = std::async(getInput);
        if (future.wait_for(timeout) == std::future_status::ready)
            answer = future.get();
        char input = ' ';
        if(answer=="w" or answer=="W")
            input = 'w';
        else if(answer=="s" or answer=="S")
            input = 's';
        else if(answer=="d" or answer=="D")
            input = 'd';
        else if(answer=="a" or answer=="A")
            input = 'a';
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
        assert((rowu>=0 and rowu<board.getRows()) and (colu>=0 and colu<board.getCols()) and "Invalid Initialization of Ghost, check location...\n");
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
        this->pty = pty;
    }

    pair<unsigned,unsigned> getPostion() const{
        return make_pair(row,col);
    }

    PelletType getType() const {
        return pty;
    }

    void consume(){

    }

    void reset(GameBoard &board, pair<unsigned,unsigned> position, PelletType pty){
        unsigned rowu = position.first;
        unsigned colu = position.second;
        assert((rowu>=0 and rowu<board.getRows()) and (colu>=0 and colu<board.getCols()) and "Invalid Initialization of Pellet, check location...\n");
        row = position.first;
        col = position.second;
        this->pty = pty;
    }
};

class Game{
private:
    GameBoard board;
    GameState state;
    Pacman pac;
    Ghost g1;
    Ghost g2;

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

public:
    Game() : board(6,6), state(board), pac(1,2), g1(2,1), g2(3,4)  {
        initalizeGameSimplestPolicy();
    }

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

    void getAndProcessInputClassic(){
        pac.getUserInputAndProcess();
    }

    void getAndProcessInput(){
        char input = getKeyPress();
        switch(input){
            case 'w': {
                pac.move(Direction::Up);
                break;
            }

            case 's': {
                pac.move(Direction::Down);
                break;
            }

            case 'd': {
                pac.move(Direction::Right);
                break;
            }

            case 'a': {
                pac.move(Direction::Left);
                break;
            }

            default:
                assert("unsupported action");
        }
    }

    void updateGameState(){
        state.renderGameState();
    }

    void render(){
        #ifdef _WIN32
            system("cls");
        #elif __linux__
            system("clear");
        #endif
        // TODO : Add rendering of score, lives left.
        state.renderGameState();
    }

    void runGame(){
        // setNonBlocking();
        int devBreak = 0;
        while(true){
            getAndProcessInputClassic();
            updateGameState();
            render();
            devBreak++;
            // dev break cycle till the exit logic comes
            if(devBreak==500)
                break;
            this_thread::sleep_for(chrono::microseconds(100));
        }
        // restoreBlocking();
    }

};

int main(){
    Game g;
    g.runGame();
}