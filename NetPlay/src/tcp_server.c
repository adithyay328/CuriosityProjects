#include <sys/socket.h>
#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>

const char* SERVER_ADDRESS = "192.168.100.2";
const int SERVER_PORT = 1313;

int main() {
  // To start, we indicate to Linux that we want
  // a socket of a certain type. The socket syscall
  // says we want an ipv4 oriented tcp socket;
  // AF_INET = ipv4, SOCK_STREAM = TCP-esque( I say
  // esque because a UNIX_DOMAIN socket with SOCK_STREAM
  // is not TCP based, but behaves like it)
  int serverFd = socket(AF_INET, SOCK_STREAM, 0);

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

  // Now, let's start listening for connections.
  // Set our socket into listening mode, and allow 5 new
  // conncetions(5 is the size of the valid connections,
  // 1024 is the system wide new connection limit).
  //
  // Basically, when a new client SYNs with us, they get
  // added to a queue, and we SYN-ACK them. Until we
  // receive an ACK from them, it stays on the system wide
  // pool. Once the ACK is given, it moves to the connection
  // queue of the socket below, which has limit 5.
  listen( serverFd, 5 );

  // Now, we need to get a socket to allows us to talk
  // to a new client. To do this, we need to accept() on
  // the above listening socket, which will gives us a new
  // FD and the destination address of the other system
  // that connected to us.
 
  // A sockaddr_in containing the ip location of the
  // other machine.
  struct sockaddr_in clientAddress; 
  socklen_t clientLen; // Redundant, but we need to pass it

  // Get our new socket
  int clientFd = accept( serverFd, &clientAddress, &clientLen);
  
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

  // Send is pretty simple. Give the client fd, the string,
  // length of message, and then any flags.
  //
  // NOTE: First send the msg size
  send ( clientFd, &msgLen, sizeof(msgLen), 0);
  send( clientFd, hWorld, strlen ( hWorld ), 0 );

  printf("Shutting down client socket, got response: %d\n", shutdown( clientFd, SHUT_RDWR));
  printf("Shutting down server socket, got response: %d\n", shutdown( serverFd , SHUT_RDWR));
  printf("Closing down client socket, got response: %d\n", close( clientFd ));
  printf("Closing down server socket, got response: %d\n", close( serverFd ));
};
