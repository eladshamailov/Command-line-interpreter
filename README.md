# Command-line-interpreter
Implementation of a shell (command-line interpreter).
Like traditional UNIX shells, the shell program will also be a user level process , that will
rely heavily on the operating system's services.

The shell do the following:

* Receive commands from the user. 

* Interpret the commands, and use the operating system to help starting up programs and processes requested by the user. 

* Manage process execution (e.g. run processes in the background, suspend them, etc.), using the operating system's services. 

## Shell description
The shell works like a real command interpreter.
It means that the shell supports executing commands without waiting for the process to terminate.
In addition , the shell supports the waiting option , using the waitpid call.
The shell also has "cd" feature that allows the user to change the current working directory.

## Job control description
I used a linked list, where each node is a struct job in order to store a list of all running/suspended jobs.
```
typedef struct job{
    char *cmd;                            /* the whole command line as typed by the user, including input/output redirection and pipes */
    int idx;				  /* index of current job (starting from 1) */
    pid_t pgid; 		          /* process group id of the job*/
    int status;                           /* status of the job */
    struct termios *tmodes;               /* saved terminal modes */
    struct job *next;	                  /* next job in chain */
} job;
```

### Job control feautres

* Receive a cmd (command line), initialize job fields (to NULLs, 0s etc.), and allocate memory for the new job and its fields
*  free all memory allocate for the job, include the job struct itself. 
* Receive a job list and an index, and return the job that has the given index. 
* update the status of jobs running in the background
* Adding jobs to the list, and printing them
* Running jobs in the foreground
* Runing jobs in the background

## Examples
```
$>test1&
Start of test1
$>test2&
Start of test2
$>test3&
Start of test3
$>jobs
[1]     Running         ./test1&
[2]     Running         ./test2&
[3]     Running         ./test3&
$>End of test3
End of test2
End of test1
quit
```
Run in the foreground
```
$> test1
Start of test1
^Z
$> jobs 
[1]     Suspended               test1
$> fg 1
End of test1
[1]     Done               test1
$>quit
```
Run in the background
```
$> test1
Start of test1
^Z
$> test2
Start of test2
^Z
$> test3
Start of test3
^Z
$> jobs
[1]     Suspended               test1
[2]     Suspended               test2
[3]     Suspended               test3
$> fg 3
End of test3
[3]    Done               test3
$> jobs
[1]     Suspended               test1
[2]     Suspended               test2
$> bg 2
$> End of test2
fg 1
End of test1
[1]     Done               test1
$>jobs
[2]     Done               test2
$>jobs
$> quit
```

