#include <assert.h>
#include <chrono>
#include <cstdio>
#include <future>
#include <iostream>
#include <queue>
#include <thread>
#include <vector>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#ifdef _WIN32
#include <conio.h>
#elif __linux__
#include <termios.h>
#include <unistd.h>
#endif
#define DEBUG false

using namespace std;

enum CellType { EmptyT, WallT, PelletT, PowerPelletT, PacmanT, GhostT };

enum class PelletType { Regular, Power };

enum class Direction { Up, Down, Left, Right };

enum class Signal { GameOver, StartNextLife };

// Forward delcn.
class GameState;
class GameBoard;
class Pacman;
class Ghost;
class Pellet;
class Game;
class GameBoard {
private:
  int rows;
  int cols;
  vector<vector<CellType>> board;
  bool hasBeenInitalized = false;


  char getCellCharByCellType(CellType Cty) const {
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

public:
   void drawCell(CellType cty) const{
    cout<<"|"<<getCellCharByCellType(cty)<<"|";
  }
  // Left, Dowm, Right, Up
  vector<pair<int, int>> Visit4Neighbours = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
  GameBoard(int rows, int cols) {
    hasBeenInitalized = true;
    this->rows = rows;
    this->cols = cols;
    board.resize(rows, vector<CellType>(cols, CellType::EmptyT));
  }

  void renderBoard() const {
    for (int i = 0; i < rows; i++) {
      for (int i = 0; i < 7; i++)
        cout << " ";
      for (int j = 0; j < cols; j++) {
        drawCell(board[i][j]);
      }
      cout << '\n';
    }
  }

  bool isValidCell(int x, int y) {
    assert(hasBeenInitalized and "Board was never Initalized...\n");
    return (x >= 0 and x < rows and y >= 0 and y < cols);
  }

  void updateCell(int row, int col, CellType cty) { board[row][col] = cty; }

  CellType getEntityAt(int row, int col) {
    assert(hasBeenInitalized and "Board was never Initalized...\n");
    return board[row][col];
  }

  unsigned getRows() {
    assert(hasBeenInitalized and "Board was never Initalized...\n");
    return rows;
  }

  unsigned getCols() {
    assert(hasBeenInitalized and "Board was never Initalized...\n");
    return cols;
  }
};

// The GameState represents the current game context, it encompasses Board, the
// Pacman and the ghosts. Control the core game logic from this game state and
// also render the board. The Board does not have objects on it, it only has
// textual representation of the enity/object like Pac or ghost or pellet.
class GameState {
private:
public:
  GameBoard *board;
  Pacman *pac;
  vector<Ghost> *ghosts;

  // GameState(GameBoard gboard, Pacman *spac, vector<Ghost>sghosts) :
  // board(gboard), pac(spac), ghosts(sghosts) {}
  GameState(GameBoard *gboard, Pacman *spac, vector<Ghost> *sghosts) {
    ghosts = sghosts;
    pac = spac;
    board = gboard;
  }

  void renderGameState();

  // TODO
  void updateGameState();
};

class Pacman {
private:
  unsigned row;
  unsigned col;
  unsigned score;
  // Default number of lives
  unsigned lives = 3;
  bool isAlive = true;

  void updatePosition(int urow, int ucol) {
    this->row = urow;
    this->col = ucol;
  }

  void handleUpMovement(GameBoard &board) {
    assert(pacLifeStatus() && "Pac not live...\n");
    pair<unsigned, unsigned> currPos = getPostion();
    int currRow = currPos.first;
    int currCol = currPos.second;
    if (currRow == 0)
      return;
    CellType entity = board.getEntityAt(currRow - 1, currCol);
    switch (entity) {
    // Just move up.
    case (CellType::EmptyT): {
      currRow -= 1;
      board.updateCell(currRow, currCol, CellType::PacmanT);
      board.updateCell(currRow + 1, currCol, CellType::EmptyT);
      updatePosition(currRow, currCol);
      break;
    }

    //  Nothing happens
    case (CellType::WallT): {
      break;
    }

    // consume pellet
    case (CellType::PelletT): {
      score += 1;
      currRow -= 1;
      board.updateCell(currRow, currCol, CellType::PacmanT);
      board.updateCell(currRow + 1, currCol, CellType::EmptyT);
      updatePosition(currRow, currCol);
      break;
    }

    // Consume power pellet.
    case (CellType::PowerPelletT): {
      score += 50;
      currRow -= 1;
      board.updateCell(currRow, currCol, CellType::PacmanT);
      board.updateCell(currRow + 1, currCol, CellType::EmptyT);
      updatePosition(currRow, currCol);
      break;
    }

    case (CellType::GhostT): {
      // TODO : Kill Pac and exit the game. Make this more sophisticated.
      Signal s = killPacman();
      if (s == Signal::GameOver) {
        assert(lives == 0 and "lives non zero and kill signal");
        isAlive = false;
        return;
      }
      isAlive = true;
      break;
    }

      assert("Unexpected request for up movement of pac...\n");
    }
  }

  void handleDownMovement(GameBoard &board) {
    assert(pacLifeStatus() && "Pac not live...\n");
    pair<unsigned, unsigned> currPos = getPostion();
    int currRow = currPos.first;
    int currCol = currPos.second;
    // Cannot move down from the last row. TODO : can interpass from last to
    // first row, if valid.
    if (currRow == board.getRows() - 1)
      return;
    CellType entity = board.getEntityAt(currRow + 1, currCol);
    switch (entity) {
    // Just move up.
    case (CellType::EmptyT): {
      currRow += 1;
      board.updateCell(currRow, currCol, CellType::PacmanT);
      board.updateCell(currRow - 1, currCol, CellType::EmptyT);
      updatePosition(currRow, currCol);
      break;
    }

    //  Nothing happens
    case (CellType::WallT): {
      break;
    }

    // consume pellet
    case (CellType::PelletT): {
      score += 1;
      currRow += 1;
      board.updateCell(currRow, currCol, CellType::PacmanT);
      board.updateCell(currRow - 1, currCol, CellType::EmptyT);
      updatePosition(currRow, currCol);
      break;
    }
    // Consume power pellet.
    case (CellType::PowerPelletT): {
      score += 50;
      currRow += 1;
      board.updateCell(currRow, currCol, CellType::PacmanT);
      board.updateCell(currRow - 1, currCol, CellType::EmptyT);
      updatePosition(currRow, currCol);
      break;
    }

    case (CellType::GhostT): {
      // TODO : Kill Pac and exit the game. Make this more sophisticated.
      Signal s = killPacman();
      if (s == Signal::GameOver) {
        assert(lives == 0 and "lives non zero and kill signal");
        isAlive = false;
        return;
      }
      isAlive = true;
      break;
    }

      assert("Unexpected request for down movement of pac...\n");
    }
  }

  void handleLeftMovement(GameBoard &board) {
    assert(pacLifeStatus() && "Pac not live...\n");
    pair<unsigned, unsigned> currPos = getPostion();
    int currRow = currPos.first;
    int currCol = currPos.second;
    // already at first col cannot move furthur leftwards
    if (currCol == 0)
      return;
    CellType entity = board.getEntityAt(currRow, currCol - 1);
    switch (entity) {
    case (CellType::EmptyT): {
      currCol -= 1;
      board.updateCell(currRow, currCol, CellType::PacmanT);
      board.updateCell(currRow, currCol + 1, CellType::EmptyT);
      updatePosition(currRow, currCol);
      break;
    }

    //  Nothing happens
    case (CellType::WallT): {
      break;
    }

    // consume pellet
    case (CellType::PelletT): {
      score += 1;
      currCol -= 1;
      board.updateCell(currRow, currCol, CellType::PacmanT);
      board.updateCell(currRow, currCol + 1, CellType::EmptyT);
      updatePosition(currRow, currCol);
      break;
    }

    // Consume power pellet.
    case (CellType::PowerPelletT): {
      score += 50;
      currCol -= 1;
      board.updateCell(currRow, currCol, CellType::PacmanT);
      board.updateCell(currRow, currCol + 1, CellType::EmptyT);
      updatePosition(currRow, currCol);
      break;
    }

    case (CellType::GhostT): {
      // TODO : Kill Pac and exit the game. Make this more sophisticated.
      Signal s = killPacman();
      if (s == Signal::GameOver) {
        assert(lives == 0 and "lives non zero and kill signal");
        isAlive = false;
        return;
      }
      isAlive = true;
      break;
    }

      assert("Unexpected request for left movement of pac...\n");
    }
  }

  void handleRightMovement(GameBoard &board) {
    assert(pacLifeStatus() && "Pac not live...\n");
    pair<unsigned, unsigned> currPos = getPostion();
    int currRow = currPos.first;
    int currCol = currPos.second;
    // already at last col cannot move furthur rightwards
    if (currCol == board.getCols() - 1)
      return;
    CellType entity = board.getEntityAt(currRow, currCol + 1);
    switch (entity) {
    case (CellType::EmptyT): {
      currCol += 1;
      board.updateCell(currRow, currCol, CellType::PacmanT);
      board.updateCell(currRow, currCol - 1, CellType::EmptyT);
      updatePosition(currRow, currCol);
      break;
    }

      //  Nothing happens
    case (CellType::WallT): {
      break;
    }

    // consume pellet
    case (CellType::PelletT): {
      score += 1;
      currCol += 1;
      board.updateCell(currRow, currCol, CellType::PacmanT);
      board.updateCell(currRow, currCol - 1, CellType::EmptyT);
      updatePosition(currRow, currCol);
      break;
    }

    // Consume power pellet.
    case (CellType::PowerPelletT): {
      score += 50;
      currCol += 1;
      board.updateCell(currRow, currCol, CellType::PacmanT);
      board.updateCell(currRow, currCol - 1, CellType::EmptyT);
      updatePosition(currRow, currCol);
      break;
    }

    case (CellType::GhostT): {
      // TODO : Kill Pac and exit the game. Make this more sophisticated.
      Signal s = killPacman();
      if (s == Signal::GameOver) {
        assert(lives == 0 and "lives non zero and kill signal");
        isAlive = false;
        return;
      }
      isAlive = true;
      break;
    }

      assert("Unexpected request for right movement of pac...\n");
    }
  }

public:
  Pacman(int strow, int stcol) {
    row = strow;
    col = stcol;
    score = 0;
  }

  pair<unsigned, unsigned> getPostion() const { return make_pair(row, col); }

  unsigned int getScore() const { return score; }

  bool pacLifeStatus() { return (isAlive && lives >= 1); }

  void updateAliveStatus(bool status) { this->isAlive = status; }

  // A kill request raised when paccy has ran into a ghost or a ghost hunted
  // down the Pac.
  Signal killPacman() {
    assert(lives >= 1 && "Cannot be killed without a life...\n");
    if (lives == 1) {
      lives = 0;
      isAlive = false;
      cout << "GAME OVER :(\n";
      return Signal::GameOver;
    } else {
      // Giving it another life.
      lives -= 1;
      isAlive = false;
      cout << "YOU JUST GOT KILLED STARTING OVER, LIFE : " << lives << "\n";
      return Signal::StartNextLife;
    }
  }

  unsigned lifeLeftCount() { return lives; }
  // Pass GameState as the GameContext and let this Object flow through code as
  // a base context.
  void move(Direction direction, GameState *state) {
    assert(state->pac->pacLifeStatus() && "Pac no longer live...\n");
    switch (direction) {
    case (Direction::Up): {
      handleUpMovement(*state->board);
      break;
    }
    case (Direction::Down): {
      handleDownMovement(*state->board);
      break;
    }
    case (Direction::Left): {
      handleLeftMovement(*state->board);
      break;
    }
    case (Direction::Right): {
      handleRightMovement(*state->board);
      break;
    }
    }
  }

  void resetPac(GameBoard &board, pair<unsigned, unsigned> position) {
    unsigned rowu = position.first;
    unsigned colu = position.second;
    assert((rowu >= 0 and rowu < board.getRows()) and
           (colu >= 0 and colu < board.getCols()) and
           "Invalid Initialization of Pacman, check location...\n");
    row = rowu;
    col = colu;
    board.updateCell(row, col, CellType::PacmanT);
  }

  void resetScore() { score = 0; }
};

class Ghost {
private:
  int row;
  int col;

  void resetGhost(GameBoard &board, pair<unsigned, unsigned> position) {
    unsigned rowu = position.first;
    unsigned colu = position.second;
    assert((rowu >= 0 and rowu < board.getRows()) and
           (colu >= 0 and colu < board.getCols()) and
           "Invalid Initialization of Ghost, check location...\n");
    row = position.first;
    col = position.second;
    board.updateCell(row, col, CellType::GhostT);
  }

  void KillPacman(GameState *state) {
    // IF Dead already then return.
    if (state->pac->pacLifeStatus() == false)
      return;
    Signal signal = state->pac->killPacman();
    if (signal == Signal::GameOver) {
      assert(state->pac->lifeLeftCount() == 0 and
             "lives non zero and kill signal");
      state->pac->updateAliveStatus(false);
      return;
    }
    state->pac->updateAliveStatus(true);
  }

public:
  Ghost(int srow, int scol) {
    row = srow;
    col = scol;
  }

  pair<unsigned, unsigned> getPostion() const { return make_pair(row, col); }

  // Perform a bfs starting at the PAC and annotate the graph, with the Minimum
  // distance to reach that point, if that point in maze was reachable. And this
  // would be unique for each invocation of updation of this ghost position.
  // TODO : This does not seem to be different for every ghost on board, So
  // redoing this computation for each ghost might not be fesible. Unless
  // Multithreading is used, even then board will always be the same for each
  // ghost.
  void updatePositionClosestDistancePolicy(GameState *state) {
    GameBoard &board = *state->board;
    vector<vector<unsigned long>> Distance(
        board.getRows(), vector<unsigned long>(board.getCols(), INT64_MAX));
    vector<vector<bool>> Visited(board.getRows(),
                                 vector<bool>(board.getCols(), false));
    Pacman &pac = *state->pac;
    auto pacPos = pac.getPostion();
    unsigned prow = pacPos.first;
    unsigned pcol = pacPos.second;
    Distance[prow][pcol] = 0;
    Visited[prow][pcol] = true;
    queue<pair<unsigned, unsigned>> Frontier;
    Frontier.push(make_pair(prow, pcol));
    while (not Frontier.empty()) {
      auto currNode = Frontier.front();
      Frontier.pop();
      for (auto &neighbour : board.Visit4Neighbours) {
        int nxtrow = currNode.first + neighbour.first;
        int nxtcol = currNode.second + neighbour.second;
        if (not board.isValidCell(nxtrow, nxtcol))
          continue;
        CellType entity = board.getEntityAt(nxtrow, nxtcol);
        if ((not Visited[nxtrow][nxtcol]) and
            (entity == CellType::GhostT or entity == CellType::EmptyT or
             entity == CellType::PelletT or entity == CellType::PowerPelletT)) {
          Distance[nxtrow][nxtcol] =
              Distance[currNode.first][currNode.second] + 1;
          Frontier.push(make_pair(nxtrow, nxtcol));
        }
      }
      Visited[currNode.first][currNode.second] = true;
    }

    if (Visited[row][col] == false and Distance[row][col] == INT64_MAX)
      cout << "Ghost at : " << row << " , " << col
           << " is unreachable from Pac...\n";
    else {
      unsigned long MinDistFromPac = Distance[row][col];
      if (MinDistFromPac < INT64_MAX) {
        // TODO : move towards pac to hunt him.
        char dirToMove = ' ';
        for (int i = 0; i < 4; i++) {
          auto neighbour = board.Visit4Neighbours[i];
          int nxtrow = row + neighbour.first;
          int nxtcol = col + neighbour.second;
          if (not board.isValidCell(nxtrow, nxtcol))
            continue;
          if (board.getEntityAt(nxtrow, nxtcol) == CellType::WallT or
              board.getEntityAt(nxtrow, nxtcol) == CellType::GhostT)
            continue;
          if (MinDistFromPac == Distance[nxtrow][nxtcol] + 1) {
            if (i == 0)
              dirToMove = 'l';
            else if (i == 1)
              dirToMove = 'd';
            else if (i == 2)
              dirToMove = 'r';
            else
              dirToMove = 'u';
          }
        }
        if (dirToMove != ' ') {
          switch (dirToMove) {
          case ('u'): {
            CellType entity = board.getEntityAt(row - 1, col);
            switch (entity) {
            case (CellType::EmptyT): {
              row -= 1;
              board.updateCell(row, col, CellType::GhostT);
              board.updateCell(row + 1, col, CellType::EmptyT);
              break;
            }
            case (CellType::GhostT): {
              break;
            }
            case (CellType::PacmanT): {
              // TODO : get him
              KillPacman(state);
              break;
            }
            case (CellType::PelletT): {
              row -= 1;
              board.updateCell(row, col, CellType::GhostT);
              board.updateCell(row + 1, col, CellType::PelletT);
              break;
            }
            case (CellType::PowerPelletT): {
              row -= 1;
              board.updateCell(row, col, CellType::GhostT);
              board.updateCell(row + 1, col, CellType::PowerPelletT);
              break;
            }
            }
            break;
          }
          case ('d'): {
            CellType entity = board.getEntityAt(row + 1, col);
            switch (entity) {
            case (CellType::EmptyT): {
              row += 1;
              board.updateCell(row, col, CellType::GhostT);
              board.updateCell(row - 1, col, CellType::EmptyT);
              break;
            }
            case (CellType::GhostT): {
              break;
            }
            case (CellType::PacmanT): {
              // TODO : get him
              KillPacman(state);
              break;
            }
            case (CellType::PelletT): {
              row += 1;
              board.updateCell(row, col, CellType::GhostT);
              board.updateCell(row - 1, col, CellType::PelletT);
              break;
            }
            case (CellType::PowerPelletT): {
              row += 1;
              board.updateCell(row, col, CellType::GhostT);
              board.updateCell(row - 1, col, CellType::PowerPelletT);
              break;
            }
            }
            break;
          }
          case ('r'): {
            CellType entity = board.getEntityAt(row, col + 1);
            switch (entity) {
            case (CellType::EmptyT): {
              col += 1;
              board.updateCell(row, col, CellType::GhostT);
              board.updateCell(row, col - 1, CellType::EmptyT);
              break;
            }
            case (CellType::GhostT): {
              break;
            }
            case (CellType::PacmanT): {
              // TODO : get him
              KillPacman(state);
              break;
            }
            case (CellType::PelletT): {
              col += 1;
              board.updateCell(row, col, CellType::GhostT);
              board.updateCell(row, col - 1, CellType::PelletT);
              break;
            }
            case (CellType::PowerPelletT): {
              col += 1;
              board.updateCell(row, col, CellType::GhostT);
              board.updateCell(row, col - 1, CellType::PowerPelletT);
              break;
            }
            }
            break;
          }
          case ('l'): {
            CellType entity = board.getEntityAt(row, col - 1);
            switch (entity) {
            case (CellType::EmptyT): {
              col -= 1;
              board.updateCell(row, col, CellType::GhostT);
              board.updateCell(row, col + 1, CellType::EmptyT);
              break;
            }
            case (CellType::GhostT): {
              break;
            }
            case (CellType::PacmanT): {
              // TODO : get him
              KillPacman(state);
              break;
            }
            case (CellType::PelletT): {
              col -= 1;
              board.updateCell(row, col, CellType::GhostT);
              board.updateCell(row, col + 1, CellType::PelletT);
              break;
            }
            case (CellType::PowerPelletT): {
              col -= 1;
              board.updateCell(row, col, CellType::GhostT);
              board.updateCell(row, col + 1, CellType::PowerPelletT);
              break;
            }
            }
          } break;
          }
        }
      } else {
        cout << "Ghost out of reach IG...\n";
      }
    }
  }

  // For now we decide on moving a ghost Randomly. Takes game state and
  // exploration depth for recursive attemps.
  void updatePositionRandomPolicy(GameState *state, int &depth) {
    if (depth <= 0)
      return;
    GameBoard &board = *state->board;
    unsigned int getDir = rand() % 4;
    Direction dir = static_cast<Direction>(getDir);
    switch (dir) {
    case (Direction::Up): {
      if (row > 0) {
        CellType entity = board.getEntityAt(row - 1, col);
        switch (entity) {
        case (CellType::EmptyT): {
          row -= 1;
          board.updateCell(row, col, CellType::GhostT);
          board.updateCell(row + 1, col, CellType::EmptyT);
          break;
        }

        case (CellType::GhostT): {
          depth -= 1;
          updatePositionRandomPolicy(state, depth);
          break;
        }

        case (CellType::PacmanT): {
          // TODO : Get the guy.
          KillPacman(state);
        } break;

        case (CellType::PelletT): {
          // Check game syementics, for now move the pallet to previous position
          // of ghost.
          row -= 1;
          board.updateCell(row, col, CellType::GhostT);
          board.updateCell(row + 1, col, CellType::PelletT);
          break;
        }

        case (CellType::PowerPelletT): {
          row -= 1;
          board.updateCell(row, col, CellType::GhostT);
          board.updateCell(row + 1, col, CellType::PowerPelletT);
          break;
        }

        case (CellType::WallT): {
          depth -= 1;
          updatePositionRandomPolicy(state, depth);
          break;
        }
        }
      } else {
        depth -= 1;
        updatePositionRandomPolicy(state, depth);
      }
    } break;

    case (Direction::Down): {
      if (row < board.getRows() - 1) {
        CellType entity = board.getEntityAt(row + 1, col);
        switch (entity) {
        case (CellType::EmptyT): {
          row += 1;
          board.updateCell(row, col, CellType::GhostT);
          board.updateCell(row - 1, col, CellType::EmptyT);
          break;
        }

        case (CellType::GhostT): {
          depth -= 1;
          updatePositionRandomPolicy(state, depth);
          break;
        }

        case (CellType::PacmanT): {
          // TODO : Get the guy.
          KillPacman(state);
        } break;

        case (CellType::PelletT): {
          // Check game syementics, for now move the pallet to previous position
          // of ghost.
          row += 1;
          board.updateCell(row, col, CellType::GhostT);
          board.updateCell(row - 1, col, CellType::PelletT);
          break;
        }

        case (CellType::PowerPelletT): {
          row += 1;
          board.updateCell(row, col, CellType::GhostT);
          board.updateCell(row - 1, col, CellType::PowerPelletT);
          break;
        }

        case (CellType::WallT): {
          depth -= 1;
          updatePositionRandomPolicy(state, depth);
          break;
        }
        }
      } else {
        depth -= 1;
        updatePositionRandomPolicy(state, depth);
      }
    } break;

    case (Direction::Right): {
      if (col < board.getCols() - 1) {
        CellType entity = board.getEntityAt(row, col + 1);
        switch (entity) {
        case (CellType::EmptyT): {
          col += 1;
          board.updateCell(row, col, CellType::GhostT);
          board.updateCell(row, col - 1, CellType::EmptyT);
          break;
        }

        case (CellType::GhostT): {
          depth -= 1;
          updatePositionRandomPolicy(state, depth);
          break;
        }

        case (CellType::PacmanT): {
          // TODO : Get the guy.
          KillPacman(state);
          break;
        }

        case (CellType::PelletT): {
          // Check game syementics, for now move the pallet to previous position
          // of ghost.
          col += 1;
          board.updateCell(row, col, CellType::GhostT);
          board.updateCell(row, col - 1, CellType::PelletT);
          break;
        }

        case (CellType::PowerPelletT): {
          col += 1;
          board.updateCell(row, col, CellType::GhostT);
          board.updateCell(row, col - 1, CellType::PowerPelletT);
          break;
        }

        case (CellType::WallT): {
          depth -= 1;
          updatePositionRandomPolicy(state, depth);
          break;
        }
        }
      } else {
        depth -= 1;
        updatePositionRandomPolicy(state, depth);
      }
    } break;

    case (Direction::Left): {
      if (col > 0) {
        CellType entity = board.getEntityAt(row, col - 1);
        switch (entity) {
        case (CellType::EmptyT): {
          col -= 1;
          board.updateCell(row, col, CellType::GhostT);
          board.updateCell(row, col + 1, CellType::EmptyT);
          break;
        }

        case (CellType::GhostT): {
          depth -= 1;
          updatePositionRandomPolicy(state, depth);
          break;
        }

        case (CellType::PacmanT): {
          // TODO : Get the guy.
          KillPacman(state);
          break;
        }

        case (CellType::PelletT): {
          // Check game syementics, for now move the pallet to previous position
          // of ghost.
          col -= 1;
          board.updateCell(row, col, CellType::GhostT);
          board.updateCell(row, col + 1, CellType::PelletT);
          break;
        }

        case (CellType::PowerPelletT): {
          col += 1;
          board.updateCell(row, col, CellType::GhostT);
          board.updateCell(row, col + 1, CellType::PowerPelletT);
          break;
        }

        case (CellType::WallT): {
          depth -= 1;
          updatePositionRandomPolicy(state, depth);
          break;
        }
        }
      } else {
        depth -= 1;
        updatePositionRandomPolicy(state, depth);
      }
    } break;
    }
  }
};

class Pellet {
private:
  int row;
  int col;
  PelletType pty;

public:
  Pellet(int row, int col, PelletType pty) {
    this->row = row;
    this->col = col;
    this->pty = pty;
  }

  pair<unsigned, unsigned> getPostion() const { return make_pair(row, col); }

  PelletType getType() const { return pty; }
};

void GameState::renderGameState() {
  auto pos = pac->getPostion();
  cout << "LIVES LEFT : " << pac->lifeLeftCount() << " | X = " << pos.first
       << " Y = " << pos.second << " |  Score : " << pac->getScore() << "\n";
  this->board->renderBoard();
  cout << "\n\n\n";
}

// Is this relevent any more? is this useful??
void GameState::updateGameState() {}

/// @brief A Game only has a game state and ability to update it, Initalize it.
class Game {
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
  static string getInput() {
    string inp;
    cin >> inp;
    return inp;
  }

  char getUserInput() {
    std::chrono::seconds timeout(5);
    std::string answer; // default to maybe
    std::future<std::string> future = std::async(getInput);
    if (future.wait_for(timeout) == std::future_status::ready)
      answer = future.get();
    char input = ' ';
    if (answer == "w" or answer == "W")
      return 'w';
    else if (answer == "s" or answer == "S")
      return 's';
    else if (answer == "d" or answer == "D")
      return 'd';
    else if (answer == "a" or answer == "A")
      return 'a';
    cout << "Please enter valid movement...\n";
    return input;
  }
  
  void generateMazeRecursively(GameBoard &board,vector<vector<bool>> &visited,unsigned urow,unsigned ucol){
    board.updateCell(urow,ucol,CellType::WallT);
    visited[urow][ucol] = true;
    vector<pair<int,int>> possibleNext;
    for(auto offset : board.Visit4Neighbours){
      int nxtR = urow + offset.first;
      int nxtC = ucol + offset.second;
      if(board.isValidCell(nxtR,nxtC) and (not visited[nxtR][nxtC]))
        possibleNext.push_back(make_pair(nxtR,nxtC));
    }
    if(possibleNext.size()==0)
      return;
    auto nextLoc = possibleNext[rand()%possibleNext.size()];
    generateMazeRecursively(board,visited,nextLoc.first,nextLoc.second);
  }

public:
  Game(GameState *sstate) { state = sstate; }

  void initalizeGameMazeGenerationPolicy(){
    GameBoard &board = *state->board;
    vector<vector<bool>> visited(board.getRows(),vector<bool>(board.getCols(),false));
    for(int i = 0;i<board.getRows();i++){
      for(int j =0;j<board.getCols();j++){
        board.updateCell(i,j,CellType::EmptyT);
      }
    }
    generateMazeRecursively(board,visited,3,2);
    board.renderBoard();
  }

  void initalizeGameSimplestPolicy() {
    GameBoard &board = *state->board;
    for (int i = 0; i < 6; i++) {
      board.updateCell(0, i, CellType::WallT);
      board.updateCell(5, i, CellType::WallT);
      board.updateCell(i, 0, CellType::WallT);
      board.updateCell(i, 5, CellType::WallT);
    }
    board.updateCell(2, 2, CellType::WallT);
    board.updateCell(2, 3, CellType::WallT);
    board.updateCell(3, 2, CellType::WallT);
    board.updateCell(3, 3, CellType::WallT);
    board.updateCell(1, 1, CellType::PelletT);
    board.updateCell(1, 4, CellType::PelletT);
    board.updateCell(4, 1, CellType::PelletT);
    board.updateCell(4, 4, CellType::PelletT);
    Pacman *pac = state->pac;
    pac->resetPac(board, pac->getPostion());
    for (int g = 0; g < state->ghosts->size(); g++) {
      auto pos = state->ghosts->at(g).getPostion();
      board.updateCell(pos.first, pos.second, CellType::GhostT);
    }

    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 6; j++)
        if (board.getEntityAt(i, j) == CellType::EmptyT)
          board.updateCell(i, j, CellType::PelletT);
    }
    board.renderBoard();
  }

  void getAndProcessInputClassic() {
    assert(state->pac->pacLifeStatus() and
           "Cannot process command for non live Pac...");
    char input;
    cin >> input;
#ifdef DEBUG
    cout << ">>> Input : " << input << '\n';
#endif
    switch (input) {
    case 'w': {
      state->pac->move(Direction::Up, state);
      break;
    }

    case 's': {
      state->pac->move(Direction::Down, state);
      break;
    }

    case 'd': {
      state->pac->move(Direction::Right, state);
      break;
    }

    case 'a': {
      state->pac->move(Direction::Left, state);
      break;
    }

    default:
      assert("unsupported action");
    }
  }

  void updateGameState() {}

  void render() {
#ifdef _WIN32
    system("cls");
#elif __linux__
    //system("clear");
#endif
    // TODO : Add rendering of score, lives left.
    state->renderGameState();
  }

  void runGame() {
    initalizeGameMazeGenerationPolicy();
    //initalizeGameSimplestPolicy();
    while (state->pac->pacLifeStatus()) {
      getAndProcessInputClassic();
      state->updateGameState();
      for (int i = 0; i < state->ghosts->size(); i++) {
        // Number of tries per ghost
        int depth = 3;
        state->ghosts->at(i).updatePositionClosestDistancePolicy(state);
      }
      render();
      this_thread::sleep_for(chrono::microseconds(100));
    }
  }
};

int main() {
  // Size of the board.
  //GameBoard(5,5).drawCell(CellType::PacmanT);
  int bsize;
  cin>>bsize;
  GameBoard *board = new GameBoard(bsize,bsize);
  Pacman *pac = new Pacman(1,2);
  vector<Ghost> ghosts = {Ghost(2, 1), Ghost(3, 4)};
  GameState *state = new GameState(board, pac, &ghosts);
  Game g(state);
  g.runGame();
}

/*

g++ -O3 pacman.cpp  -lGL -lGLU -lglut  -o pacman

*/

