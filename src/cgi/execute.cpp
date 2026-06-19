#include "webserv.hpp"
#include "sys/wait.h"

static void  execute_child(const char *filename, int stdin_pipe[2], int stdout_pipe[2], char **env)
{
  char *executable = (char *)"/usr/bin/python3";
  char *argv[] = {executable, const_cast<char*>(filename), NULL};
  close(stdin_pipe[1]);
  close(stdout_pipe[0]);
  dup2(stdin_pipe[0], STDIN_FILENO);
  dup2(stdout_pipe[1], STDOUT_FILENO);
  execve(executable, argv, env);
  exit(ERROR);
}

static std::string get_cgi_output(int pid, int output_fd)
{
  ssize_t     n;
  char        buf[4096];
  std::string output;

  while ((n = read(output_fd, buf, sizeof(buf))) > 0)
    output.append(buf, n);
  if (n == ERROR)
    return "";
  int status;
  if (waitpid(pid, &status, 0) == ERROR)
    return "";
  if (WIFEXITED(status) == SUCCESS || WEXITSTATUS(status) != SUCCESS)
    return "";
  return output;
}

std::string execute_cgi(const char *filename, char **env)
{
  int   stdin_pipe[2];
  int   stdout_pipe[2];

  if (pipe(stdin_pipe) == ERROR || pipe(stdout_pipe) == ERROR)
    return "";

  pid_t pid = fork();
  if (pid == -1)
    return "";
  else if (pid == 0)
    execute_child(filename, stdin_pipe, stdout_pipe, env);

  close(stdin_pipe);

  close(stdout_pipe[1]);
  std::string output = get_cgi_output(pid, stdout_pipe[0]);
  close(stdout_pipe[0]);
  return output;
}
