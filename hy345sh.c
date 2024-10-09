#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define COMMAND_LENGTH 1024
#define MAX_PARAMS  64

void command_promt(){
    static int new_promt = 1;
    if(new_promt){
      system("clear");
      new_promt=0;
    }
    printf("csd4384-hy345sh@user:id ");
}

void read_command(char* command,char** parameters){
  char* token;
  int i = 0;
  if(fgets(command,COMMAND_LENGTH,stdin)==NULL){
    printf("Failed to read input!\n");
    exit(EXIT_FAILURE);
  }
  token = strtok(command," \t\n");
  while(token!=NULL && i<MAX_PARAMS-1){
    parameters[i++]=token;
    token=strtok(NULL," \t\n");
  }
  parameters[i]=NULL;
}

/*void waitpid(int x,int *status,int y){
    wait(NULL);
}*/

int main(int argc,char **argv){
    char cmd[COMMAND_LENGTH],command[COMMAND_LENGTH];
    char* parameters[MAX_PARAMS];
    char *envv[] = {(char*)"PATH=/bin",0};
    int status;
    while(1){
        command_promt();
        read_command(command,parameters);
        /*pid_t pid = fork();
        if(pid==0){
            strcpy(cmd,"/bin/");
            strcat(cmd,command);
            execve(cmd,parameters,envv);
        } else if(pid>0){
            waitpid(-1,&status,0);
        } else {
            printf("Fork not executed successfully!\n");
        }*/
	int i=0;
	while(parameters[i]!=NULL){
	  printf("%s\n",parameters[i]);
	  i++;
	}
	if(strcmp(command,"exit")==0) break;
    }
    return 0;
}
