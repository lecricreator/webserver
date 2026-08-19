#ifndef LOCATION
# define LOCATION
# include "Set_variable.hpp"
# include <vector>

class Location {
    private:
        //std::string                 _path_location;
        //std::string                 _root;
        //std::vector<std::string>    _index;
        std::string                 _fastcgi_pass;
        std::string                 _fastcgi_index;
        std::vector<std::string>    _fastcgi_param;
        std::string                 _include;
        std::string                 _charset;
        std::string                 _expires;
        bool                        _is_cgi_py;
        bool                        _autoindex;
    public:
        Location();
        bool                        parse_location(std::ifstream *fd_file, const std::string pline, size_t posi);
        Set_variable                set;
        std::string                 _path_location;
        std::string                 _root;
        std::vector<std::string>    _index;


        //GET
        const std::string&              get_root() const {return (_root);};
        const std::vector<std::string>& get_index() const {return (_index);};
        const std::string&              get_path_location() const {return (_path_location);};
        const bool&                     get_autoindex() const {return (_autoindex);};

};

#endif
