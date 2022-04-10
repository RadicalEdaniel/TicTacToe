#ifndef TICTACTOE_H
#define TICTACTOE_H
#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <algorithm>
#include <cctype>
#include <string>
#include <time.h>
using namespace std;
typedef  vector<vector<char>>  board_t;

#define CHECK_ALL for (int row = 0;     row < options.currentSize;    row++)\
                  for (int column = 0 ; column < options.currentSize; column++)//Just to not copy-paste it

#define PLAYER_X 0
#define PLAYER_O 1
#define SYMBOL_X 'X'
#define SYMBOL_O 'O'
#define SYMBOL_EMPTY ' '

enum eGameStatus
{
    X_WON,
    O_WON,
    DRAW,
    GAME_CONTINUES
};
enum eBoardSize
{
    BOARD_3X3=3,
    BOARD_4X4,
    BOARD_5X5
};

enum eGameStyle
{
    PVP,PVE,EVE
};

struct CellN
{
    int row=0;
    int column=0;
};

//a struct that includes game's options
struct GameOptions
{

public:
    GameOptions(eGameStyle style = PVP, eBoardSize size = BOARD_3X3):
        style(style), currentSize(size)
    {
        players[PLAYER_X]=Player('X');
        players[PLAYER_O]=Player('O');
    }

    //a class for a player
    class Player
    {
    public:
        Player( char symbol = 'X', bool human = false):
            symbol(symbol), human(human)
        {
        }
        //just simple functions to return private vars
        bool isHuman();
        char getSymbol();

        //i reject my humanity jojo!
        void turnIntoACyborg();
    private:
        char symbol;
        bool human;
    };

public:
    Player players[2];
    eGameStyle style;
    int currentSize;

};

class TicTacToe
{
public:
    TicTacToe();
    //function for drawing the boardstate
    void drawTheBoardstate();
    //goes through the board and checks if there's a win
    eGameStatus checkGameStatus();
    //the "main" function of the game.
    void startGame();
    //a function for ai to place it's symbol on a board
    void aiMakesAMove(int playerN);
    //a player places it's symbol on a board via typing coordinates
    void humanMakesAMove(int playerN);
    //i've decided to use minimax algorythm for ai "brains". It's basically a tree that tries (almost, for 4x4 and 5x5 it's not as thorough) every possible move to decide the best course of action
    int miniMaxAlgorithm(int playerN, bool isMax, int depth = 0, int alphaPrune =numeric_limits<int>::min(), int betaPrune =numeric_limits<int>::max());

private:
    //the main vector that contains chars of the 2x2 matrix
    board_t boardstate;
    GameOptions options;
    //a variable that is used
    int amountOfEmptySpaces;

};

#endif // TICTACTOE_H
