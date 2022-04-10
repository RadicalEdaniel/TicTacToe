#include "tictactoe.h"


TicTacToe::TicTacToe()
{
    //getting a random seed - one time for the whole game
    srand(time(NULL));
}

//function for drawing the boardstate
void TicTacToe::drawTheBoardstate()
{
    /*
     * Lambdas and vars for board drawage
     */
    std::cout<<std::endl;
    char rowChar='A';
    char columnChar='1';

    //A rail to create a clearer visual. It adjusts depending on the size of the board
    auto doubleRail = [&]()
    {
        std::cout<<"=================";
        for (int i = 0; i < options.currentSize-3; i++)
            std::cout<<"===="; //a size of an extra cell is 4 chars
        std::cout<<endl;
    };

    //A "hat" of the board
    auto firstSingleRail = [&]()
    {
        std::cout<<"   ___ ";

        for (int column = 0 ; column < options.currentSize-1; column++)//Just to not copy-paste it
            std::cout<<"___ ";
        std::cout<<endl;
    };

    //A rail in-between the rows
    auto singleRail = [&]()
    {
        std::cout<<"  |___|";

        for (int column = 0 ; column < options.currentSize-1; column++)//Just to not copy-paste it
            std::cout<<"___|";
        std::cout<<endl;
    };

    //A part of a cell (there's 3 parts in each row, for clearer visuals)
    auto cellPart = [=](int row, bool empty)
    {
        char symbol;
        if(empty)
            std::cout  <<"  ";
        else
            std::cout  <<char(rowChar+row)<< " ";
        for (int column = 0 ; column < options.currentSize; column++)//Just to not copy-paste it
        {
            symbol = (empty)?' ':boardstate[row][column];
            std::cout<<"| "<<(symbol)<<" ";
        }
        std::cout<<"|";
    };

    /*
     * Drawing the board
     */
    doubleRail();

    std::cout<<"    ";
    for (int i = 0; i < options.currentSize; i++)
        std::cout<<char(columnChar+i)<<"   ";
    std::cout<<endl;
    firstSingleRail();
    for (int row = 0; row < options.currentSize;    row++)
    {
        cellPart(row, true);
        cout<<std::endl;
        cellPart(row, false);
        cout<<std::endl;
        singleRail();
    }
    doubleRail();
}

//goes through the board and checks if there's a win
eGameStatus TicTacToe::checkGameStatus()
{
    for(int player = 0; player<2;player++)
    {

        bool rowCheck = true;
        bool columnCheck = true;
        bool diagonalCheck = true;
        bool inversDiagonalCheck = true;

        //horizontal and vertical check
        for (int row = 0;     row < options.currentSize;    row++)
        {
            rowCheck = true;
            columnCheck = true;
            for (int column = 0 ; column < options.currentSize; column++)
            {
                rowCheck    &= options.players[player].getSymbol() == boardstate[row][column];
                columnCheck &= options.players[player].getSymbol() == boardstate[column][row];
            }
            if(rowCheck == true || columnCheck == true)
            {
                if (options.players[player].getSymbol() == SYMBOL_X)
                    return X_WON;
                else return O_WON;
            }
        }

        //diagonal check
        for (int i = 0; i < options.currentSize; i++)
        {
            diagonalCheck       &= boardstate[i][i]                          == options.players[player].getSymbol();
            inversDiagonalCheck &= boardstate[options.currentSize - i -1][i] == options.players[player].getSymbol();
        }
        if(diagonalCheck == true || inversDiagonalCheck == true)
        {
            if (options.players[player].getSymbol() == SYMBOL_X)
                return X_WON;
            else return O_WON;
        }

        //if there's no win and we can still make moves - the show must go on
    }
    bool drawCheck = false;
    CHECK_ALL
        drawCheck |= boardstate[row][column] == SYMBOL_EMPTY; //if there's no empty space left(and no win) - it's a draw
    if (!drawCheck)
        return DRAW;
    return GAME_CONTINUES;
}

//the "main" function of the game.
void TicTacToe::startGame()
{
    while (true) // a cycle of a game (and starting anew)
    {
        while (true) // a cycle to get a state
        {
            cout<<("Choose battle type! (pvp, pve, eve): ")<<endl;
            string got;
            cin>>got;
            //set input to lower case one char be one
            transform(got.begin(), got.end(), got.begin(),
                [](unsigned char c)
            {
                return tolower(c);
            });

            if(got == "pvp")
            {
                options.style = PVP;
                options.players[PLAYER_X]=GameOptions::Player(SYMBOL_X,true);
                options.players[PLAYER_O]=GameOptions::Player(SYMBOL_O,true);
                break;
            }
            else if(got == "pve")
            {
                options.style = PVE;
                bool human = rand() % 2;//setting a random symbol for a player
                options.players[PLAYER_X]=GameOptions::Player(SYMBOL_X,human);
                options.players[PLAYER_O]=GameOptions::Player(SYMBOL_O,!human);
                break;
            }
            else if(got == "eve")
            {
                options.style = EVE;
                options.players[PLAYER_X]=GameOptions::Player(SYMBOL_X,false);
                options.players[PLAYER_O]=GameOptions::Player(SYMBOL_O,false);
                break;
            }
        }

        //asking the player for the boardsize until they give us the correct answer
        while (true)
        {
            int check;
            cout<<"Pick a size! (3, 4, 5): "<<endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cin>>check;
            if(check>=3&&check<=5)
            {
                options.currentSize = check;
                boardstate.clear();
                //allocating space in the board vector matrix
                boardstate.resize(options.currentSize, std::vector<char>(options.currentSize, SYMBOL_EMPTY));
                amountOfEmptySpaces = options.currentSize*options.currentSize;
                break;
            }
        }

        bool gameInProgress = true; //while it's true - game's while is happy
        int turnN=0;                //number of the turn.
        bool currentPlayer;         //current player switches every turn

        while(gameInProgress)
        {
            //0 1 0 1 0 1 (0 = X, 1 = O)
            currentPlayer=turnN%2;
            turnN++;
            //comment this for hardcore mode.
            drawTheBoardstate();
            //checking if we won the game
            switch(checkGameStatus())
            {
                case(GAME_CONTINUES):

                    if(options.players[currentPlayer].isHuman())
                        humanMakesAMove(currentPlayer);
                    else
                        aiMakesAMove(currentPlayer);
                    amountOfEmptySpaces--;
                    continue; //while the game's in progress - the show goes on with this lil line

                case(X_WON):
                    cout<<"X won!"<<endl;
                break;

                case(O_WON):
                    cout<<"Wow, O has actually won."<<endl;
                break;

                case(DRAW):
                    cout<<"DRAW!"<<endl;
                break;
            }
            gameInProgress=false;//to exit the current game loop
            //after the game's finished - we ask player if he wants to play again
            while(true)
            {
                cout<<"Play again? y/n"<<endl;
                string got;

                cin>>got;
                if (got == "y")
                    break;

                else if(got == "n")
                {
                   cout<<"deleting system32..."<<endl; //tee hee
                   cin.ignore();
                   exit(0);
                }
            }
        }
    }
}

//machines are taking over. This is the first step
void TicTacToe::aiMakesAMove(int playerN)
{
    //creating vars here
    int bestScore = numeric_limits<int>::min();
    int currentScore;
    CellN bestCell;
    int depth = 0;//how deep we are in the algorithm

    //vars for alpha-beta prunes
    //the example of prunes:
    //-      /4\
    //+   /4\    /5\
    //   4   3  5   ???
    //the idea is that we don't need to check the ??? route because the minimizer on top will always pick 4 anyway
    int alphaPrune =numeric_limits<int>::min();//best already exposed option for the maximazer
    int betaPrune = numeric_limits<int>::max();//best already exposed option for the minimazer
    char aiSymbol=options.players[playerN].getSymbol();


    cout<<"Ai makes a move with "<<aiSymbol<<endl;

    //here we check for every possible move and then imagine every possible move after that
    CHECK_ALL
        if(boardstate[row][column] == SYMBOL_EMPTY)
        {
            //we place a POSSIBLE move
            boardstate[row][column] = aiSymbol;
            //"max" is false because we already made move, so it's our imaginary enemy's turn!
            currentScore = miniMaxAlgorithm(playerN, false,depth,alphaPrune,betaPrune);
            boardstate[row][column] = SYMBOL_EMPTY; //we don't want to populate our board with symbols so we delete it after we tried it
            if (currentScore > bestScore )
            {
                bestScore = currentScore;
                bestCell.row = row;
                bestCell.column = column;
            }
        }
    //in the end we assign the best move to the board
    boardstate[bestCell.row][bestCell.column] = aiSymbol;
}

//the only thing we can do is resist it for a time. But our doom is inevitable
void TicTacToe::humanMakesAMove(int playerN)
{
    while(true)
    {
        try
        {
            cout <<"It's "<<options.players[playerN].getSymbol()<<"'s turn! "
                <<"Pick a spot (like 'A1' or 'B3') or type 'nope' to turn into a machine" << endl;
            string got;
            cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cin >> got;

            //set input to lower case one char be one
            transform(got.begin(), got.end(), got.begin(),
                [](unsigned char c)
            {
                return tolower(c);
            });

            if(got == "nope")
            {
               options.players[playerN].turnIntoACyborg();
               aiMakesAMove(playerN);
               return;
            }
            if(got.length()<2)
                continue;

            //getting the values from the input
            int row     = int(got[0] - ('a'-1))-1;
            int column  = int(got[1] - '0')-1;

            //if the value is correct - placing it on the board
            if((row>=0&& row<options.currentSize )&&
                (column>=0&&column<options.currentSize)&&
                    boardstate[row][column]==' ')
            {
                boardstate[row][column] = options.players[playerN].getSymbol();
                return;
            }
        }
        catch (std::exception e)//just in case something outrageous happens
        {
            cout << "Wrong data!"<<endl;
            cin.clear();
        }
    }
}

//i've decided to use minimax algorythm for ai "brains". It's basically a tree that tries (almost) every possible move to decide the best course of action
int TicTacToe::miniMaxAlgorithm(int playerN, bool isMax,int depth, int alphaPrune, int betaPrune)
{
    //without that boards above 3x3 are working extremely slowly. Probably can tinker with these values to crete different game difficulties/make it work faster on 5x5
    int allowedDepth=(amountOfEmptySpaces>15)?4:(amountOfEmptySpaces>9)?5:numeric_limits<int>::max();


    if(depth>allowedDepth) //we can't go too deep or we'll wait forever
        return 0;

    //checking if we actually won already
    eGameStatus result = checkGameStatus();
    if(result != GAME_CONTINUES)
    {
        if(result == DRAW)
            return 0;
        if(result == playerN) //we won! But at what cost...
            return 100;        //answer: 100 dollars
        return -100;
    }

    int bestScore;
    //The maximazer, "our" possible move.
    if(isMax)
    {
        //the best score we had. Our standards are pretty low right now, huh.
        bestScore =numeric_limits<int>::min();
        //current score - results we be getting from current branch of the tree.
        int currentScore;
        CHECK_ALL
        {
            if (boardstate[row][column] == SYMBOL_EMPTY)
            {
                boardstate[row][column] = options.players[playerN].getSymbol();
                //here we're brancing out to another, well, branch.
                //We deeper we go - the less valuable the win is (as it is less likely to actually occur)
                currentScore = miniMaxAlgorithm(playerN,false,depth+1,alphaPrune,betaPrune) - depth;

                boardstate[row][column] = SYMBOL_EMPTY;
                //checking if the best score still holds
                bestScore = max(currentScore,bestScore);
                //we're using prunes to possibly reduce the amount of moves we have to take
                alphaPrune = max(alphaPrune, bestScore);

                //if the minimazer parent has the better option already - it'll take that. No need to look at the other branches
                if(betaPrune <= alphaPrune)
                    return alphaPrune;
            }
        }
    }
    //The minimazer. "enemy's" possible turns. Basically the same as "ours", but in the other direction.
    else
    {
        bestScore = numeric_limits<int>::max();
        int currentScore;
        CHECK_ALL
        {
            if (boardstate[row][column] == SYMBOL_EMPTY)
            {
                boardstate[row][column] = options.players[!playerN].getSymbol();
                currentScore = miniMaxAlgorithm(playerN,true,depth+1,alphaPrune,betaPrune)+ depth;
                boardstate[row][column] = SYMBOL_EMPTY;

                bestScore = min(currentScore,bestScore)  ;

                betaPrune = min(betaPrune, bestScore);
                if(betaPrune <= alphaPrune)
                    return betaPrune;
            }
        }
    }
    return bestScore;
}

bool GameOptions::Player::isHuman()
{
    return human;
}

char GameOptions::Player::getSymbol()
{
    return symbol;
}

//alas
void GameOptions::Player::turnIntoACyborg()
{
    human=false;
}
