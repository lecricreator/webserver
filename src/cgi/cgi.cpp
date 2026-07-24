#include "cgi.hpp"

std::string create_response(const t_cgi_info &cgi_info)
{
	std::string client_response;
	if (cgi_info.status == EMPTY_FIELD)
		client_response = "HTTP/1.1 200 OK";
	else
		client_response = "HTTP/1.1 " + cgi_info.status;
	client_response.append("\n");
	client_response.append("Content-Type: " + cgi_info.content_type);
	client_response.append("\n");
	client_response.append("Content-Length: " + to_str((int)cgi_info.body.size()));
	client_response.append("\n");
	client_response.append("\n");
	client_response.append(cgi_info.body);
	return client_response;
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

	if ((cgi_output = execute_cgi(full_path.c_str(), env, NULL)) == "")
		return print(full_path + ": failure"), FAILURE;
	if (parse_cgi(cgi_output, cgi_info) == FAILURE)
		response = "502 Bad gateway";
	else if (is_implemented(cgi_info) == FAILURE)
		response = "501 Not implemented";
	else if (status == EMPTY_FIELD || status == "200 OK")
		response = create_response(cgi_info);
	else
		response = "fetch correspondent status page";
	return SUCCESS;
}
