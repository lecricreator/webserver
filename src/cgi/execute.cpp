#include "cgi.hpp"

static void  execute_child(std::string path, int stdin_pipe[2], int stdout_pipe[2], char **env)
{
  char *executable = (char *)"/usr/bin/python3";
  char *argv[] = {executable, const_cast<char*>(path.c_str()), NULL};
  if (chdir(WWW_ROOT CGI_ROOT) == ERROR)
    exit(ERROR);
  close(stdin_pipe[1]);
  close(stdout_pipe[0]);
  dup2(stdin_pipe[0], STDIN_FILENO);
  dup2(stdout_pipe[1], STDOUT_FILENO);
  execve(executable, argv, env);
  exit(ERROR);
}

//return SUCCESS even if parse_data is empty
static int write_to_child(int stdin_pipe[2], int stdout_pipe[2], const char *cgi_input_data)
{
  if (cgi_input_data && cgi_input_data[0])
  {
    ssize_t len = strlen(cgi_input_data);
    if (write(stdin_pipe[1], cgi_input_data, len) != len)
    {
      close(stdin_pipe[1]);
      close(stdout_pipe[0]);
      return ERROR;
    }
  }
  return SUCCESS;
}

std::string get_cgi_output(int pid, int output_fd)
{
  ssize_t     bytes_read;
  char        buf[4096];
  std::string output;

  while ((bytes_read = read(output_fd, buf, sizeof(buf))) > 0)
    output.append(buf, bytes_read);
  if (bytes_read == ERROR)
    return std::string();
  int status;
  if (waitpid(pid, &status, 0) == ERROR)
    return std::string();
  if (WIFEXITED(status) == SUCCESS || WEXITSTATUS(status) != SUCCESS)
    return std::string();
  return output;
}

int execute_parent(int stdin_pipe[2], int stdout_pipe[2], const char *cgi_input_data)
{
  close(stdin_pipe[0]);
  close(stdout_pipe[1]);

  if (write_to_child(stdin_pipe, stdout_pipe, cgi_input_data) == ERROR)
    return ERROR;
  close(stdin_pipe[1]);
  return SUCCESS;
}

int execute_cgi(std::string path, char **env, const char *cgi_input_data, t_parse_data &parse_data)
{
  int   stdin_pipe[2];
  int   stdout_pipe[2];

  if (pipe(stdin_pipe) == ERROR || pipe(stdout_pipe) == ERROR)
    return ERROR;

  pid_t pid = fork();
  if (pid == -1)
    return ERROR;
  else if (pid == 0)
    execute_child(path, stdin_pipe, stdout_pipe, env);
  if (execute_parent(stdin_pipe, stdout_pipe, cgi_input_data) == ERROR)
    return ERROR;
  print("cgi fd:");
  print(stdout_pipe[0]);
  parse_data.cgi_fd = stdout_pipe[0];
  parse_data.cgi_pid = pid;
  return SUCCESS;
}
