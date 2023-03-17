#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <dirent.h>

void end();
void handleSigInt(int sig);
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
			handleLS();
                }else if(strcmp(words[x], "cd")==0){
			handleCD();
		}else if(strcmp(words[x], "dog")==0){
                	handleDog();
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

int handleLS(int argc, char *argv[]) {
    DIR *dir;
    struct dirent *ent;

    // Open the current directory or a chosen directory as an argument
    if (argc == 1) {
        dir = opendir(".");
    } else if (argc == 2) {
        dir = opendir(argv[1]);
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
}

int handleCD(int argc, char *argv[]) {

   // chack to see if number of arguments is right
   if (argc != 2) {
     printf("Please format it as: %s <directory>\n", argv[0]);
     exit(EXIT_FAILURE);
   }
   // attempt to change directory to the given argument
   if (chdir(argv[1]) == -1) {
      printf("no %s directory found\n", argv[1]);
      exit(EXIT_FAILURE);
   }

   if (argv[1] == "..") {
      chdir("..");
   }else{
      chdir(argv[1]);
   }

   // print the new current working directory
   char *cwd = getcwd(NULL, 0);
   printf("Current working directory: %s\n", cwd);
   free(cwd);

   return 0;
}

int handleDog() {
    char filename[100];
    char text[100];
    FILE *file_ptr;

    printf("Enter file name: ");
    scanf("%s", filename);

    file_ptr = fopen(filename, "w"); // open/create file in write mode

    if (file_ptr == NULL) { // check if file was opened successfully
        printf("Error opening file.");
        return 1;
    }

    printf("What would you like to write in the file: ");
    scanf("%s", text);

    fprintf(file_ptr, text); // write to file

    fclose(file_ptr); // close file

    return 0;
}

