#include "Server.h"

#define MAX_EVENTS 1000

int Server::run()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0)
    {
        return server_fd;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    if(ret)
    {
        return ret;
    }

    ret = listen(server_fd, SOMAXCONN);
    if(ret)
    {
        return ret;
    }

    fcntl(server_fd, F_SETFL, fcntl(server_fd, F_GETFL, 0) | O_NONBLOCK);

    int epoll_fd = epoll_create1(0);
    if(epoll_fd < 0)
    {
        return epoll_fd;
    }

    epoll_event event;
    epoll_event *events;
    event.events = EPOLLIN;
    event.data.fd = server_fd;

    ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event);
    if(ret == -1)
    {
        return ret;
    }

    while (1)
    {
        int events_num = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if(events_num == -1)
        {
            return -1;
        }

        for (int i = 0; i < events_num; ++i)
        {
            if(events[i].data.fd == server_fd)
            {
                sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_addr_len);
                if(client_fd < 0)
                {
                    continue;
                }

                fcntl(client_fd, F_SETFL, fcntl(client_fd, F_GETFL, 0) | O_NONBLOCK);

                event.events = EPOLLIN;
                event.data.fd = client_fd;
                ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
                if(ret == -1)
                {
                    continue;
                }
            }
            else
            {
                
            }
        }
    }
    
}
