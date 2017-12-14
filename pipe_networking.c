#include "pipe_networking.h"

int checkerror(int stat) {
  if (stat == -1) {
    printf("ERROR: %s\n", strerror(errno));
  }
  return stat;
}

/*=========================
  server_handshake
  args: int * to_client

  Perofrms the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
    //make named pipe, spipe of server reads from client
    mkfifo("source", 0777);
    int spipe = checkerror(open("source", O_RDONLY));
    char * client_info; // name of client's pipe

    //reads from named pipe (client) (waits for client to do stuff)
    checkerror(read(spipe, &client_info, sizeof(char*)));
    printf("server recieved: %s\n", client_info);

    //removes name from connection
    int f= fork();
    if (!f) {
      char * args[] = {"rm", "source", NULL};
      execvp(args[0], args);
    }

    //opens up name of client's pipe, writes back to client
    char * dumb = "successful";
    *to_client = open(client_info, O_WRONLY);
    write(*to_client, &dumb, sizeof(char *));
    return spipe;
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
  mkfifo(client_name, 0744);

  //opens pipe from server, writes to server pipe
  *to_server = open("source", O_WRONLY);
  checkerror(write(*to_server, &client_name, sizeof(char*)));

  //client reads what server has returned
  char * server_info;
  int cpipe = checkerror(open(client_name, O_RDONLY));
  checkerror(read(cpipe, &server_info, sizeof(char *)));
  printf("%s\n", server_info);

  //if the success message wasn't received, messed up
  if (strcmp(server_info, "successful")) {
    return -1;
  }

  //makes it an unnamed connection
  int f = fork();
  if (!f) {
    char * args[] = {"rm", client_name, NULL};
    execvp(args[0], args);
  }

  return cpipe;
}
