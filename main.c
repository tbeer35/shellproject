#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <dirent.h>
#include <stdbool.h>

//declare all functions we use later
void end();
void handleSigInt(int sig);
int handleLS(int wordNum, char *words[], int curWord, bool pipe);
int handleCD(int wordNum, char *words[]);
int handleDog(int wordNum, char *words[]);
void handlePWD();
void parseInput();
//main starts the program, creates an int and char** for later use, calls parse input
int main(int argc, char **argv) {
        printf("welcome to the shell, enjoy your stay\n");
        parseInput(argc, &argv);
}

//takes in the input in a char** and how much there is and calls various other functions to handle each one
void executeInput(char *words[], int wordNum, bool pipe){
	int x =0;
        for (int j = 0; j < wordNum; j++) //loop over all input
                printf("words[%d]: %s\n", j, words[j]);
                if(strcmp(words[x], "exit")==0){
                        end();
                }else if(strcmp(words[x], "ls")==0){
			x = handleLS(wordNum, words, x, pipe);
			pipe = false;
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

//takes in a int and char** as input, loops until the shell is killed
//takes in input, cleans whitespace, calls an execute input function
void parseInput(int argc, char** argv) {
    //declare variables
    char buf[100]; // buffer for line
    char whitespace[] = " \t\r\n\v";
    char *words_on_line[10]; // 10 words on a line
    int stop = 0;
    while(1){ //loop until the shell is killed
	    bool pipe = false;
    	signal(SIGINT, handleSigInt); //catch ctrl-C
    	if (fgets(buf, 100, stdin) == NULL && feof(stdin)){ //catch ctrl-D
        	end(); //call end
    	}
    	char *s = buf;
    	char *end_buf = buf + strlen(buf);
    	int eol = 0, i = 0;
    	while (1) { //clear whitespace and put input into a char** and the amount of input into an int
        	while (s < end_buf && strchr(whitespace, *s))
                	s++;
        	if (*s == 0) // eol - done
                 	break;
        	words_on_line[i++] = s;
        	while (s < end_buf && !strchr(whitespace, *s))
             	s++;
        	*s = 0;
    	}
	//then call executeInput giving it the input and how much there is
	for(int j = 0; j <= i; j++){
		if(strcmp(words_on_line[j], "|") == 0){
			pipe = true;
		}
	}
    	executeInput(words_on_line, i, pipe);
    	//for (int j = 0; j < i; j++)
          //  	printf("words_on_line[%d]: %s\n", j, words_on_line[j]);
    	//if (strcmp(words_on_line[0],"stop") == 0)
         //	exit(-1);
    }
}
void handleSigInt(int sig){
        printf("caught signal %d\n", sig);
}
void end(){
        exit(-1);
}

int handleLS(int wordNum, char *words[], int curWord, bool pipe) {
    printf("curWord is %d\n", curWord);
    DIR *dir;
    struct dirent *ent;
	int startWord = curWord;	
	    //for (int z = 0; z < 10; z++) {
	while(curWord <= wordNum){
		printf("in the while\n");
	   if (words[curWord] != "&" && pipe == false|| words[curWord] <= wordNum && pipe == false){
		   printf("in the if\n");

		// Open the current directory or a chosen directory as an argument
		if (wordNum == 1 || strcmp(words[curWord+1], "")) {
		    dir = opendir(".");
		} else if (wordNum == 2 || strcmp(words[curWord+2], "")) {
		    dir = opendir(words[1]);
		} else {
		    printf("Usage: ls [dir]\n");
		    exit(1);
		}
		// if no directory it throws the error message
		if (dir == NULL) {
		    printf("Error: Cannot open directory\n");
		    break;
		}

		// Loop through all the entries in the directory
		while ((ent = readdir(dir)) != NULL) {
		    printf("%s\n", ent->d_name);
		}

		// Close the directory
		closedir(dir);
		return 0;
	    } else if (words[curWord] == "&" && pipe == false) {
		//Run process as background
		printf("in the else if\n");
		int bgProcess = fork();
		printf("%s running in background", &bgProcess);	
	    } else if (pipe == true) {
		//Run pipe command
		if(fork() == 0){
			printf("in child, curWord is %d\n", curWord);
			//child calls LS
			if (strcmp(words[curWord+1],"") == 0 || strcmp(words[curWord+1],"|")==0) {
		                printf("in the first if\n");
				dir = opendir(".");
                        } else if (strcmp(words[curWord+2],"") == 0 || strcmp(words[curWord+2],"|")==0) {
				printf("in the second if\n");
				dir = opendir(words[startWord+1]);
	                } else {
 	                        printf("Usage: ls [dir]\n");
	                        exit(1);
	                }
                        // if no directory it throws the error message
	                if (dir == NULL) {
	                        printf("Error: Cannot open directory\n");
				kill(getpid(), SIGINT);
                        }
                        // Loop through all the entries in the directory
                        while ((ent = readdir(dir)) != NULL) {
	                        printf("%s\n", ent->d_name);
                        }
                        // Close the directory
                        closedir(dir);
			pipe = false;
			kill(getpid(), SIGINT);
			
		}else{
			//parent calls whatever it needs to
			printf("in parent");
			return curWord;
		}


	    }
	printf("about to iterate curWord\n");
	curWord++;
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
