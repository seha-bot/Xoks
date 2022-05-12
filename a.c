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

char win(char* ploca)
{
    for(int y = 0; y < 3; y++)
    {
        for(int x = 0; x < 3; x++)
        {
            char winner = ploca[x + y * 3];
            if(winner != '-')
            {
                int n1 = 0, n2 = 0, n3 = 0, n4 = 0;
                for(int i = -1; i <= 1; i++)
                {
                    int p1 = x + i, p2 = y + i, p3 = y - i;
                    if(p1 >= 0 && p1 < 3) if(ploca[p1 + y * 3] == winner) n1++;
                    if(p2 >= 0 && p2 < 3) if(ploca[x + p2 * 3] == winner) n2++;
                    if(p1 >= 0 && p1 < 3 && p2 >= 0 && p2 < 3)  if(ploca[p1 + p2 * 3] == winner) n3++;
                    if(p1 >= 0 && p1 < 3 && p3 >= 0 && p3 < 3)  if(ploca[p1 + p3 * 3] == winner) n4++;
                }
                if(n1 == 3 || n2 == 3 || n3 == 3 || n4 == 3) return winner;
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