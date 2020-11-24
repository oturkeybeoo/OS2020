#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include "readcmd.h"


void trace_cmd(struct cmdline *l) {

		int i, j;

		/* Display each command of the pipe */
		printf("trace_cmd\n");
		for (i=0; l->seq[i]!=0; i++) {
			char **cmd = l->seq[i];
			printf("seq[%d]: ", i);
			for (j=0; cmd[j]!=0; j++) {
				printf("(%s) ", cmd[j]);
			}
			printf("\n");
		}
}

void change_dir(char *param) {
	char *curr_dir;
	char buf[1024];
	char *path;
	if ((curr_dir=getcwd(NULL, 0)) == NULL) {
		perror("cd failed (getcwd)");
		return;
	}
	if (!param) path = getenv("HOME");
	else 
		if (strncmp(param,"/",1)) {
			if ((strlen(curr_dir)+strlen(param)+2)>1024) {
				perror("cd failed (path too large)");
				free(curr_dir);
				return;
			}
			path = strcpy(buf,curr_dir);
			path = strcat(path, "/");
			path = strcat(path, param);
		} else path = param;
		//printf("%s\n",path);
		if (chdir(path)) {
			perror ("cd failed (chdir)");
			chdir(curr_dir);
		}
		free(curr_dir);
}



int main()
{
	int spid, status;
	struct cmdline *l;
	char ***seq;
	
	while(1) {
		printf("shell> ");

		l = readcmd();
		trace_cmd(l);
		seq = l->seq;

		if (! *seq) continue;

		if(!strcasecmp(**seq, "exit")) {
			return 0;
		}

		if(!strcasecmp(**seq, "cd")) {
			change_dir((*seq)[1]);
			continue;
		}
	}
	
	//execution of simple command
	// int status;
	// int pid = folk();						
	// if (pid == 0) {	
	// 	execvp(sep[0][0], seq[0]);		
	// }
	// else {
	// 	waitpid(pid, $status, 0);
	// }
	
	
	int pid;
	int fd[2], fd2[2];
	int start = 1;
	int i = 0;
	int status;
	
	while (seq[i] != NULL) {
		char **cmd = seq[i];
		i++;
		pid = fork();
		if(seq[i] != NULL) {
			pipe(fd);
		}
		if(pid == 0) {
			if(seq[i] != NULL) {
				close(fd[0]);
				dup2(fd[i], stdout);
				close(fd[1]);
			}
			if (!start) {
				close(fd2[1]);
				dup2(fd2[0], stdin);
				close(fd2[0]);
			}
			if(execvp(*cmd, cmd) < 0) {
				perror("execvp failed");
	            exit(EXIT_FAILURE);
			}
			else {
				if (!start) {
				close(fd2[1]);
				close(fd2[0]);
			}
			fd2[0] = fd[0]; fd2[1] = fd[1];
		}
		debut = 0;
		}
		if (waitpid(spid,&status,0) < 0) {
			perror("waitpid failed");
	    	exit(EXIT_FAILURE);
		}
	}
	

}
