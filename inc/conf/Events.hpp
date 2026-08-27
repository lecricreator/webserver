#ifndef EVENTS
# define EVENTS

class Events {
    private:
        int         _worker_connections;
        std::string _use;
    public:
        Events();
		Events(const Events& copy);
		Events	&operator=(const Events& copy);
		~Events();
        bool    parse_events(std::ifstream *fd_file);
};

#endif
