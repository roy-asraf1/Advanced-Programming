#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>


int main() 
{
char command[1024];
char *token;
char *outfile;
int i, fd, amper, redirect, retid, status;
int append, redirect_stderr; //another variables not in the original code
char *argv[10]; //

while (1)
{
    printf("hello: ");  
    fgets(command, 1024, stdin); //read a line from the user //need to add error command
    command[strlen(command) - 1] = '\0'; // remove the newline character

    /* parse command line */
    i = 0;
    token = strtok (command," "); //split the command into tokens
    while (token != NULL) //while there are tokens
    {
        argv[i] = token; //store the token in the array
        token = strtok (NULL, " "); //get the next token
        i++;
    }
    argv[i] = NULL; //set the last element of the array to NULL

    /* Is command empty */
    if (argv[0] == NULL) 
        continue;

        amper = 0;
        redirect = 0; 
        append = 0; 
        redirect_stderr = 0;

        for (int j = 0; j < i; j++) { 
            if (!strcmp(argv[j], "&")) { 
                amper = 1; 
                argv[j] = NULL;
            }
            else if (!strcmp(argv[j], ">>")) {
                append = 1;
                outfile = argv[j + 1];
                argv[j] = NULL; argv[j + 1] = NULL;
                break;
            }
            else if (!strcmp(argv[j], "2>")) {
                redirect_stderr = 1;
                outfile = argv[j + 1];
                argv[j] = NULL; argv[j + 1] = NULL;
                break;
            }
            else if (!strcmp(argv[j], ">")) {
                redirect = 1;
                outfile = argv[j + 1];
                argv[j] = NULL; argv[j + 1] = NULL;
                break;
            }
        }

    if (fork() == 0) { 
        /* redirection of IO ? */
        if (redirect) {
            fd = creat(outfile, 0660); 
            close (STDOUT_FILENO) ; 
            dup(fd); 
            close(fd); 
            /* stdout is now redirected */
        }
        else if (append) 
        { //another elseif not in the original code
            
            fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0660);
            close(STDOUT_FILENO);
            dup(fd);
            close(fd);
        }
        else if (redirect_stderr) 
        { //another elseif not in the original code
            fd = creat(outfile, 0660);
            close(STDERR_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);

        execvp(argv[0], argv);
        fprintf(stderr, "Failed to execute '%s'\n", argv[0]);
        exit(EXIT_FAILURE);
        }
    /* parent continues here */
    if (amper == 0)
        retid = wait(&status);
    }

}

}