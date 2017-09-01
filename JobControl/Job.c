
#include <stdio.h>
#include <string.h>
#include "LineParser.h"
#include <ctype.h>
#include <linux/limits.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include "JobControl.h"


void signalHandler(int signal)
{
    printf("signal %s ",strsignal(signal));
    printf("is ignored\n");
}

void signalCatcher(){
   if (signal(SIGCHLD,signalHandler)==SIG_ERR){
        printf("\n");
        printf("Couldn't set signal handler\n");
        EXIT_FAILURE;
    }
    if (signal(SIGTSTP,signalHandler)==SIG_ERR){ 
        printf("\n");
        printf("Couldn't set signal handler\n"); 
        EXIT_FAILURE;
    }
    if (signal(SIGQUIT,signalHandler)==SIG_ERR){
         printf("\n");
         printf("Couldn't set signal handler\n");
         EXIT_FAILURE;
    }
    signal(SIGTTIN,SIG_IGN);
    signal(SIGTTOU,SIG_IGN);
}
void execute(cmdLine *pCmdLine){
     if(execvp(pCmdLine->arguments[0],pCmdLine->arguments)==-1){
       perror("Error,exit the program");
       exit(0);
     }
}

void programWait(pid_t pid,cmdLine *pCmdLine,job ** jobsList, job* currJob,struct termios* tmodes){
      int stat;
      if(pCmdLine->blocking==1){
      printf("progam waiting\n");
      printf("\n");
      currJob->pgid=pid;
      runJobInForeground(jobsList,currJob,1,tmodes,getpid());  
      printf("stopped waiting");
      printf("\n");
    }
    else{
        runJobInBackground(currJob,1);
    }
}

int main(int argc, char **argv) {

    job* jobsList= NULL;
    char buf[PATH_MAX];
    pid_t shellPgid=getpid(); /* returns the process ID of the calling process */  
    int idx;
    cmdLine* pCmdLine;
    pid_t pid;
    int fg=0;
    job* currJob=NULL;
    setpgid(shellPgid,shellPgid);
    struct termios tmodes;
    tcgetattr(STDIN_FILENO,&tmodes);/* save the shell terminal attributes */
    
    while(1){
        signalCatcher();
        if (getcwd(buf, sizeof(buf)) == NULL)
            perror("getcwd() error");
        else{
            fprintf(stdout,"current working directory: %s", buf);
            fprintf(stdout,"\n");
            fgets(buf,2048,stdin);/* Read a line from the "user", i.e. from stdin (no more than 2048 bytes). It is advisable to use fgets  */ 
            }
        if(strncmp(buf,"quit",4)==0){
            freeJobList(&jobsList);
            exit(0); /* End the infinite loop of the shell if the command "quit" is entered in the shell, and exit the shell "normally" */
            }
        pCmdLine=parseCmdLines(buf); /* Parse the input using parseCmdLines() (LineParser.h). The result is a structure cmdLine that contains all necessary parsed data */
        if(strncmp(buf,"cd",2)==0){
                if(chdir(pCmdLine->arguments[1])==-1)
                    perror("cd error");
            }
         else if(strncmp(buf,"jobs",4)==0){
            freeCmdLines(pCmdLine);
            printJobs(&jobsList);
            continue;
            }
        else if(strncmp(pCmdLine->arguments[0],"fg",2)==0){
            fg=0;
            idx=atoi(pCmdLine->arguments[1]);
            runJobInForeground(jobsList,findJobByIndex(jobsList,idx),1,&tmodes,getpid());
            fg=1;
            if(fg==1)
                printJobs(&jobsList);
            freeCmdLines(pCmdLine);
            continue;
        }
        else if(strncmp(pCmdLine->arguments[0],"bg",2)==0){
            idx=atoi(pCmdLine-> arguments[1]);
            freeCmdLines(pCmdLine);
            runJobInBackground(findJobByIndex(jobsList,idx),1);
        }
        else {
            pid=fork();
            if(pid<0){
                printf("fork op failed");
            }   
            currJob= addJob(&jobsList,pCmdLine->arguments[0]);
            currJob->status=1;
            if(pid!=0){
                setpgid(pid,pid);
                programWait(pid,pCmdLine,jobsList,currJob,&tmodes);
                freeCmdLines(pCmdLine);
            }
            else{
                setpgid(getpid(), getpid());
                int currJobPgid=getpid();
                currJob->pgid=currJobPgid;
                execute(pCmdLine);
                signalCatcher();
                setpgid(pid,pid);
                programWait(pid,pCmdLine,jobsList,currJob,&tmodes);
                freeCmdLines(pCmdLine);
           }
        }
    }
         freeJobList(jobsList);
         
    return 0;
} 
 