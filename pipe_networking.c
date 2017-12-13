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
    //make named pipe
    mkfifo("source", 0744);
    int spipe = open("source", O_RDONLY);
    int client_info;
  
    //reads from named pipe (blocks until there is something)
    read(spipe, &client_info, sizeof(int));
    printf("%d\n", client_info);
    char *args[3];
    args[0] = "rm";
    args[1] = "source";
    args[2] = "0";
    execvp(args[0], args);

    //write to client
    write(client_info, &client_info, sizeof(int));
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
  //unnamed pipe; server-WRITES, client READS
  int cpipe[2];
  pipe(cpipe);

  //named pipe; connection to server
  int spipe = open("source", O_WRONLY);
  write(spipe, &cpipe[WRITE], sizeof(int));

  //receiving server info
  int server_info;
  read(cpipe[READ], &server_info, sizeof(int));

  printf("%d\n", server_info);
  
  return 0;
}
