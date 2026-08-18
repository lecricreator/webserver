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
  free_env(env);
  exit(ERROR);
}

size_t ft_strlen(const char *s)
{
  const char *start = s;
  while (*s) s++;
  return s - start;
}

//return SUCCESS even if cgi_data is empty
static int write_to_child(char *cgi_data, int stdin_pipe[2], int stdout_pipe[2])
{
  if (cgi_data && cgi_data[0])
  {
    ssize_t len = ft_strlen(cgi_data);
    if (write(stdin_pipe[1], cgi_data, len) != len)
    {
      close(stdin_pipe[1]);
      close(stdout_pipe[0]);
      return ERROR;
    }
  }
  return SUCCESS;
}

static std::string get_cgi_output(int pid, int output_fd)
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

std::string execute_parent(int stdin_pipe[2], int stdout_pipe[2], char *cgi_data, pid_t pid)
{
  close(stdin_pipe[0]);
  close(stdout_pipe[1]);

  if (write_to_child(cgi_data, stdin_pipe, stdout_pipe) == ERROR)
    return std::string();
  close(stdin_pipe[1]);

  std::string output = get_cgi_output(pid, stdout_pipe[0]);
  close(stdout_pipe[0]);
  return output;
}

std::string execute_cgi(std::string path, char **env, char *cgi_data)
{
  int   stdin_pipe[2];
  int   stdout_pipe[2];

  if (pipe(stdin_pipe) == ERROR || pipe(stdout_pipe) == ERROR)
    return std::string();

  pid_t pid = fork();
  if (pid == -1)
    return std::string();
  else if (pid == 0)
    execute_child(path, stdin_pipe, stdout_pipe, env);
  return execute_parent(stdin_pipe, stdout_pipe, cgi_data, pid);
}
