//Methodios Zacharioudakis

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define COMMAND_LENGTH 1024
#define MAX_PARAMS  64

void command_promt(){
    static int new_promt = 1;
    char* username;
    pid_t processid;
    username = getlogin();
    processid = getpid();
    if(new_promt){
      system("clear");
      new_promt=0;
    }
    printf("csd4384-hy345sh@%s:%d ",username,processid);
}

void parse_command(char* command,char** parameters){
  char* token;
  int i = 0;
  token = strtok(command," \t\n");
  while(token!=NULL && i<MAX_PARAMS-1){
    parameters[i++]=token;
    token=strtok(NULL," \t\n");
  }
  parameters[i]=NULL;
}

void execute_pipe(char** arg1,char** arg2){
  int fd[2];
  pipe(fd);
  if(fork()==0){
    dup2(fd[1],STDOUT_FILENO);
    close(fd[0]);
    close(fd[1]);
    execvp(arg1[0],arg1);
    exit(EXIT_FAILURE);
  }
  if(fork()==0){
    dup2(fd[0],STDIN_FILENO);
    close(fd[0]);
    close(fd[1]);
    execvp(arg2[0],arg2);
    exit(EXIT_FAILURE);
  }
  close(fd[0]);
  close(fd[1]);
  wait(NULL);
  wait(NULL);
}

void execute_command(char** parameters){
  pid_t pid = fork();
  if(pid<0){
    printf("Fork failed!\n");
  }  else if(pid==0) {
    int i,fd;
    for(i=0;parameters[i]!=NULL;i++){
      if(strcmp(parameters[i],">")==0){
	//fd = open(parameters[i+1]);
	dup2(fd,STDOUT_FILENO);
	close(fd);
	parameters[i]=NULL;
      } else if(strcmp(parameters[i],"<")==0){
	dup2(fd,STDIN_FILENO);
	close(fd);
	parameters[i]=NULL;
      } else if(strcmp(parameters[i],">>")==0){
	dup2(fd,STDOUT_FILENO);
	close(fd);
	parameters[i]=NULL;
      }
    }
    if(execvp(parameters[0],parameters)==-1){
      printf("Execution failed!\n");
      exit(EXIT_FAILURE);
    }
  } else wait(NULL);
  
}

void parse_pipes(char* cmd){
  char* params[MAX_PARAMS],*arg1[MAX_PARAMS],*arg2[MAX_PARAMS],*pipe;
  pipe = strstr(cmd,"|");
  if(pipe!=NULL){ // searches for pipes in the command and handles them 
    *pipe='\0';
    parse_command(cmd,arg1);
    parse_command(pipe+1,arg2);
    execute_pipe(arg1,arg2);
  } else { //parses the non pipe command
    parse_command(cmd,params);
    if(params[0]!=NULL) execute_command(params);
  }
}

void execute_multiple_commands(char* command){
  char* cmd;
  cmd = strtok(command,";"); //separetes the multiple commands with ; as delimeter
  while(cmd!=NULL){
    parse_pipes(cmd); // parses the single command
    cmd = strtok(NULL,":");
  }
}

int main(int argc,char **argv){
  char command[COMMAND_LENGTH];
  while(1){
    command_promt();
    if(fgets(command,COMMAND_LENGTH,stdin)==NULL){
      printf("Failed to read input!\n");
      exit(EXIT_FAILURE);
    }
    if(strcmp(command,"exit\n")==0) break; //Stops at exit command
    if(strstr(command,";")!=NULL) execute_multiple_commands(command); // Handles command that contains multiple commands separated by ;
    else parse_pipes(command); // parses the single command
  }
  return 0;
}
