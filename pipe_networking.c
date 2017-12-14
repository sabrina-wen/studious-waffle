#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Perofrms the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  while (1) {
    //make named pipe, spipe of server reads from client
    mkfifo("source", 0777);
    int spipe = open("source", O_RDONLY);
    if (spipe == -1) {
      printf("Error creating spipe: %s\n", strerror(errno));
    }
    char * client_info; // name of client's pipe

    //reads from named pipe (client) (waits for client to do stuff)
    if (read(spipe, &client_info, sizeof(char*)) == -1) {
      printf("Read error: %s\n", strerror(errno));
    }
    // server prints recieved info from client (name of client's pipe)
    printf("server recieved: %s\n", client_info);
    //char * args[] = {"rm", "source", NULL};
    //execvp(args[0], args);

    //opens up name of client's pipe, writes back to client
    /*int cpipe = open(client_info, O_WRONLY);
    if (write(cpipe, client_info, sizeof(char*)) == -1) {
      printf("%s\n", strerror(errno));
    } */
  }
  return 0;
}


/*=========================
  client_handshake
  args: int * to_server

  Perofrms the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  //named pipe; server-WRITES, client READS
  char * client_name = "dknfknsdmagrheu9nvurvn";
  //mkfifo(client_name, 0744);
  //int cpipe = open(client_name, O_RDONLY);
  //if (cpipe == -1) {
  //  printf("%s\n", strerror(errno));
//  }

  //opens pipe from server, writes to server pipe
  int spipe = open("source", O_WRONLY);
  if (write(spipe, &client_name, sizeof(char*)) == -1) {
    printf("%s\n", strerror(errno));
  }

  // client waits for server to send back info, prints out what info it recieves
  /*char * server_info;
  read(cpipe, server_info, sizeof(char*));

  printf("client received: %s\n", server_info); */

  return 0;
}
