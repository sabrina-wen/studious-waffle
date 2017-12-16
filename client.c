#include "pipe_networking.h"


int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  char input[BUFFER_SIZE];
  while(1) {
    //receives input from user
    printf("Whatcha thinkin'?\n");
    fgets(input, BUFFER_SIZE, stdin);
    *strchr(input, '\n') = 0;
    write(to_server, input, sizeof(char*));
  }
}
