#include "conf/Location.hpp"
#include "conf/Set_variable.hpp"
#include "conf/Conf.hpp"


Location::Location() {
	_fastcgi_pass = "";
	_fastcgi_index = "";
	_include = "";
	_charset = "";
	_expires = "";

    this->_is_cgi_py = false;
    this->_autoindex = false;
    _limit_except.push_back("UNINITIALIZED");

	_path_location = "";
	_root = "";
}

Location::Location(const Location& copy)
{
	_fastcgi_pass = copy._fastcgi_pass;
	_fastcgi_index = copy._fastcgi_index;
	_fastcgi_param = copy._fastcgi_param;
	_include = copy._include;
	_charset = copy._charset;
	_expires = copy._expires;
	_is_cgi_py = copy._is_cgi_py;
	_autoindex = copy._autoindex;
	_limit_except = copy._limit_except;

	_path_location = copy._path_location;
	_root = copy._root;
	_index = copy._index;
}

Location	&Location::operator=(const Location& copy)
{
	_fastcgi_pass = copy._fastcgi_pass;
	_fastcgi_index = copy._fastcgi_index;
	_fastcgi_param = copy._fastcgi_param;
	_include = copy._include;
	_charset = copy._charset;
	_expires = copy._expires;
	_is_cgi_py = copy._is_cgi_py;
	_autoindex = copy._autoindex;
	_limit_except = copy._limit_except;

	_path_location = copy._path_location;
	_root = copy._root;
	_index = copy._index;
	return *this;	
}

Location::~Location() {}

bool    Location::parse_location(std::ifstream *fd_file, const std::string pline, size_t posi) {
    std::string line;

    //print(pline);
    this->set.add_in_var(pline, posi, &this->_path_location);
    //print(this->_path_location);
    while (std::getline(*fd_file, line)) {
        if ((posi = line.find("#")) != std::string::npos) {
            continue ;
        } else if ((posi = line.find("root ")) != std::string::npos) {
            this->set.add_in_var(line, posi + 5, &this->_root);
        } else if ((posi = line.find("autoindex ")) != std::string::npos) {
            this->set.add_in_var(line, posi + 10, &this->_autoindex);
        } else if ((posi = line.find("index ")) != std::string::npos) {
            this->set.add_in_var(line, posi + 6, &this->_index);
        } else if ((posi = line.find("is_cgi_py ")) != std::string::npos) {
            this->set.add_in_var(line, posi + 10, &this->_is_cgi_py);
        } else if ((posi = line.find("limit_except ")) != std::string::npos) {
            _limit_except.clear();
            this->set.add_in_var(line, posi + 13, &this->_limit_except);
        } else if ((posi = line.find("}")) != std::string::npos) {
            return (true);
        }
    }
    print_error_conf(NO_END_BRACKET_LOCATION);
    return (false);
}
