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
    int Run();

private:
    int port_;
    int serverSocket_;

private:
    int InitServerSocket();
};