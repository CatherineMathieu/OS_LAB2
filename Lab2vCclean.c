#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<sys/file.h>
#include <signal.h>

#define FALSE 0
#define TRUE (!FALSE)

#define LOCK_SH 1 /* shared lock */
#define LOCK_EX 2 /* exclusive lock */
#define LOCK_NB 4 /* don't block when locking */
#define LOCK_UN 8 /* unlock */

void sighandler_Wait(int); /* handler declaration */

void clearContent(char const fileToModify[])
{
    fopen(fileToModify, "w");
}

int main(void)
{
    pid_t childPid;
    pid_t parentPid = getpid();
    pid_t oppositePid;
    FILE *filePointer;
    int lock = 0;
    int release = 0;
    char text[] = "a short text\n";
    char fileName[] = "fileTest.txt";
    char c;
    int writer;
    int isParent;

    printf("Process %d : Parent\n", parentPid);

    // Allow child process to use fileno(filePointer) when reading for the first time
    filePointer = fopen(fileName, "w");

    // Initialize signal handler for inter-process communication
    signal(SIGINT, sighandler_Wait);

    // Create child process
    childPid = fork();

    // Assign process first roles
    isParent = childPid > 0;
    writer = isParent;

    // Determine id of the other process
    oppositePid = isParent ? childPid : parentPid;

    while(1)
    {
        if(writer)
        {
            printf("Process %d : OPEN and LOCK file\n", getpid());
            filePointer = fopen(fileName, "w");
            if(filePointer == NULL)
            {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
            }
            flock(fileno(filePointer), LOCK_EX | LOCK_NB);

            printf("Process %d : WRITE file\n", getpid());
            for(int i = 0; text[i] != '\n'; i++)
            {
                fputc(text[i], filePointer); // write to file
            }
            sleep(1);

            printf("Process %d : RELEASE lock and CLOSE file\n", getpid());
            flock(fileno(filePointer), LOCK_UN | LOCK_NB);
            fclose(filePointer);
            sleep(1);

            printf("Process %d : SIGNAL opposite process\n", getpid());
            kill(oppositePid, SIGINT);

            printf("Process %d : WAIT signal\n", getpid());
            pause();
            sleep(1);

            printf("Process %d : CLEAR file\n", getpid());
            clearContent(fileName);
            writer = FALSE;

            printf("Process %d : SIGNAL opposite process\n", getpid());
            kill(oppositePid, SIGINT);
        }
        else
        {
            printf("Process %d : WAIT signal\n", getpid());
            pause();

            printf("Process %d : WAIT while lock\n", getpid());
            while((flock(fileno(filePointer), LOCK_EX | LOCK_NB)) !=0)
            {
                // printf("Lock = %d\n", lock); // Proof that lock work.
            }
            flock(fileno(filePointer), LOCK_UN | LOCK_NB);

            printf("Process %d : READ file\n", getpid());
            filePointer = fopen(fileName, "r");
            if(filePointer == NULL)
            {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
            }
            while ((c = getc(filePointer)) != EOF)
            {
                printf("%c", c);
            }
            printf("\n");
            fclose(filePointer);

            printf("Process %d : SIGNAL opposite process\n", getpid());
            kill(oppositePid, SIGINT);

            printf("Process %d : SWAP role\n", getpid());
            writer = TRUE;

            printf("Process %d : WAIT signal\n", getpid());
            pause();
            sleep(1);
        }
    }
}

void sighandler_Wait(int signum)
{

}