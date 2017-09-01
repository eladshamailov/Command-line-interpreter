
#include <stdio.h>
#include <string.h>
#include "LineParser.h"
#include <linux/limits.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>

void execute(cmdLine *pCmdLine){
    int stat;
    pid_t pidChild = fork(); 
    
   if (pidChild == 0) {
     if(execvp(pCmdLine->arguments[0],pCmdLine->arguments)==-1){
       perror("execvp failed");
       exit(0);
     }
}
   else{
      if(pidChild>0&&pCmdLine->blocking==1){ 
	    waitpid(pidChild,&stat,0);
      } 
}
     	freeCmdLines(pCmdLine); 
}

int main(int argc, char** argv){
  
       char buf[PATH_MAX];
       cmdLine * pCmdLine;
       
    while(1){  
      
      if (getcwd(buf, sizeof(buf)) == NULL)
           perror("getcwd() error");
       else{
           fprintf(stdout,"current working directory: %s", buf);
	   fprintf(stdout,"\n");
           fgets(buf,2048,stdin);/* Read a line from the "user", i.e. from stdin (no more than 2048 bytes). It is advisable to use fgets  */ 
	}
      if(strncmp(buf,"quit",4)==0){ 
	exit(0); /* End the infinite loop of the shell if the command "quit" is entered in the shell, and exit the shell "normally" */
      }
      pCmdLine=parseCmdLines(buf); /* Parse the input using parseCmdLines() (LineParser.h). The result is a structure cmdLine that contains all necessary parsed data */
      if(strncmp(buf,"cd",2)==0){ 
	if(chdir(pCmdLine->arguments[1])==-1)
           perror("cd error");
      }
      else
	execute(pCmdLine);
    }
    	freeCmdLines(pCmdLine); 

    return 0; 
}
