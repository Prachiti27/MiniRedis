#ifndef SERVER_H
#define SERVER_H

#include "database.h"

class Server{
    public:
        Server(int port);
        void start();
    
        private:
            int port;
            int server_fd;
            Database db;
};

#endif