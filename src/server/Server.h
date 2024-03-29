#include <fcntl.h>
#include <netinet/ip.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <vector>

static void msg(const char* msg) { fprintf(stderr, "%s\n", msg); }

static void die(const char* msg) {
  int err = errno;
  fprintf(stderr, "[%d] %s\n", err, msg);
  abort();
}

template <typename T>
static void println(const T& msg) {
  static_assert(std::is_integral<T>::value);
  std::cout << std::to_string(msg) << std::endl;
  std::cout.flush();
}

template <typename T>
static void printerr(const T& err) {
  static_assert(std::is_integral<T>::value);
  std::cerr << std::to_string(err) << "errno=" << errno << std::endl;
  std::cerr.flush();
}

class Server {
 public:
  Server(int port);
  ~Server();
  int Init();
  int Run();

 private:
  int port_;
  int serverSocket_;
  int server_fd_;
  int epoll_fd_;
  //epoll_event* events_;
  std::vector<epoll_event> events_;
};