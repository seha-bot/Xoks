#include<stdio.h>
#include<stdlib.h>

void printp(char* ploca)
{
    printf("\x1b[u");
    for(int y = 0; y < 3; y++)
    {
        for(int x = 0; x < 3; x++)
        {
            int i = x + y * 3;
            printf("%c ", ploca[i]);
        }
        printf("\n");
    }
}

int getnh(char* ploca, char c, int x, int y)
{
    int n = 0;
    for(int i = -1; i <= 1; i++)
    {
        int p = x + i;
        int id = p + y * 3;
        if(p >= 0 && p < 3 && ploca[id] == c) n++;
    }
    return n;
}
int getnv(char* ploca, char c, int x, int y)
{
    int n = 0;
    for(int i = -1; i <= 1; i++)
    {
        int p = y + i;
        int id = x + p * 3;
        if(p >= 0 && p < 3 && ploca[id] == c) n++;
    }
    return n;
}
int getnd1(char* ploca, char c, int x, int y)
{
    int n = 0;
    for(int i = -1; i <= 1; i++)
    {
        int p1 = x + i;
        int p2 = y + i;
        int id = p2 + p1 * 3;
        if(p1 >= 0 && p1 < 3 && p2 >= 0 && p2 < 3 && ploca[id] == c) n++;
    }
    return n;
}
int getnd2(char* ploca, char c, int x, int y)
{
    int n = 0;
    for(int i = -1; i <= 1; i++)
    {
        int p1 = x + i;
        int p2 = y - i;
        int id = p2 + p1 * 3;
        if(p1 >= 0 && p1 < 3 && p2 >= 0 && p2 < 3 && ploca[id] == c) n++;
    }
    return n;
}

char win(char* ploca)
{
    for(int y = 0; y < 3; y++)
    {
        for(int x = 0; x < 3; x++)
        {
            char winner = ploca[x + y * 3];
            if(winner != '-')
            {
                if(getnh(ploca, winner, x, y) == 3) return winner;
                if(getnv(ploca, winner, x, y) == 3) return winner;
                if(getnd1(ploca, winner, x, y) == 3) return winner;
                if(getnd2(ploca, winner, x, y) == 3) return winner;
            }
        }
    }
    return 0;
}

int main()
{
    printf("\x1b[s");
    char* ploca = (char*)malloc(9);
    for(int i = 0; i < 9; i++) ploca[i] = '-';
    int potezi = 9;

    while(1)
    {
        printp(ploca);

        char winner = win(ploca);
        if(winner) { printf("\x1b[2KWINNER = %c\n", winner); break; }
        if(potezi <= 0) { printf("\x1b[2KNerijeseno!\n"); break; }

        int i = 0; input:
        printf("\x1b[2KIzaberi mjesto (1-9): \n\x1b[1F\x1b[22C");
        scanf("%d", &i); i--;

        printf("\x1b[2K");
        if(i < 0 || i > 8 || ploca[i] != '-') { printf("Ne moze tako!\x1b[1F"); goto input; }

        ploca[i] = (potezi-- & 1) ? 'x' : 'o';
    }

    return 0;
}