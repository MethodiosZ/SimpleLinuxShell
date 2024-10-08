#include <stdio.h>
#include <stdlib.h>
//#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

void command_promt(){
    static int new_promt = 1;
    if(new_promt){
        const char* new_window = " \e[1;1H\e[2j";
        write(STDOUT_FILENO,new_window,12);
        new_promt=0;
    }
    printf("$");
}

void read_command(char* command,char** parameters){
    char* line,*pch;
    int count = 0, i = 0, j = 0;
    char** array;
    for(;;){
        int c = fgetc(stdin);
        line[count++] = (char) c;
        if(c=='\n') break;
    }
    if(count==1) return;
    pch = strtok(line," \n");
    while(pch!=NULL){
        array[i++] = strdup(pch);
        pch = strtok(NULL," \n");
    }
    strcpy(command,array[0]);
    for(int j=0;j<i;j++){
        parameters[j]  = array[j];
    }
    parameters[i]=NULL;
}

void waitpid(int x,int *status,int y){
    wait(NULL);
}

int main(int argc,char **argv){
    char* cmd,*command;
    char** parameters;
    char *envv[] = {(char*)"PATH=/bin",0};
    int status;
    while(1){
        command_promt();
        read_command(command,parameters);
        int pid = fork();
        if(pid==0){
            strcpy(cmd,"/bin/");
            strcat(cmd,command);
            execve(cmd,parameters,envv);
        } else if(pid>0){
            waitpid(-1,&status,0);
        } else {
            printf("Fork not executed successfully!\n");
        }
        if(strcmp(command,"exit")==0) break;
    }
    return 0;
}