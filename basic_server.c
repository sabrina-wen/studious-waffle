#include "pipe_networking.h"

void modify(char * str) {
  char * delim;
  while (delim = strchr(str, 'i')) {
    *delim = 'Y';
  }
  while (delim = strchr(str, 'e')) {
    *delim = 'E';
  }
  while (delim = strchr(str, 'a')) {
    *delim = 'A';
  }
  while (delim = strchr(str, 'o')) {
    *delim = 'O';
  }

}

int main() {

  int to_client;
  int from_client;
  char buf[BUFFER_SIZE];

  while (1) {
    printf("\n");
    from_client = server_handshake( &to_client);
    printf("================================\n");

    while(read(from_client, buf, BUFFER_SIZE)) {
      printf("Recieved from client: %s\n", buf);
      modify(buf);
      write(to_client, buf, strlen(buf));
    }
  }
}
