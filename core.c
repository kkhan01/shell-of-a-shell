#include "headers.h"

static void sighandler(int signo){
  //user wants to exit
  if (signo == SIGINT){
    printf("exiting due to sigint :( cya later\n");
    exit(0);
  }
}

int num_separated(char *line, char *separator){
  int i;
  char *copy = malloc(sizeof(char*) * strlen(line));
  strcpy(copy, line);
  for(i = 0; copy; i++){
    strsep(&copy, separator);
  }
  return i;
}

char **separate_commands(char *line, char *separator){
  int i = num_separated(line, separator);
  char **commands = calloc(i+1, sizeof(char **));
  for(i = 0; line; i++){
    commands[i] = strsep(&line, separator);
  }
  return commands;
}
/*
  char **parse_args(char *line, char * separator){
  int i = num_separated(line, separator);
  char **commands = calloc(i+1, sizeof(char **));
  for(i = 0; line; i++){
  commands[i] = strsep(&line, separator);
  }
  return commands;
  }
*/
int cd(char **args){
  if(!chdir(args[1])){
    return 0;
  }
  //printf("Error number: %d\n", errno);
  printf("%s\n",strerror(errno));
  return 1; //shows problematic, but keeps shell running hopefully
}

//NEEDS EXECSSIVE TESTING
int execute(char **args){
  if(!strcmp(args[0],"exit")){
    printf("/nlogout\n");
    sleep(1);
    exit(0);
  }
  if(!strcmp(args[0], "cd")){
    return cd(args);
  }
  int status;
  int f = fork();
  if(!f){
    execvp(args[0], args);
  }
  else{
    wait(&status);
  }
  return 1;
}

void redir(char * file, int destination){
  int new_fd = open(file, O_RDWR | O_CREAT, 0666);
  int x = dup(destination);
  //printf("destination: %d\t new fd: %d\n", destination, new_fd);
  dup2(new_fd, destination);
  dup2(x, destination);
  close(new_fd);
}

void pipin(char * first, char * second){
  FILE * fp1 = popen(first, "r");
  FILE * fp2 = popen(second, "w");
  //char path[1000];
}

char * trim(char * bush){
  if(bush[0] == " "){
    int i = 0;
    int len = strlen(bush);
    for(; i < len; i++){
      bush[i] = bush[i+1];
    }
  }
  return bush;
}

//started func to run single command, must come back to edit
void command(char * cmd){
  char *c = ">";
  //checking for redirectional stuffies
  if(strchr(cmd, *c) != NULL){
    //printf("DEUGGING\n");
    char **cmds = separate_commands(cmd,">");
    //printf("%s\t\n", cmds[3]);
    cmds[1] = trim(cmds[1]);
    int i = 0;
    for(; i < 5; i++){
      printf("%s\n", cmds[i]);
    }
    redir(cmds[3], STDOUT_FILENO);
  }
  c = "<";
  if(strchr(cmd, *c) != NULL){
    char **cmds = separate_commands(cmd, " < ");
    redir(cmds[3], STDIN_FILENO);
  } /*c = "|"
  if(strchr(cmd, *p) != NULL){
    char **cmds = separate_commands(cmd, " | ");
    printf("commands: %s\t%s\t%s\n", cmd[0], cmd[1], cmd[2]);
  }*/
  //ADD REST OF RUNNNING COMMAND OPTIONS
}
