#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
static HANDLE stdoutHandle;
static DWORD outModeInit;

void setupConsole() {
	DWORD outMode = 0;
	stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if(stdoutHandle == INVALID_HANDLE_VALUE) exit(GetLastError());
	if(!GetConsoleMode(stdoutHandle, &outMode)) exit(GetLastError());
	outModeInit = outMode;
	outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if(!SetConsoleMode(stdoutHandle, outMode)) exit(GetLastError());
}

void resetConsole() {
    printf("\x1b[0m");
	if(!SetConsoleMode(stdoutHandle, outModeInit)) exit(GetLastError());
}
#else
void setupConsole() {}

void resetConsole() {
    printf("\x1b[0m");
}
#endif

void printp(char* ploca)
{
    printf("\x1b[u");
    for(int y = 0; y < 3; y++)
    {
        printf("\x1b[2K");
        for(int x = 0; x < 3; x++)
        {
            printf("%c ", ploca[x + y * 3]);
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
            if(winner == '-') continue;
            int n1 = 0, n2 = 0, n3 = 0, n4 = 0;
            for(int i = -1; i <= 1; i++)
            {
                int p1 = x + i, p2 = y + i, p3 = y - i;
                if(p1 >= 0 && p1 < 3) if(ploca[p1 + y * 3] == winner) n1++;
                if(p2 >= 0 && p2 < 3) if(ploca[x + p2 * 3] == winner) n2++;
                if(p1 >= 0 && p1 < 3 && p2 >= 0 && p2 < 3) if(ploca[p1 + p2 * 3] == winner) n3++;
                if(p1 >= 0 && p1 < 3 && p3 >= 0 && p3 < 3) if(ploca[p1 + p3 * 3] == winner) n4++;
            }
            if(n1 == 3 || n2 == 3 || n3 == 3 || n4 == 3) return winner;
        }
    }
    return 0;
}

char* read_file(const char* path)
{
    FILE * f = fopen(path, "r");
    fseek(f, 0, SEEK_END);
    char* buffer = (char*)malloc(ftell(f) + 1);
    fseek(f, 0, SEEK_SET);
    int i = 0;
    while(1)
    {
        char c = getc(f);
        if(c == EOF) break;
        buffer[i++] = c;
    }
    buffer[i] = '\0';
    fclose(f);
    return buffer;
}
void record_ploca(const char* path, char* ploca)
{
    FILE * f = fopen(path, "a");
    for(int i = 0; i < 9; i++)
    {
        fprintf(f, "%c", ploca[i]);
    }
    fprintf(f, "%c", '\n');
    fclose(f);
}

char* match(char* ploca, char* brain, char simbol)
{
    int best = 0, mmax = 0, off = 0;
    while(1)
    {
        int m = 0;
        for(int i = 0; i < 9; i++)
        {
            if(brain[i + off] == 'l' && ploca[i] == simbol || brain[i + off] == 'w' && ploca[i] != simbol && ploca[i] != '-') m++;
        }
        if(m > mmax)
        {
            mmax = m;
            best = off;
        }
        off += 10;
        if(brain[off] == '\0') break;
    }
    return mmax >= 1 ? &(brain[best]) : 0;
}

int bot(char* ploca, char simbol)
{
    char* mozak = read_file("mozak");
    char* stela = match(ploca, mozak, simbol);

    if(stela) for(int i = 0; i < 9; i++)
    {
        if(stela[i] == 'w' && ploca[i] == '-' && stela[i] != 'l') { free(mozak); return i; }
    }

    int i;
    r:i = rand() % 9;
    if(ploca[i] == '-') return i;
    else goto r;
}

int main()
{
    setupConsole();
    srand(time(0));
    printf("\x1b[s");
    char* ploca = (char*)malloc(9);

    char t = 0;
    int x = 0;
    printf("Trening? (y/n): ");
    scanf("%c", &t);

    if(t == 'y')
    {
        printf("Broj iteracija: ");
        scanf("%d", &x);
    }
    else x = 1;

    for(x; x > 0; x--)
    {
        int i; for(i = 0; i < 9; i++) ploca[i] = '-';
        int potezi = 9;

        while(1)
        {
            printp(ploca);

            char winner = win(ploca);
            if(winner) {
                printf("\x1b[2KWINNER = %c\n", winner);
                for(i = 0; i < 9; i++) ploca[i] = ploca[i] == winner ? 'w' : (ploca[i] == '-' ? '-' : 'l');
                record_ploca("mozak", ploca);
                break;
            }
            if(potezi <= 0) { printf("\x1b[2KNerijeseno!\n"); break; }

            if(potezi & 1 && t == 'n')
            {
                input:printf("\x1b[2KIzaberi mjesto (1-9): \n\x1b[1F\x1b[22C");
                scanf("%d", &i);

                printf("\x1b[2K");
                if(--i < 0 || i > 8 || ploca[i] != '-') { printf("\x1b[41mNe moze tako!\x1b[0m\x1b[1F"); goto input; }
            }
            else
            i = bot(ploca, (potezi & 1) ? 'x' : 'o');
            ploca[i] = (potezi-- & 1) ? 'x' : 'o';
        }
    }

    free(ploca);
    resetConsole();
    return 0;
}