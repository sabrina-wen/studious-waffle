#include "pipe_networking.h"


int main() {

  int to_client;
  int from_client;

  char buf[BUFFER_SIZE];

  while (1) {
    from_client = server_handshake( &to_client);
    while(read(from_client, buf, sizeof(char*))) {
      printf("Recieved from client: %s\n", buf);
      write(to_client, buf, sizeof(char*));
    }
  }
}
