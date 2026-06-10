#include "webserv.hpp"
#include "sys/wait.h"

void  execute_child(const char *filename, int stdin_pipe[2], int stdout_pipe[2], char **env)
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

//return SUCCESS even if cgi_data is empty
bool write_to_child(char *cgi_data, int stdin_pipe[2], int stdout_pipe[2])
{
  if (cgi_data && cgi_data[0])
  {
    ssize_t len = strlen(cgi_data);
    if (write(stdin_pipe[1], cgi_data, len) != len)
    {
      close(stdin_pipe[1]);
      close(stdout_pipe[0]);
      return false;
    }
  }
  return true;
}

std::string get_cgi_output(int pid, int output_fd)
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

std::string execute_cgi(const char *filename, char **env, char *cgi_data)
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

  close(stdin_pipe[0]);
  close(stdout_pipe[1]);

  if (!write_to_child(cgi_data, stdin_pipe, stdout_pipe))
    return "";
  close(stdin_pipe[1]);

  std::string output = get_cgi_output(pid, stdout_pipe[0]);
  close(stdout_pipe[0]);
  return output;
}

//CGI specs (RFC 3875) instructs script data to be passed through stdin and metadata from env
//the output of the CGI must be a header, a blank line then a body with the output
int main()
{
  std::string cgi_dir = "cgi/";
  char *env[] = {
      (char*)"REQUEST_METHOD=GET",
      (char*)"QUERY_STRING=name=Alice",
      NULL
  };
  for (char i = '1'; i <= (char)'7'; i++)
  {
    std::string full_path = cgi_dir + i + ".py";
    if (execute_cgi(full_path.c_str(), env, NULL) == "")
    {
      print("failure");
      return FAILURE;
    }
    print(full_path + ": success");
  }
  return SUCCESS;
}
