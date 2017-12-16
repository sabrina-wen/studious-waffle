#include "pipe_networking.h"

int checkerror(int stat) {
  if (stat == -1) {
    printf("ERROR: %s\n", strerror(errno));
    exit(1);
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
    mkfifo("source", 0600);
    printf("creating server to client pipe\n");
    int spipe = checkerror(open("source", O_RDONLY));
    int client_info; // name of client's pipe
    //reads from named pipe (client) (waits for client to do stuff)
    checkerror(read(spipe, &client_info, sizeof(int)));
    printf("server recieved: %d\n", client_info);

    //opens up name of client's pipe, writes back to client
    char str_pid[HANDSHAKE_BUFFER_SIZE];
    sprintf(str_pid, "%d", client_info);

    *to_client = open(str_pid, O_WRONLY);
    write(*to_client, &client_info, sizeof(char *));
    printf("writing in server to client pipe\n");

    //reads ACK from client to server pipe
    char client_str[HANDSHAKE_BUFFER_SIZE];
    checkerror(read(spipe, client_str, HANDSHAKE_BUFFER_SIZE));
    printf("server recieved: %s\n", client_str);

    //removes name from connection
    remove("source");
    printf("removed server to pipe name\n");

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
  char str_pid[HANDSHAKE_BUFFER_SIZE];
  int client_name = getpid();
  sprintf(str_pid, "%d", client_name);
  mkfifo(str_pid, 0600);
  printf("making client to server pipe %s\n", str_pid);

  //opens pipe from server, writes to server pipe
  *to_server = open("source", O_WRONLY);
  checkerror(write(*to_server, &client_name, sizeof(int)));
  printf("client is writing: %s\n", str_pid);

  //client reads what server has returned
  int server_info;
  int cpipe = checkerror(open(str_pid, O_RDONLY));
  checkerror(read(cpipe, &server_info, sizeof(int)));
  printf("client recieved: %d\n", server_info);

  //if the success message wasn't received, messed up
  if (server_info != client_name) {
    return -1;
  }

  // client is writing ack to server
  checkerror(write(*to_server, ACK, strlen(ACK)+1));
  printf("client is writing: %s\n", ACK);

  //makes it an unnamed connection
  remove(str_pid);
  printf("removed client to server pipe name\n");

  return cpipe;
}
