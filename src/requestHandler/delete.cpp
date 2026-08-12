#include "webserv.hpp"

unsigned int    httpRequest::deleteRequest()
{
	//std::cout << "deleteRequest() call\n";
	_path.erase(0, 1);

	if (_path.find("www/upload/") != 0)
	{
		//std::cout << _path << " deleteRequest invalid path\n";
		setErrorCode(400);
		return _errorCode;
	}
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
