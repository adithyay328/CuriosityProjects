#include <sys/socket.h>
#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>

const char* SERVER_ADDRESS = "192.168.100.2";
const int SERVER_PORT = 1313;

int main() {
  // Get a UDP socket up
  int clientFd = socket(AF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in clientAddress;
  
  clientAddress.sin_family = AF_INET;
  clientAddress.sin_port = htons(0);
  clientAddress.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

  bind ( clientFd, &clientAddress, (socklen_t)sizeof(clientAddress));

  // UDP has no concept of connections, instead we'll
  // send an empty message to the server to open
  // a connection
  struct sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
  serverAddress.sin_port = htons(SERVER_PORT);

  sendto ( clientFd, &serverAddress, 0, 0, &serverAddress, (socklen_t)sizeof(clientAddress));

  // Now, wait for the string response
  const int BUFF_SIZE = 1000;
  char recvBuff[BUFF_SIZE];

  recv ( clientFd, &recvBuff, BUFF_SIZE, 0 );

  // Print
  printf("%s\n", recvBuff);
};
