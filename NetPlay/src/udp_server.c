#include <sys/socket.h>
#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>

char* SERVER_ADDRESS = "192.168.100.2";
int SERVER_PORT = 1313;

int main() {
  // Let's quickly get a UDP socket up
  int serverFd = socket(AF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in serverAddress;

  // AF_INET isn't set by default, since C doesn't
  // have constructors on structs
  serverAddress.sin_family = AF_INET;

  // Port number and the server addr need to be set;
  // note the byte ordering fix for port
  serverAddress.sin_port = htons(SERVER_PORT);
  serverAddress.sin_addr.s_addr = inet_addr("192.168.100.2");

  // Now that we have our address and port, let's bind;
  // this actually connects this socketo a system port
  printf("%d\n", bind( serverFd, &serverAddress, (socklen_t)sizeof( serverAddress )));
  printf("%d\n", errno);

  // UDP doesn't actually have a listen
  // operation, but instead allows us to use
  // recv_from, which gives a message + the address
  // it came from. 
  struct sockaddr_in clientAddress; 
  socklen_t clientLen; // Redundant, but we need to pass it
  
  recvfrom ( serverFd, &clientAddress, 0, 0, &clientAddress, &clientLen);

  // At this point, respond back with our message
  // Print their ip addr and port
  printf("Incoming connection ip address: %s\n", inet_ntoa( clientAddress.sin_addr ) );
  printf("Incoming connection port: %d\n", ntohs( clientAddress.sin_port ) );

  // Reply with hello world
  char* hWorld = "Hello World! Damn this is a lot easier than I thought\n";

  // Remember, we want to send the size of it first,
  // then send the string, so the client knows how
  // much to expect
  size_t msgLen = strlen(hWorld);

  printf("Going to send a string of size: %d\n", msgLen);

  // Send is different for UDP, use sendto instead
  // of send
  //
  // NOTE: First send the msg size
  // sendto ( serverFd, &msgLen, sizeof(msgLen), 0, &clientAddress, &clientLen);
  printf(" %d\n", sendto ( serverFd, &hWorld, msgLen, 0, &clientAddress, (socklen_t)sizeof( serverAddress )));

  printf("Shutting down server socket, got response: %d\n", shutdown( serverFd , SHUT_RDWR));
  printf("Closing down server socket, got response: %d\n", close( serverFd ));
};
