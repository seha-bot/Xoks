#include<stdio.h>
#include<stdlib.h>
#include<nec.h>

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
            }
        }
    }
    return 0;
}

int main()
{
    printf("\x1b[s");
    char* ploca = 0;
    nec_realloc(ploca, 9);
    for(int i = 0; i < 9; i++) ploca[i] = '-';
    int potezi = 9;

    while(1)
    {
        printp(ploca);

        if(win(ploca)) { printf("\x1b[2KWINNER = %c\n", win(ploca)); break; }
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