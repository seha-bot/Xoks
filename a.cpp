#include<iostream>
#include<string>

int index(int x, int y, int boardSize)
{
    return x >= 0 && x < boardSize && y >= 0 && y < boardSize ? x + y * boardSize : -1;
}

void printBoard(char* board, int boardSize)
{
    for(int y = 0; y < boardSize; y++)
    {
        for(int x = 0; x < boardSize; x++)
        {
            std::cout << board[index(x, y, boardSize)] << " ";
        }
        std::cout << std::endl;
    }
}

int place(char* board, char* player, int *turns, int loc)
{
    if(board[loc] == '-')
    {
        board[loc] = *player ? 'x' : 'o';
        *player = !(*player);
        (*turns)--;
        return 1;
    }
    else
    {
        std::cout << "Cannot place there." << std::endl;
        return 0;
    }
}

int check(int x, int y, char* board, int boardSize, int winner, int spacesRequiredToWin)
{
    int mode = 4;
    while(mode--)
    {
        int win = 0;
        for(int c = -spacesRequiredToWin; c <= spacesRequiredToWin; c++)
        {
            int i = -1;
            if(mode == 0) i = index(x + c, y, boardSize);
            if(mode == 1) i = index(x, y + c, boardSize);
            if(mode == 2) i = index(x + c, y + c, boardSize);
            if(mode == 3) i = index(x - c, y + c, boardSize);
            if(i != -1) if(board[i] == winner) win++;
        }
        if(win == spacesRequiredToWin) return 1;
    }
    return 0;
}

char checkWin(char* board, int boardSize, int spacesRequiredToWin)
{
    for(int y = 0; y < boardSize; y++)
    {
        for(int x = 0; x < boardSize; x++)
        {
            char winner = board[index(x, y, boardSize)];
            if(winner == 'x' || winner == 'o')
            {
                if(check(x, y, board, boardSize, winner, spacesRequiredToWin)) return winner == 'x' ? 'x' : 'o';
            }
        }
    }
    return 0;
}

int bot(char* board, int boardSize)
{
    for(int i = 0; i < boardSize * boardSize; i++)
    {
        if(board[i] == '-') return i;
    }
}

int main()
{
    char player = 1;
    int boardSize = 3;
    int spacesRequiredToWin = 3;
    int turns = boardSize * boardSize;
    char *board = (char*)malloc(turns);
    for(int i = 0; i < turns; i++) board[i] = '-';

    while(1)
    {
        printBoard(board, boardSize);
        char winner = checkWin(board, boardSize, spacesRequiredToWin);
        if(winner != 0) { std::cout << "Winner: " << winner << std::endl; break; }
        if(turns <= 0) { std::cout << "Tie." << std::endl; break; }
        if(player == 0)
        {
            std::cout << "Bot:" << std::endl;
            place(board, &player, &turns, bot(board, boardSize));
            continue;
        }
        std::cout << "Pick your spot, 1-" << (boardSize * boardSize) << ": "; int choice; std::cin >> choice;
        place(board, &player, &turns, choice - 1);
    }
    return 0;
}