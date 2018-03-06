#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//------------------------------------------------------------------------------
//	SHELL PROGRAM 3
//	Jhon Nunez
//	jnunez34
//-----------------------------------SIGNALS------------------------------------
void sigint_handler(int sig){

  char msg[] = " SIGINT handled \n361> ";
  write(1, msg, sizeof(msg));

}
//------------------------------------------------------------------------------
void sigint_handler2(int sig){

  char msg[] = " SIGTSTP handled \n361> ";
  write(1, msg, sizeof(msg));

}
//------------------------------------------------------------------------------
int main(){

        char line[500];
        char *args[20];

	//I/O Redirecting
	char buf1[100];
	//int fd1 = open("file.txt", O_RDONLY);

	char fd;
        printf("361> ");

	signal(SIGINT, sigint_handler);// handle the signal ctrl-c
        signal(SIGTSTP, sigint_handler2);// handle the signal ctrl-z

	fgets(line, 500, stdin);

        while (strcmp(line, "exit\n") != 0){// if exit is input close program
        	int i = 0;
        	char *word = strtok(line," ");
        	printf("361> ", word);


		signal(SIGINT, sigint_handler);// handle the signal ctrl-c
		signal(SIGTSTP, sigint_handler2);// handle the signal ctrl-z

//------------------------------------PARSING------------------------------------

                while (word){// parse the input and place into array
			args[i] = malloc(sizeof(char)*100);
			strcpy(args[i], word);

                        word = strtok(NULL, " ");
                        i = i + 1;
                }//end of inner while loop
   		strtok(args[i-1],"\n");
		args[i] = malloc(sizeof(char)*1);
		args[i] = (char) 0;


// PRINT TEST
//        int j = 0;
//        for (;j<i;j++){//print array for testing
//                printf("args[%d]: %s\n", j, args[j]);
//        }//end of for loop

//--------------------------------------EXEC-------------------------------------

        int pid = fork();

        if (pid == 0)
        {
                printf("PID: %d\n", getpid());
               // execv(args[0], args);

		int a;
		for (a = 0; a < i ; a++){

                        if( strncmp(args[a], ">>", 2) == 0)
                        {
                                args[a] = (char*) 0;
                                fd = open(args[a+1], O_RDWR|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
                                dup2(fd,1);
                                close(fd);
                                break;
                        }
                	else if( strncmp(args[a], ">", 1) == 0)
                	{
				args[a] = (char*) 0;
                                fd = open(args[a+1], O_RDWR|O_CREAT, S_IRUSR |S_IWUSR|S_IRGRP|S_IROTH);
                                dup2(fd,1);
                                close(fd);
                                break;

			}

			else if (strncmp(args[a], "<", 1) == 0)
                        {
                        //      read(fd1, buf1, 99);
                        //      buf1[99] = '\0';
                        //      printf("%s", buf1);
                                args[a] = (char*) 0;
                                fd = open(args[a+1], O_RDONLY);
			        dup2(fd, 0);
				close(fd);
				break;
			}

		}
		execvp(args[0], args);
		exit(0);

	}//end of first if
        else
        {
                //printf("I am the parent.  I am waiting for my child %d to die.\n", pid);
                int status;
                wait(&status);
                printf("EXIT: %d \n", WEXITSTATUS(status));
        }//end of else


	printf("361> ");
	fgets(line, 500 , stdin);


}//end of while loop

	return 0;


}//end of program
