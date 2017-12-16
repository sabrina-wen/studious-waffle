#include "pipe_networking.h"


int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  printf("================================\n");
  char input[BUFFER_SIZE];
  while(1) {
    //receives input from user
    printf("WHATCHA THINKIN'?\n");
    fgets(input, BUFFER_SIZE, stdin);
    *strchr(input, '\n') = 0;
    write(to_server, input, strlen(input)+1);
    read(from_server, input, strlen(input)+1);
    printf("client recieved: %s\n\n", input);
  }
}
