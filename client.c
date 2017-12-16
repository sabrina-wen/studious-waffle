#include "pipe_networking.h"


int main() {

  int to_server;
  int from_server;
  char input[BUFFER_SIZE];

  //does handshake once
  from_server = client_handshake( &to_server );
  printf("================================\n");
  
  while(1) {

    //receives input from user and removes newline
    printf("WHATCHA THINKIN'?\n");
    fgets(input, BUFFER_SIZE, stdin);
    *strchr(input, '\n') = 0;

    //writes to server, blocks until reads server response
    write(to_server, input, strlen(input)+1);
    read(from_server, input, strlen(input)+1);
    printf("client recieved: %s\n\n", input);
  }
}
