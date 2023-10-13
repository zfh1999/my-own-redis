#include <iostream>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/ip.h>

class Server
{
public:
    Server(int port);
    ~Server();
    int run();

private:
    int port_;
    
};