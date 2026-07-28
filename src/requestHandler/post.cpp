#include "webserv.hpp"

/**
 * @brief Checks that a file path is a directory with write access to it.
*/
bool is_directory(const std::string& path) {
    //assert(path.empty() == false && "String contains an actual path");

    struct stat path_stat;
    std::memset(&path_stat, 0, sizeof(path_stat));
    if (stat(path.c_str(), &path_stat) != 0) 
        return false;

    if (!S_ISDIR(path_stat.st_mode)) 
        return false;

    if (access(path.c_str(), W_OK) != 0) 
        return false;

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

bool    httpRequest::postRequest()
{
    //use stat to check if the path is a file or a directory
    //use access to check if you can write in said directory

    if (_body.empty())
        return true; //maybe set error code to 200
    if (!is_directory(_path))
    {
        setErrorCode(500);
        return false;
    }

    int fd = open(_path.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0600);

}