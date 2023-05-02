//Shivam Kundan
//ECE493 Spring '16
//Lab5

/*This program implements a unix process scheduler that creates
  N processes and schedules them in a Round Robin way for time
  quantum qt. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include "queue.c"

void terminate();

queue plist;    //Holds pid's of forked processes
int qt;         //Time quantum
pid_t pid; 
char cwd[1024];
char path[1024];

int main(int argc, char const *argv[])
{
    //Variable initialization
    pid_t cpid[argc-2];
    queueInit(&plist);

    //Get current working dir
    if (getcwd(cwd, sizeof(cwd)) == NULL) perror("getcwd() error");

    //Parse args
    if (argc>2)
    {
        qt = 1000 * atoi (&argv[1][0]); //Here you need the qt
    }
    else
    {
        printf ("Incorrect format!\nUse: %s qt prog1 [prog2]...[prog N}\n",argv[0]) ;
        exit(-1);
    }

    //Create executable files
    for (int x=2;x<argc;x++) 
        {
            printf("Message from father: Creating program %s\n",argv[x]);
            if (fork()==0) execlp("gcc","gcc","./p.c","-o",argv[x],"-w",NULL);
        }

    sleep(1);

    //Signal handling
    signal(SIGCHLD,terminate);

    //Fork processes to execute and enqueue programs
    for (int i=2;i<argc;i++)
    {
        //Next four lines are used to obtain the complete path to the executable
        strcpy(path,"");
        strcpy(path,cwd);
        strcat(path,"/");
        strcat(path,argv[i]);
        
        if ((cpid[i]=fork())==0)
        {
            execlp(path,argv[i],NULL);
            exit(0);
        }
        else
        {
            queueInsert(&plist, cpid[i]);   //Parent process saves pid's to plist
        }
    }

    printf("\nprocesses created:\n");
    queuePrint(plist);

    
    sleep(1);
    printf("\nI am the scheduler and I will now begin scheduling my programs:\n");
    sleep(1);

    while(plist.first!=NULL)
    {
        //Signal process to continue
        kill(plist.first->pid,SIGCONT);
        
        //Wait for time quantum
        usleep(qt);
        
        //If process has not finished
        if (pid==0) 
        {
            kill(plist.first->pid,SIGUSR1);   //Signal process to stop
            usleep(1000);
            queueRotate(&plist);   //Dequeue and re-enqueue
        }
        
        //If process has finished
        else
        {
            printf("A child is dead ");
            queuePop(&plist);
        }
    }

    wait(NULL);
    printf("All of my children died so bye...\n");

    exit(0);
}

void terminate()
/*Signal handler for SIGCHLD.*/
{
    while (1) 
    {
        pid = wait3 (NULL, WNOHANG,NULL );
        if (pid<1) return;
    }
}

