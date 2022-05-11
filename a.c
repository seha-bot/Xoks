#include<stdio.h>
#include<stdlib.h>
#include<nec.h>

void printp(char* ploca)
{
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

int main()
{
    char* ploca = 0;
    nec_realloc(ploca, 9);
    for(int i = 0; i < 9; i++) ploca[i] = '-';
    int potezi = 9;

    while(1)
    {
        printp(ploca);
        if(potezi <= 0) break;
        int i = 0; input:
        printf("Izaberi mjesto (1-9): ");
        scanf("%d", &i); i--;
        if(i < 0 || i > 8) { printf("Ne moze tako!\n"); goto input; }

        ploca[i] = (potezi-- & 1) ? 'x' : 'o';
    }

    return 0;
}