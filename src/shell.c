#define _POSIX_C_SOURCE 1
#include "../include/msgs.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <ctype.h>
#include <stdbool.h>
// For History
char array[10][1024];
int arraySize = 0;
int cmdCount = -1;
// For control c
int ctrlC = 0;
int histErr = 0;
// For checking whether or not code executed successfully
int codeExecuted = 0;
char passDir[1024];
char tmp[1024];


bool numCheck(char *str){

  for(int i = 0; i<strlen(str); i++){
    if(!isdigit((char)str[i]) && str[i] != '\n'){
      return false;
    }
  }

  return true;

}

// For handling the exit internal command
void signalHandler(int signal) {
  if (signal == SIGINT) {
    // clean input
    ctrlC = 1;

      char *msg1 = FORMAT_MSG("help",HELP_HELP_MSG);
      write(STDOUT_FILENO,msg1,strlen(msg1));
      char *msg2 = FORMAT_MSG("cd", CD_HELP_MSG);
      write(STDOUT_FILENO,msg2,strlen(msg2));
      char *msg3 = FORMAT_MSG("exit",EXIT_HELP_MSG);
      write(STDOUT_FILENO,msg3,strlen(msg3));
      char * msg4 = FORMAT_MSG("pwd",PWD_HELP_MSG);
      write(STDOUT_FILENO,msg4,strlen(msg4));
      char *msg5 = FORMAT_MSG("history",HISTORY_HELP_MSG);
      write(STDOUT_FILENO,msg5,strlen(msg5));
    return;
  }
}

// Handles internal commands
void internalCommand(char **arg,int cnt) {

  if (strcmp(arg[0], "exit") == 0) {
    if (arg[1] != NULL) {
      char *msg = FORMAT_MSG("exit", TMA_MSG);
      write(STDERR_FILENO, msg, strlen(msg));
      return;
    }

    else {
      exit(0);
    }

  }

  else if (strcmp(arg[0], "pwd") == 0) {
    char cc[1024];
    for(int i =0;i<1024;i++){
    cc[i] = '\0';
    }
    
    if(arg[1]!=NULL){
      char *msg = FORMAT_MSG("pwd", TMA_MSG);
      write(STDERR_FILENO,msg,strlen(msg));
      return;
    }
    else{
      if(getcwd(cc,1024) != NULL){
        write(STDOUT_FILENO, cc, strlen(cc));
        write(STDOUT_FILENO,"\n",1);
        //write(STDOUT_FILENO,"$ ",2);
        return;
      }
    
      else{
        char *msg = FORMAT_MSG("pwd", GETCWD_ERROR_MSG);
        write(STDERR_FILENO,msg,strlen(msg));
        return;
      }
    }
    
  }

  else if (strcmp(arg[0], "cd") == 0) {
    if(arg[1]==NULL){
      if(getcwd(tmp,sizeof(tmp))!=NULL){
        strncpy(passDir,tmp,sizeof(passDir)-1);
      }
      struct passwd *p = getpwuid(getuid());
      char *home = p->pw_dir;
      if(chdir(home)<0){
        char* msg = FORMAT_MSG("cd", CHDIR_ERROR_MSG);
        write(STDERR_FILENO,msg,strlen(msg));
      }
    }
    else if(arg[1][1]=='/' || arg[1][0] == '/'){

        if(getcwd(tmp,sizeof(tmp))!=NULL){
          strncpy(passDir,tmp,sizeof(passDir)-1);
        
        write(STDOUT_FILENO,passDir,strlen(passDir));
        }
        else {
          write(STDOUT_FILENO,"failed",6);
        }
        char *tok = strtok(arg[1],"/");
        int count = 0;
        char *dir[1024];
        while(tok!=NULL){
          dir[count] = tok;
          tok = strtok(NULL,"/");
          count = count + 1;
        }
        //write(STDOUT_FILENO,count,1);
        struct passwd *p = getpwuid(getuid());
        char *home = p->pw_dir;
        if(chdir(home)<0){
          char* msg = FORMAT_MSG("cd", CHDIR_ERROR_MSG);
          write(STDERR_FILENO,msg,strlen(msg));
        }
        for(int i=1;i<count;i++){
          if(chdir(dir[i])<0){
            char* msg = FORMAT_MSG("cd", CHDIR_ERROR_MSG);
            write(STDERR_FILENO,msg,strlen(msg));
          };
        }




      }
      else if(strcmp(arg[1],"~")==0){
        if(getcwd(tmp,sizeof(tmp))!=NULL){
          strncpy(passDir,tmp,sizeof(passDir)-1);
        }
        struct passwd *p = getpwuid(getuid());
        char *home = p->pw_dir;
        if(chdir(home)<0){
        char* msg = FORMAT_MSG("cd", CHDIR_ERROR_MSG);
        write(STDERR_FILENO,msg,strlen(msg));
        }
      }

      else if(strcmp(arg[1],"-")==0){
        if(strlen(passDir)>0){

          if(chdir(passDir)<0){
            char* msg = FORMAT_MSG("cd", CHDIR_ERROR_MSG);
            write(STDERR_FILENO,msg,strlen(msg));
          }
        }

      }
      
      else{
       if(chdir(arg[1])<0){
        char *msg = FORMAT_MSG("cd", CHDIR_ERROR_MSG);
        write(STDERR_FILENO,msg,strlen(msg));
       }
      }
    }


  else if (strcmp(arg[0], "help") == 0) {
    if(cnt>2){
      char *msg = FORMAT_MSG("help", TMA_MSG);
      write(STDERR_FILENO,msg,strlen(msg));
      return;
    }
    else if(cnt>1){
      if(strcmp(arg[1],"pwd")==0){
        char * msg = FORMAT_MSG("pwd",PWD_HELP_MSG);
        write(STDOUT_FILENO,msg,strlen(msg));
      }
      else if(strcmp(arg[1],"cd")==0){
        char *msg = FORMAT_MSG("cd",CD_HELP_MSG);
        write(STDOUT_FILENO,msg,strlen(msg));
      }
      else if(strcmp(arg[1],"exit")==0){
        char *msg = FORMAT_MSG("exit",EXIT_HELP_MSG);
        write(STDOUT_FILENO,msg,strlen(msg));
      }
      else if(strcmp(arg[1],"help")==0){
        char *msg = FORMAT_MSG("help",HELP_HELP_MSG);
        write(STDOUT_FILENO,msg,strlen(msg));
      }
      else if(strcmp(arg[1],"history")==0){
        char *msg = FORMAT_MSG("history",HISTORY_HELP_MSG);
        write(STDOUT_FILENO,msg,strlen(msg));
      }
      else{
        int length = strlen(arg[1]) + 2;
        strncat(arg[1],": ",length);
        strncat(arg[1],EXTERN_HELP_MSG,length + strlen(EXTERN_HELP_MSG));
        write(STDOUT_FILENO,arg[1],strlen(arg[1]));
        write(STDOUT_FILENO,"\n",1);
      }

      return;
  
    }

      
    else{
      char *msg1 = FORMAT_MSG("help",HELP_HELP_MSG);
      write(STDOUT_FILENO,msg1,strlen(msg1));
      char *msg2 = FORMAT_MSG("cd", CD_HELP_MSG);
      write(STDOUT_FILENO,msg2,strlen(msg2));
      char *msg3 = FORMAT_MSG("exit",EXIT_HELP_MSG);
      write(STDOUT_FILENO,msg3,strlen(msg3));
      char * msg4 = FORMAT_MSG("pwd",PWD_HELP_MSG);
      write(STDOUT_FILENO,msg4,strlen(msg4));
      char *msg5 = FORMAT_MSG("history",HISTORY_HELP_MSG);
      write(STDOUT_FILENO,msg5,strlen(msg5));

      return ;
      }
    }
  else if(strcmp(arg[0],"history")==0){
    int j = 0;
    for(int i = arraySize-1;i>=0;i--){
      int num = cmdCount - j;
      char str[20];
      sprintf(str,"%d",num);
      int length = strlen(str) + 1;
      strncat(str,"\t",length);
      strncat(str,array[i],length+strlen(array[i]));
      write(STDOUT_FILENO,str,strlen(str));
      write(STDOUT_FILENO,"\n",1);
      j += 1;
    }

  }
}
  

int main() {
  struct sigaction handler;
  handler.sa_handler = signalHandler;
  handler.sa_flags = 0;
  sigemptyset(&handler.sa_mask);
  sigaction(SIGINT,&handler,NULL);
  
  // Directory
  char cwd[100];

  // User input command
  char cmd[1024];

  // Remove garbage values
  for (int i = 0; i < 100; i++) {
    cmd[i] = '\0';
    cwd[i] = '\0';
  }

  // Shell loop
  while (1) {
    histErr = 0;
    // while(waitpid(-1,NULL,WNOHANG)!=0){}
    ctrlC = 0;
    // Get cwd
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      write(STDOUT_FILENO, cwd, strlen(cwd));
      write(STDOUT_FILENO, "$ ", 2);
    }
    // Get cwd failed
    else {
      char *msg = FORMAT_MSG("shell", GETCWD_ERROR_MSG);
      write(STDERR_FILENO, msg, strlen(msg));
    }

    // while(waitpid(-1,NULL,WNOHANG)>0){}
    //  Read from user data
    ssize_t inputC = read(STDIN_FILENO, cmd, 100);

    

    // Check if read correctly
    if (inputC < 0 && ctrlC==0) {
      char *msg = FORMAT_MSG("shell", READ_ERROR_MSG);
      write(STDERR_FILENO, msg, strlen(msg));
    }

    // Remove trailing following garbage values
    cmd[inputC - 1] = '\0';
    histErr = 0;
    if(cmd[0]=='!'){
      if(inputC>1&&cmd[1]=='!'&&array[arraySize-1]!=NULL&&arraySize!=0){
        write(STDOUT_FILENO,array[arraySize-1],sizeof(array[arraySize-1]));
        write(STDOUT_FILENO,"\n",1);
        strncpy(cmd,array[arraySize-1],sizeof(cmd)-1);
        //write(STDOUT_FILENO,"hi",2);
      }
      else if(arraySize==0){
        histErr = 1;
        char *msg = FORMAT_MSG("history",HISTORY_NO_LAST_MSG);
        write(STDERR_FILENO,msg,strlen(msg));


      }
      
      else{
        char *hTok = strtok(cmd,"!");
        int c = 0;
        char *hArg[1024];

        while(hTok != NULL){
          hArg[c] = hTok;
          hTok = strtok(NULL,"!");
          c = c + 1;
        }

        hArg[c] = NULL;


        if(numCheck(hArg[0])){        
//        write(STDOUT_FILENO,hArg[0],strlen((hArg[0])-1));
        if(arraySize>=10 && cmdCount >= atoi(hArg[0])&&atoi(hArg[0])>=(cmdCount-10)){

          write(STDOUT_FILENO,array[9-(cmdCount - atoi(hArg[0]))],strlen(array[10-(cmdCount - atoi(hArg[0]))]));
          write(STDOUT_FILENO,"\n",1);
          strncpy(cmd,array[9-(cmdCount - atoi(hArg[0]))],sizeof(cmd)-1);
       }
        else if(arraySize < 10 && cmdCount >= atoi(hArg[0])){
          write(STDOUT_FILENO,array[atoi(hArg[0])],strlen(array[atoi(hArg[0])]));
          write(STDOUT_FILENO,"\n",1);
          strncpy(cmd,array[atoi(hArg[0])],sizeof(cmd)-1);
          }
        else{
          char *msg =FORMAT_MSG("history",HISTORY_INVALID_MSG);
          write(STDERR_FILENO,msg,strlen(msg));
          histErr = 1;
        }
        }

        else{
          char *msg = FORMAT_MSG("history",HISTORY_INVALID_MSG);
          write(STDERR_FILENO,msg,strlen(msg));
          histErr = 1;

        }
      }
    }

    char preservedCMD[1024];
    strncpy(preservedCMD,cmd,sizeof(preservedCMD)-1);
    // Check if it is background process '&'
    int background = 0;
    if (cmd[inputC - 2] == '&') {
      background = 1;

      // Remove ampersand
      cmd[inputC - 2] = '\0';
    }

    // Tokenize command input for simple parsing
    char *tok = strtok(cmd, " ");
    int count = 0;
    char *arg[100];

    while (tok != NULL) {
      arg[count] = tok;
      tok = strtok(NULL, " ");
      count = count + 1;
    }

    // Remove trailing following garbage values
    arg[count] = NULL;

    //Check if argument is NULL or not
    if(arg[0]!=NULL && ctrlC == 0){

      if(histErr != 1){
      // For history function
      cmdCount += 1; 
      if(arraySize==10){
        for(int i=0;i<arraySize-1;i++){
          strncpy(array[i],array[i+1],sizeof(array[i])-1);
        }
        
        strncpy(array[arraySize-1],preservedCMD,sizeof(array[arraySize-1])-1);

      }
      else{
        strncpy(array[arraySize],preservedCMD,sizeof(array[arraySize]) - 1);
        arraySize += 1;

      }
      }


    //Only fork if it is not an internal command
    int internalC = 0;
    if(strcmp(arg[0],
          "exit")==0||strcmp(arg[0],"pwd")==0||strcmp(arg[0],"cd")==0||strcmp(arg[0],"help")==0||strcmp(arg[0],"history")==0){
      internalC = 1;
    }

    if(internalC == 0){
    // Create a child process
    pid_t pid = fork();

    // Check if child process was made
    if (pid == 0) {

      codeExecuted = 0;
      if(histErr != 1){
      if (execvp(arg[0], arg) < 0) {
        if(histErr ==0){
          
          char *msg = FORMAT_MSG("shell", EXEC_ERROR_MSG);
          write(STDERR_FILENO, msg, strlen(msg));
        }
        else{
        histErr = 0;
        }
      }
      }
      exit(0);
    }

      // while(waitpid(-1,NULL,WNOHANG)>0){}
    

    // Parent Process
    else if (pid > 0) {

      int wstatus = 0;

      // Foreground process do this
      if (background == 0) {
        if (waitpid(pid, &wstatus, 0) == -1) {
          char *msg = FORMAT_MSG("shell", WAIT_ERROR_MSG);
          write(STDERR_FILENO, msg, strlen(msg));
        }

        // For exit internal command
      }
    }

    // Failed fork
    else {
      char *msg = FORMAT_MSG("shell", FORK_ERROR_MSG);
      write(STDERR_FILENO, msg, strlen(msg));
    }

    // Kill zombie processes
     while(waitpid(-1,NULL,WNOHANG)>0);
  }
    
  else{
    internalCommand(arg,count);
  }
    }} 
}
