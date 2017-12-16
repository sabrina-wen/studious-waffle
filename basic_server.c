#include "pipe_networking.h"


//modifies client text by replacing certain vowels with capitalized letters
void modify(char * str) {
  char * delim;
  while (delim = strchr(str, 'i')) { *delim = 'Y'; }
  while (delim = strchr(str, 'e')) { *delim = 'E'; }
  while (delim = strchr(str, 'a')) { *delim = 'A'; }
  while (delim = strchr(str, 'o')) { *delim = 'O'; }
}

int main() {

  int to_client;
  int from_client;
  char buf[BUFFER_SIZE];

  //outer loop to keep server running
  while (1) {
    
    //perform handshake everytime you reconnect to new client
    printf("\n");
    from_client = server_handshake( &to_client);
    printf("================================\n");

    //while loop to continuously read and modify client info before writing
    while(read(from_client, buf, BUFFER_SIZE)) {
      printf("Recieved from client: %s\n", buf);
      modify(buf);
      write(to_client, buf, strlen(buf));
    }
  }
}
