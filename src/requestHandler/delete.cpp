#include "webserv.hpp"

unsigned int    httpRequest::deleteRequest(const Server &server)
{
	//std::cout << "deleteRequest() call\n";

	if (_errorCode == 200 || _errorCode == 0)
	{
		std::cout << _path <<"\n";

		if (can_requested(server, _method) == -1)
		{
			std::cout << _method << "lol\n";
			setErrorCode(405);
			return _errorCode;
		}
	}
	_path.erase(0, 1);
    if (remove(_path.c_str()) < 0)
    {
        if (errno == ENOENT) {
            setErrorCode(404);
        } else if (errno == EACCES || errno == EPERM) {
            setErrorCode(403);
        } else if (errno == EINVAL) {
            setErrorCode(400);
        } else if (errno == EBUSY) {
            setErrorCode(503);
        } else {
            setErrorCode(500);
        }
		//std::cout << "deleteRequest() errorCode: " << _errorCode << "\n";
        return _errorCode;
    }

    return 200;
}
