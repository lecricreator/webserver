#include "webserv.hpp"
#define WRITE_MAX 1024
/*
logique postRequest:
logique separee en fonction de si le fichier doit etre write en chunk ou non, potentiellement 2 fonctions differentes
*/

/**
 * @brief Checks that a file path is a directory with write access to it.
*/
bool is_directory(const std::string& path) {
    //assert(path.empty() == false && "String contains an actual path");
    struct stat path_stat;
    std::memset(&path_stat, 0, sizeof(path_stat));
    if (stat(path.c_str(), &path_stat) != 0)
	{
        return false;
	}

    if (!S_ISDIR(path_stat.st_mode))
	{
        return false;
	}

    if (access(path.c_str(), W_OK) != 0)
	{
        return false;
	}

    return true;
}

/**
 * @brief Checks that a file path is an actual path and not a folder or some other file type.
 * Also checks for read file access.
 * */
bool is_regular_file(const std::string& path) {
    //assert(path.empty() == false && "String contains an actual path");

    struct stat path_stat;
    std::memset(&path_stat, 0, sizeof(path_stat));
    if (stat(path.c_str(), &path_stat) != 0) return false;
    if (!S_ISREG(path_stat.st_mode)) return false;

    if (access(path.c_str(), R_OK) != 0) return false;

    return true;
}

/**
 * POST: checks if the target directory exists and if its possible to write to it, ignore request if body is empty
 */

//int    httpRequest::postRequest()
//{
//    //use stat to check if the path is a file or a directory
//    //use access to check if you can write in said directory
//	std::cout << "postRequest() call\n";
//    _path.erase(0, 1);
//
//    if (_fileFd == 0)
//    {
//        _fileFd = open(_path.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0600);
//        if (_fileFd < 0)
//        {
//            setErrorCode(500);
//            return ERROR;
//        }
//    }
//    if (_body.size() <= WRITE_MAX)
//    {
//        std::cout << "debug1\n";
//        write(_fileFd, _body.c_str(), _body.size());
//        _status = REQ_EXECUTED;
//        return SUCCESS;
//    }
//    else
//    {
//        std::cout << "debug2\n";
//        if (write(_fileFd, _body.c_str(), WRITE_MAX) != -1)
//            _bytesWritten += WRITE_MAX;
//        if (_bytesWritten >= _body.size())
//        {
//            _status = REQ_EXECUTED;
//            return SUCCESS;
//        }
//        return UNFINISHED;
//    }
//    //std::cout << "path: " << _path << "\n";
//	//std::cout << "fd: " << fd << "\n";
//	//std::cout << "errno: " << errno << "\n";
//    return SUCCESS;
//}
