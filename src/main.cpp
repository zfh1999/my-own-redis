#include <iostream>

#include "server/Server.h"

int main() {
  Server server(9999);
  
  server.Init();
  for(;;){
	server.Run();
  }
  return 0;
}