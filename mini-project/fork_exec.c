/**
 * Example program demonstrating UNIX pipes.
 *
 * Figures 3.25 & 3.26
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 25
#define READ_END	0
#define WRITE_END	1

int main(void)
{
	char write_msg[BUFFER_SIZE] = "Greetings";
	char read_msg[BUFFER_SIZE];
	pid_t pid;
	int fd[2];

	/* create the pipe */
	if (pipe(fd) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}

	/* now fork a child process */
	pid = fork();

	if (pid < 0) {
		fprintf(stderr, "Fork failed");
		return 1;
	}

/*STart of msg passing between child and parent */


	if (pid == 0) {  /* child process */
		/* close the unused end of the read pipe */
		close(fd[READ_END]);

		/* write to the write end of the pipe */
		write(fd[WRITE_END], write_msg, strlen(write_msg)+1);
		printf("\nchild end of pipe -- sending: '%s'\n",write_msg);

		/* close the write end of the pipe */
		close(fd[WRITE_END]);
	}
	else { /* parent process*/
		/* close the unused end of write the pipe */
		close(fd[WRITE_END]);

		/* read from the pipe */
		read(fd[READ_END], read_msg, BUFFER_SIZE);
		printf("\nparent end of pipe -- received: %s\n",read_msg);

		/* close the write end of the pipe */
		close(fd[READ_END]);
		wait(NULL);
		exit(EXIT_SUCCESS);
	}

	return 0;
}
