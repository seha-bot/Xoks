#include<iostream>
#include<JSON.h>

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

int place(char* board, char* player, int *turns, int loc, char* moves, int* x, int* o)
{
    if(board[loc] == '-')
    {
        if(*player) { board[loc] = 'x'; moves[loc] = std::to_string(++*x)[0]; }
        if(!(*player)) { board[loc] = 'o'; moves[loc] = std::to_string(++*o)[0]; }
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
            char winner = 'e';
            if(board[index(x, y, boardSize)] == 'x') winner = 'x';
            if(board[index(x, y, boardSize)] == 'o') winner = 'o';
            if(winner != 'e')
            {
                if(check(x, y, board, boardSize, winner, spacesRequiredToWin))
                {
                    if(winner == 'o') return 'o';
                    if(winner == 'x') return 'x';
                }
            }
        }
    }
    return 0;
}

int match(std::vector<std::string> boards, char* board, int* matches)
{
    int winmatches = 0;
    int winboard = 0;
    int lossmatches = 0;
    int lossboard = 0;
    for(int b = 0; b < boards.size(); b++)
    {
        int t_matches = 0;
        for(int c = 0; c < 9; c++)
        {
            if(boards[b][c] != '-' && boards[b][c] != 'l' && board[c] == 'o' || boards[b][c] == board[c]) t_matches++;
        }
        if(t_matches > winmatches)
        {
            winmatches = t_matches;
            winboard = b;
        }
    }
    for(int b = 0; b < boards.size(); b++)
    {
        int t_matches = 0;
        for(int c = 0; c < 9; c++)
        {
            if(boards[b][c] != '-' && boards[b][c] != 'l' && board[c] == 'x' || boards[b][c] == board[c]) t_matches++;
        }
        if(t_matches > lossmatches)
        {
            lossmatches = t_matches;
            lossboard = b;
        }
    }
    *matches = winmatches > lossmatches ? winmatches : lossmatches;
    if(winmatches > lossmatches) return winboard;
    else if(lossmatches > winmatches) return lossboard;
    else return lossboard;
}

void record(char* board, int boardSize, char winner, std::vector<std::string> old_boards, char* moves)
{
    JSON boards(JSON_ARRAY);
    std::string s = "";
    for(int i = 0; i < boardSize * boardSize; i++)
    {
        if(board[i] == winner) s += moves[i];
        else if(board[i] == '-') s.append("-");
        else s.append("l");
    }
    boards.Write(s);
    for(int i = 0; i < old_boards.size(); i++)
    {
        if(old_boards[i] == s) return;
        boards.Write(old_boards[i]);
    }

    JSON file;
    file.Write("boards", boards);
    std::string buffer = file.GenerateJSON();
    file.WriteAll("data.tic", buffer);
}

int temp_ctoi(char c)
{
    if(c == '0') return 0;
    if(c == '1') return 1;
    if(c == '2') return 2;
    if(c == '3') return 3;
    if(c == '4') return 4;
    if(c == '5') return 5;
    if(c == '6') return 6;
    if(c == '7') return 7;
    if(c == '8') return 8;
    if(c == '9') return 9;
}

int bot(char* board, int boardSize, std::vector<std::string> boards)
{
    int a, b;
    std::string optimal = boards[match(boards, board, &a)];
    char *fboard = (char*)malloc(boardSize * boardSize);
    for(int i = 0; i < boardSize * boardSize; i++) fboard[i] = board[i];
    for(int i = 0; i < boardSize * boardSize; i++)
    {
        if(fboard[i] == '-' && optimal[i] != 'l') { fboard[i] = 'o'; break; }
    }
    std::string foptimal = boards[match(boards, fboard, &b)];

    if(a > b)
    {
        std::cout << "Optimal solution: " << optimal << std::endl;
        int lowest = 10, index = -1;
        for(int i = 0; i < boardSize * boardSize; i++)
        {
            if(board[i] == '-' && optimal[i] != '-' && optimal[i] != 'l')
            {
                if(temp_ctoi(optimal[i]) < lowest)
                {
                    lowest = temp_ctoi(optimal[i]);
                    index = i;
                }
            }
        }
        if(index != -1) return index;
        std::cout << "Prediction Mismatch...\n";
    }

    std::cout << "Optimal solution: " << foptimal << std::endl;
    int lowest2 = 10, index2 = -1;
    for(int i = 0; i < boardSize * boardSize; i++)
    {
        if(board[i] == '-' && foptimal[i] != '-' && foptimal[i] != 'l')
        {
            if(temp_ctoi(foptimal[i]) < lowest2)
            {
                lowest2 = temp_ctoi(foptimal[i]);
                index2 = i;
            }
        }
    }
    if(index2 != -1) return index2;

    std::cout << "Prediction Failed...\n";

    for(int i = 0; i < boardSize * boardSize; i++)
    {
        if(board[i] == '-' && optimal[i] != 'l' && optimal[i] != '-') return i;
    }
    std::cout << "Backup Prediction Failed...\n";

    for(int i = 0; i < boardSize * boardSize; i++)
    {
        if(board[i] == '-') return i;
    }
}

int main()
{
    char player = 0;
    int boardSize = 3;
    int spacesRequiredToWin = 3;
    int turns = boardSize * boardSize;
    char *board = (char*)malloc(turns);
    char *moves = (char*)malloc(turns);
    int x = 0, o = 0;
    for(int i = 0; i < turns; i++) board[i] = moves[i] = '-';

    JSON file;
    std::string buffer = file.ReadAll("data.tic");
    file = file.TranslateJSON(buffer);
    std::vector<std::string> boards = file.GetO("boards").GetAllS();

    while(1)
    {
        printBoard(board, boardSize);
        char winner = checkWin(board, boardSize, spacesRequiredToWin);
        if(winner != 0) { record(board, boardSize, winner, boards, moves); std::cout << "Winner: " << winner << std::endl; break; }
        if(turns <= 0) break;
        if(player == 0)
        {
            std::cout << "Bot:" << std::endl;
            place(board, &player, &turns, bot(board, boardSize, boards), moves, &x, &o);
            continue;
        }
        std::cout << "Pick your spot, 1-" << (boardSize * boardSize) << ": "; int choice; std::cin >> choice;
        place(board, &player, &turns, choice - 1, moves, &x, &o);
    }
    return 0;
}