#include "webserv.hpp"
#include "sys/wait.h"

int execute_cgi(char *filename, char **env, char *cgi_data)
{
  int stdin_pipe[2];
  int stdout_pipe[2];

  pipe(stdin_pipe);
  pipe(stdout_pipe);

  pid_t pid = fork();
  if (pid == 0)
  {
    char *executable = (char *)"/usr/bin/python3";
    char *argv[] = {executable, filename, NULL};
    close(stdin_pipe[1]);
    close(stdout_pipe[0]);
    dup2(stdin_pipe[0], STDIN_FILENO);
    dup2(stdout_pipe[1], STDOUT_FILENO);
    execve(executable, argv, env);
    exit(1);
  }
  else
  {
    close(stdin_pipe[0]);
    close(stdout_pipe[1]);
    if (cgi_data)
      write(stdin_pipe[1], cgi_data, sizeof(cgi_data));
    close(stdin_pipe[1]);
    char buf[4096];
    //read(stdout_pipe[0], buf, sizeof(buf));
    ssize_t n;
    while ((n = read(stdout_pipe[0], buf, sizeof(buf))) > 0)
      write(STDOUT_FILENO, buf, n);
    close(stdout_pipe[0]);
    waitpid(pid, NULL, 0);
  }
  return SUCCESS;
}

//CGI specs (RFC 3875) instructs script data to be passed through stdin and metadata from env
//the output of the CGI must be a header, a blank line then a body with the output
int main()
{
  char *env[] = {
      (char*)"REQUEST_METHOD=GET",
      (char*)"QUERY_STRING=name=Alice",
      NULL
  };
  if (execute_cgi((char *)"cgi_script.py", env, NULL) == ERROR)
    return FAILURE;
  return SUCCESS;
}
