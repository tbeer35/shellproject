#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

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
