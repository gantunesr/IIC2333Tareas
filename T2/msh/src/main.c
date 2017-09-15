#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>


int num_builtins();
int msh_exit(char **args);
int msh_set_path(char **args);
int msh_set_prompt(char **args);

char PROMPT[] = "Ingresa un comando:";
char *builtin_str[] = {"setPrompt", "setPath", "exit"};
int (*builtin_func[]) (char **) = { &msh_set_prompt, &msh_set_path, &msh_exit};

char* read_line(void) {
  char *line = NULL;
  size_t bufsize = 0;
  getline(&line, &bufsize, stdin);
  return line;
}

char **split_line(char *line, int *command_tokens){
  int bufsize = 64;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  token = strtok(line, " \n");
  while (token != NULL) {
    tokens[*command_tokens] = token;
    *(command_tokens) = *(command_tokens) + 1;
    if (*command_tokens >= bufsize) {
      bufsize += 64;
      tokens = realloc(tokens, bufsize * sizeof(char*));
    }
    token = strtok(NULL, " \n");
  }
  tokens[*command_tokens] = NULL;
  return tokens;
}

int ctrlc = 0;
void intHandler(int dummy) {
  ctrlc =1;
}

void intHandlerFather(int dummy) {
  printf("\n" );
  exit(0);
}

int status_child;
int process(char **args, int *command_tokens) {
  pid_t  wpid;
  pid_t pid;
  int in_background = strcmp(args[*command_tokens-1],"&");
  pid = fork();
  signal(SIGINT, intHandler);
  if (pid == 0) {
    if (in_background == 0) {args[*command_tokens - 1] = NULL;}
    if (execvp(args[0], args) == -1) {
      perror("Error");
      exit(0);
    }
  } else {
    //en
    do {
      if (in_background != 0) {
        wpid = waitpid(pid, &status_child, WUNTRACED);
      }
    } while (!WIFEXITED(status_child)  && in_background != 0 && !ctrlc);
    //WIFSIGNALED  1 si child termino con por signal
    //WIFEXITED  1 si child termino normalmente con exit por ejemplo
  }

  return 1;
}

int execute(char **args, int *command_tokens) {
  //si no se ingresa nada
  if (args[0] == NULL) {
    return 1;
  }
  //ver si ingresa alguno de los built ins
  for (int i = 0; i < num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }
  return process(args, command_tokens);
  }

int num_builtins() {return sizeof(builtin_str) / sizeof(char *);}

int msh_set_prompt(char **args) {
  printf("%s\n", args[1]);
  strcpy(PROMPT, args[1]);
  int i = 2;
  while (args[i] != NULL) {
    strcat(PROMPT, " ");
    strcat(PROMPT, args[i]);
    i ++;
  }
  strcat(PROMPT, ":");
  return 0;
}

int msh_set_path(char **args) {
  strcpy(PROMPT, args[1]);
  printf("%s\n", args[1]);
  return 0;
}

int msh_exit(char **args) { return 0;}


int main() {
  int last_arg, N;
  char *line;
  char **args;
  int status = 1;

  while (status) {
    ctrlc = 0;
    printf("%s ", PROMPT);
    //parseo linea
    signal(SIGINT, intHandlerFather);
    line = read_line();
    //obtener palabras
    int command_tokens = 0;
    args = split_line(line, &command_tokens);
    last_arg = (sizeof(args)/sizeof(args[0])) + 1;

    if (ctrlc == 1) {return 0;};
    //ejecutar comando

    if (strcmp(args[last_arg], "&")) {

    }
    else if (strcmp(&args[last_arg][1], "&")) {
      N = atoi(&args[last_arg][0]);
      for (int k = 0; k < N; k++) {
        status = execute(args, &command_tokens);
      }
    }
    else {
      status = execute(args, &command_tokens);
    }
    free(line);
    free(args);
  };
  return 0;
}
