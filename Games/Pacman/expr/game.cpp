#include "utils.h"
#include "board.h"
#include "ghost.h"
#include "pellet.h"
#include "pacman.h"
#include "state.h"



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
        GameBoard &board = *state->board;
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
        assert(state->pac->pacLifeStatus() and "Cannot process command for non live Pac...");
        char input;
        cin>>input;
        #ifdef DEBUG 
            cout<<">>> Input : "<<input<<'\n';
        #endif
        switch(input){
            case 'w': {
                state->pac->move(Direction::Up,state);
                break;
            }

            case 's': {
                state->pac->move(Direction::Down,state);
                break;
            }

            case 'd': {
                state->pac->move(Direction::Right,state);
                break;
            }

            case 'a': {
                state->pac->move(Direction::Left,state);
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
        #ifdef _WIN32
            system("cls");
        #elif __linux__
            system("clear");
        #endif
        // TODO : Add rendering of score, lives left.
        state->renderGameState();
    }

    void runGame(){
        // setNonBlocking();
        initalizeGameSimplestPolicy();
        int devBreak = 0;
        while(true and state->pac->pacLifeStatus()){
            getAndProcessInputClassic();
            state->updateGameState();
            render();
            devBreak++;
            // dev break cycle till the exit logic comes
            if(devBreak==10)
                break;
           this_thread::sleep_for(chrono::microseconds(100));
        }
        // restoreBlocking();
    }

};

int main(){
    Pacman *pac = new Pacman(1,2);
    vector<Ghost> ghosts = {Ghost(2,1),Ghost(3,4)};
    GameBoard *board = new GameBoard(6,6);
    GameState *state = new GameState(board, pac,&ghosts);
    Game g(state);
    g.runGame();
}