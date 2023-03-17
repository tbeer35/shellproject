#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <dirent.h>

void end();
void handleSigInt(int sig);
int handleLS(int wordNum, char *words[]);
int handleCD(int wordNum, char *words[]);
int handleDog(int wordNum, char *words[]);
void handlePWD();
void parseInput();
int main(int argc, char **argv) {
        printf("welcome to the shell, enjoy your stay\n");
        parseInput(argc, &argv);
}

void executeInput(char *words[], int wordNum){
        int x = 0;
        for (int j = 0; j < wordNum; j++)
                printf("words[%d]: %s\n", j, words[j]);
                if(strcmp(words[x], "exit")==0){
                        end();
                }else if(strcmp(words[x], "ls")==0){
			handleLS(wordNum, words);
                }else if(strcmp(words[x], "cd")==0){
			handleCD(wordNum, words);
		}else if(strcmp(words[x], "dog")==0){
                	handleDog();
		}else if(strcmp(words[x], "pwd")==0){
			handlePWD();
//		}else if(strcmp(words[x], "&")==0){
//                      handlebgp();
                }

		x++;
        if (strcmp(words[0],"stop") == 0)
                     exit(-1);
}

void parseInput(int argc, char** argv) {
    char buf[100]; // buffer for line
    char whitespace[] = " \t\r\n\v";
    char *words_on_line[10]; // 10 words on a line
    int stop = 0;
    signal(SIGINT, handleSigInt);
    if (fgets(buf, 100, stdin) == NULL && feof(stdin)){
        end();
    }
    char *s = buf;
    char *end_buf = buf + strlen(buf);
    int eol = 0, i = 0;
    while (1) {
        while (s < end_buf && strchr(whitespace, *s))
                s++;
        if (*s == 0) // eol - done
                 break;
        words_on_line[i++] = s;
        while (s < end_buf && !strchr(whitespace, *s))
             s++;
        *s = 0;
    }
    executeInput(words_on_line, i);
    for (int j = 0; j < i; j++)
            printf("words_on_line[%d]: %s\n", j, words_on_line[j]);
    if (strcmp(words_on_line[0],"stop") == 0)
         exit(-1);
}
void handleSigInt(int sig){
        printf("caught signal %d\n", sig);
}
void end(){
        exit(-1);
}

int handleLS(int wordNum, char *words[]) {
    DIR *dir;
    struct dirent *ent;
	int y = 0;
	while (y < wordNum) {     
	    for (int z = 0; z < 10; z++) {
		if (words[y] != "&" || words[y] < 10){

		// Open the current directory or a chosen directory as an argument
		if (wordNum == 1) {
		    dir = opendir(".");
		} else if (wordNum == 2) {
		    dir = opendir(words[1]);
		} else {
		    printf("Usage: ls [dir]\n");
		    exit(1);
		}
		// if no directory it throws the error message
		if (dir == NULL) {
		    printf("Error: Cannot open directory\n");
		    exit(1);
		}

		// Loop through all the entries in the directory
		while ((ent = readdir(dir)) != NULL) {
		    printf("%s\n", ent->d_name);
		}

		// Close the directory
		closedir(dir);

		return 0;
	    } else if (words[y] == "&") {
		//Run process as background
		int bgProcess = fork();
		printf("%s running in background", &bgProcess);	
	    } else if (words[y] == "|") {
		//Run pipe command

	    }
	y++;
	}
	}
}

int handleCD(int wordNum, char *words[]) {

	for (int z = 0; z < wordNum-1; z++) {
		if (strcmp(words[z+1], "..") == 0) {
		      	chdir("..");
                        char *cwd = getcwd(NULL, 0);
                        printf("Current working directory: %s\n", cwd);
                        free(cwd);
			//break;
		} else if (strcmp(words[z+1], ".") == 0) {
		      	char *cwd = getcwd(NULL, 0);
                   	printf("Current working directory: %s\n", cwd);
                   	free(cwd);
			//break;
		} else if (strcmp(words[z+1], "&") == 0) {
			//Run process as background
                	int bgProcess = fork();
                	printf("%d running in background\n", &bgProcess);
			//break; 
		} else if (strcmp(words[z+1], "|") == 0) {
                	//Run pipe command
			//break;
            	} else if (chdir(words[z+1]) == -1) {
              		printf("No matching directory found\n");
                      	//exit(EXIT_FAILURE);
			//break;
                } 

	}
	return 0;
}		


int handleDog(int wordNum, char *words[]) {
    char filename[100];
    char text[100];
    FILE *file_ptr;

    for (int z = 0; z < wordNum-1; z++) {
	if (strcmp(words[z+1], "&") == 1 && strcmp(words[z+1], "|") == 1) {
            printf("Enter file name: ");
	    fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = '\0'; // Remove trailing newline

    	    file_ptr = fopen(filename, "w"); // open/create file in write mode

    	    if (file_ptr == NULL) { // check if file was opened successfully
                printf("Error opening file.");
                return 1;
    	    }
            fclose(file_ptr); // close file
        } else if (strcmp(words[z+1], "&") == 0) {
	    int bgProcess = fork();
	    if (bgProcess == -1) {
                printf("Error forking process: %s\n", strerror(errno));
            } else if (bgProcess == 0) {
                // Child process
                handleDog(z, words);
                exit(0);
            } else {
                // Parent process
                printf("%d running in background\n", bgProcess);
            }
	} else if (strcmp(words[z+1], "|") == 0) {
	    //pipe
	}
        return 0;
    }
}


void handlePWD() {

   // print the new current working directory
   char *cwd = getcwd(NULL, 0);
   printf("Current working directory: %s\n", cwd);
   free(cwd);

}

int handlebgp() {

}
