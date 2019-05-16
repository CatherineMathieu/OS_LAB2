/*Program to read from file using getc() function*/
#include <stdio.h>
#include <sys/file.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define FALSE 0
#define TRUE (!FALSE)
#define LSIZE 150
#define RSIZE 10

void sigh(int);  /* handler declaration */

volatile sig_atomic_t flag = FALSE;

int main()
{
    char line[RSIZE][LSIZE];
    FILE *filePointer = NULL;
    char ch;
    char fileName[] = "testFileLAB2.txt";
    char text[20];
    int i = 0;
    int tot = 0;

	signal(SIGINT, sigh);
    while (1)
    {
	flag = FALSE;
        /*Open file in read mode*/
        filePointer = fopen(fileName, "r");
        while(fgets(line[i], LSIZE, filePointer))
        {
            line[i][strlen(line[i]) - 1] = '\0';
            i++;
        }
        tot = i;
        fclose(filePointer);

        while (!flag)
        {
		printf("Process id: %d\n", getpid());
            for(i = 0; i < tot; ++i)
            {
                printf(" %s\n", line[i]);
            }
            printf("\n");
            sleep(1);
        }

        //return 0;
    }
}

void sigh(int signum)
{
    flag = TRUE;
}