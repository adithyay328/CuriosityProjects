#include <sys/socket.h>
#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>

const char* SERVER_ADDRESS = "192.168.100.2";
const int SERVER_PORT = 1313;

/*
 * READ THE SERVER FIRST, IT HAS MORE DOCS
 * ABOUT THE BASICS OF HOW TCP AND SOCKETS WORK.
 */

int main() {
  int clientFd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in clientAddress;

  clientAddress.sin_family = AF_INET;
  // The port number is 0, which means that Linux
  // will find us a port number automatically
  clientAddress.sin_port = htons(0);
  // Use same address as server, they're on the same machine
  clientAddress.sin_addr.s_addr = inet_addr("192.168.100.2");

  // Now that we have our address and port, let's bind;
  // this actually connects this socketo a system port
  printf("%d\n", bind( clientFd , &clientAddress, (socklen_t)sizeof( clientAddress )));
  printf("%d\n", errno);

  // Here's where we differ from the server. Instead of
  // going into LISTEN mode to get sockets representing
  // other connections, we instead want to:
  // 1. Connect to the server socket
  // 2. Receive the message sent back
  // 3. Print it.
  //
  // Later, we'll send traffic and also
  // receive
  
  // First, make the address of the server. Then
  // we can connect to it
  struct sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(SERVER_PORT);
  serverAddress.sin_addr.s_addr = inet_addr("192.168.100.2");

  connect ( clientFd, &serverAddress, (socklen_t)sizeof( clientAddress ));

  // At this point we're connected, and we can pull data via
  // recv. But, an amusing problem I just realized is that we
  // don't know how much data is being sent i.e. how big the
  // string is going to be.
  //
  // The standard solution, as implemented by basically every
  // application layer protocol, is a header that tells us how
  // much data to expect. So, we'll do the same, in a lightweight
  // way.
  //
  // For every string message being sent, we expect a size_t
  // at the start, indicating the length of the string to be given.
  // A fun problem this is going to have is that size_t is implementation
  // specific; for now that won't matter, but for a real implementation
  // you'd probably want to fix it to something like 64 bit, and use
  // software based 64-bit if needed.
  size_t strLength;
  recv( clientFd, &strLength, sizeof(size_t), 0);

  printf("Got message length: %d\n", strLength);

  // Now, construct that long of a char buffer,
  // recv it, and then print the string response
  char msg[strLength];
  recv ( clientFd, &msg, strLength, 0);

  printf("Got string message\n");

  printf("%s", msg);

  printf("Shutting down client socket, got response: %d\n", shutdown( clientFd, SHUT_RDWR));
  printf("Closing down client socket, got response: %d\n", close( clientFd ));
};
