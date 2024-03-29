#include "Server.h"

constexpr auto MAX_EVENTS = 1000;
constexpr auto BUF_SIZE = 1024;

Server::Server(int port) {
  port_ = port;
  //events_ = new epoll_event[1000];
  events_.resize(1000);
}

Server::~Server() {
  auto ret = close(server_fd_);
  if (ret) {
    msg("cannot close server_fd");
  }
  ret = close(epoll_fd_);
  if (ret) {
    msg("cannot close epoll_fd");
  }
  //delete[] events_;
}

int Server::Init() {
  server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd_ < 0) {
    printerr(server_fd_);
    return server_fd_;
  }

  int opt = 1;
  setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port_);
  addr.sin_addr.s_addr = INADDR_ANY;
  int ret = bind(server_fd_, (sockaddr *)&addr, sizeof(addr));
  if (ret) {
    return ret;
  }

  ret = listen(server_fd_, SOMAXCONN);
  if (ret) {
    return ret;
  }

  fcntl(server_fd_, F_SETFL, fcntl(server_fd_, F_GETFL, 0) | O_NONBLOCK);

  epoll_fd_ = epoll_create1(0);
  if (epoll_fd_ < 0) {
    return epoll_fd_;
  }

  epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = server_fd_;

  ret = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, server_fd_, &event);
  if (ret == -1) {
    return ret;
  }
  return 0;
}

int Server::Run() {
  epoll_event event;
  while (1) {
    int events_num = epoll_wait(epoll_fd_, events_.data(), MAX_EVENTS, -1);
    if (events_num == -1) {
      printerr(errno);
      return -1;
    }

    for (int i = 0; i < events_num; ++i) {
      if (events_[i].data.fd == server_fd_) {
        sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd =
            accept(server_fd_, (sockaddr *)&client_addr, &client_addr_len);
        if (client_fd < 0) {
          continue;
        }

        fcntl(client_fd, F_SETFL, fcntl(client_fd, F_GETFL, 0) | O_NONBLOCK);

        event.events = EPOLLIN;
        event.data.fd = client_fd;
        auto ret = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, client_fd, &event);
        if (ret == -1) {
          continue;
        }
      } else {
        char buf[BUF_SIZE];
        int ret = recv(events_[i].data.fd, buf, BUF_SIZE, 0);
        if (ret == -1) {
          // 接收失败
          if (errno == EAGAIN) {
            continue;
          }
        } else if (ret == 0) {
          std::cout << "clientFd=" << events_[i].data.fd
                    << " is disconnect! \n";
          epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, events_[i].data.fd, NULL);
		  shutdown(events_[i].data.fd, NULL);
		  close(events_[i].data.fd);
          continue;
        }
        std::cout << "clientFd=" << events_[i].data.fd
                  << ", msg=" << std::string(buf, ret) << '\n';
        ret = send(events_[i].data.fd, buf, ret, 0);
      }
    }
  }
}