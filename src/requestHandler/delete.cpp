#include "webserv.hpp"

bool    httpRequest::deleteRequest()
{
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
        return false;
    }
    return true;
}
