#include "cgi.hpp"

std::string create_response(const std::string &status_value,
                            const std::string &content_type_value,
                            const std::string &body
                            )
{
  if (status_value.empty() || content_type_value.empty() || body.empty())
    return std::string();
  std::string status = "HTTP/1.1 ";
  std::string content_type = "Content-Type: ";
  std::string content_lenght = "Content-Length: ";

  std::string content_lenght_value = to_str(body.size());

  std::string end_line = "\r\n";

  std::string response;
  response  = status                + status_value          + end_line
            + content_type          + content_type_value    + end_line
            + content_lenght        + content_lenght_value  + end_line
            + end_line
            + body + end_line;
	return response;
}

t_cgi_info init_cgi_info()
{
	t_cgi_info cgi_info;

	cgi_info.content_type = EMPTY_FIELD;
	cgi_info.status = EMPTY_FIELD;
	cgi_info.body = EMPTY_FIELD;

	return cgi_info;
}

int is_content_type_implemented(const std::string &content_type)
{
	if (content_type.find("text") != std::string::npos)
		return SUCCESS;
	return FAILURE;
}

int is_implemented(const t_cgi_info &cgi_info)
{
	if (is_content_type_implemented(cgi_info.content_type) == SUCCESS)
		return SUCCESS;
	return FAILURE;
}

//normally we might reuse the body of other codes than 200 but we can simplify
int get_cgi_response(std::string &response, const std::string &full_path)
{
	std::string cgi_output;
	char *env[] = {
			(char*)"REQUEST_METHOD=GET",
			(char*)"QUERY_STRING=name=Alice",
			NULL
	};
	t_cgi_info cgi_info = init_cgi_info();
	std::string status = cgi_info.status;

  if (cgi_info.status == EMPTY_FIELD)
    cgi_info.status = "200 OK";

	if ((cgi_output = execute_cgi(full_path.c_str(), env, NULL)) == "")
		return print(full_path + ": failure"), FAILURE;
	if (parse_cgi(cgi_output, cgi_info) == FAILURE)
		response = "502 Bad gateway";
	else if (is_implemented(cgi_info) == FAILURE)
		response = "501 Not implemented";
	else if (status == "200 OK")
    response = create_response(cgi_info.status, cgi_info.content_type, cgi_info.body);
	else
		response = "fetch correspondent status page";
	return SUCCESS;
}
