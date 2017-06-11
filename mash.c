//Patrick Stevens

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//Max amount of arguments allowed
#define MAX_ARG 5
//Max amount of chars allowed 
#define MAX_CHAR 255

int exe(char **cmds, int shell) {
	if (execvp(cmds[0], cmds) == -1) {
		printf("[Shell %d] STATUS CODE=-1\n", shell);
	}
	return 0;
}
char **chop(char *cmd, char *file) {
	char **args;
	int index = 0;
	args = (char**)malloc(sizeof(char*) * MAX_ARG); //max # of args is 5
	args[0] = (char*)malloc(sizeof(char) * MAX_CHAR * MAX_ARG); //max #of char is 255
    
	for (index = 0; index <= MAX_ARG; index++) { 
		args[index] = (*args + MAX_CHAR * index); 
	}
	index = 0;
	args[index] = strtok(cmd, " \n");
	while (args[index] != NULL && index < MAX_ARG) {
		index++;
		args[index] = strtok(NULL, " \n");
	}
	args[index] = file;
	args[index + 1] = NULL;
	return args;
}

int main(int argc, char * argv[]) {

    char *in1 = NULL;
    char *in2 = NULL;
    char *in3 = NULL;
    char *file = NULL;
	unsigned long size = 0;

    printf("mash-1>");
    getline(&in1, &size, stdin);
    printf("mash-2>");
    getline(&in2, &size, stdin);
    printf("mash-3>");
    getline(&in3, &size, stdin);
    printf("file>");
    getline(&file, &size, stdin);
    file = strtok(file, "\n");

    char **cmd1 = chop(in1, file);
    char **cmd2 = chop(in2, file);
    char **cmd3 = chop(in3, file);

    int s;
    int p2, p3;
    int p1 = fork();
    if (p1 == 0)  { //child
        exe(cmd1, 1);
    } else if (p1 > 0) {
        p2 = fork();
        if (p2 < 0) { 
			fprintf(stderr, "Failed\n");
			exit(1);
		} else if (p2 == 0) { //child 
             exe(cmd2, 2);
        } else if (p2 > 0) {
            int p3 = fork();
            if (p3 == 0) { //child 
                exe(cmd3, 3);
            } else if (p3>0) { 
                waitpid(p1, &s, 0);
                waitpid(p2, &s, 0);
                waitpid(p3, &s, 0);
                printf("Done waiting on children: %d %d %d.\n", p1, p2, p3); 
		    }	
        }	
    }
    return 0;
}
    
    
