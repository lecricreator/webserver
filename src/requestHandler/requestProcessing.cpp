#include "webserv.hpp"
#include "cgi.hpp"

t_response_data  httpRequest::executeRequest(Server &server)
{
  std::cout << "executeRequest() call\n";

  t_response_data data;
  std::string     script_name;
  bool is_cgi_script = is_cgi(_path, script_name);
  //script name useful for env vars
  if (_errorCode == 0)
    setErrorCode(200);

  if (_status == REQ_PARSED)
  {
    if (_method == "GET")
    {
      if (!getRequest(server, data.content_type, is_cgi_script))
      {
        data.status = to_str((int)_errorCode) + " " + code_to_string(_errorCode);
        _status = REQ_EXECUTED; //ideally this should be REQ_ERROR but whatever
        return data;
      }
      _status = REQ_EXECUTED;
    }
    if (_method == "POST")
    {
      postRequest();
      _status = REQ_EXECUTED;
    }
    //if (_method == "DELETE")
    //{
    //  if (!deleteRequest())
    //  {
    //    data.status = to_str((int)_errorCode) + " " + code_to_string(_errorCode);
    //    _status = REQ_EXECUTED; //ideally this should be REQ_ERROR but whatever
    //    return data;
    //  }
    //  _status = REQ_EXECUTED;
    //}
  }

  if (!is_cgi_script)
  {
    data.status = to_str((int)_errorCode) + " " + code_to_string(_errorCode);
    data.content_type = "text/html";
    data.body = _responseBody;
  }
  else
    _errorCode = cgi(_path, data.status, data.content_type, data.body);

  bool is_debug = true;
  if (is_debug)
  {
    print(data.status);
    print(data.content_type);
    print(data.body);
  }
  return data;
}

