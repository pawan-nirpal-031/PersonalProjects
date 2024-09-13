#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <netinet/ip.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

const short READ_BUFF_SIZE = 256;

void reportFails(std::string &FailMsg) {
  int err = errno;
  std::cerr << "Failure : " << FailMsg << " with error code " << err << "\n";
  abort();
}

void displayServerInfo(char *Buff) {
  while (*Buff != '\0') {
    cout << *Buff;
    Buff++;
  }
}

int main() {
  int ClientFD = socket(AF_INET, SOCK_STREAM, 0);
  if (ClientFD < 0) {
    string msg = "client socket open failed!";
    reportFails(msg);
  }

  struct sockaddr_in Address = {};
  Address.sin_family = AF_INET;
  Address.sin_port = ntohs(1234);
  Address.sin_addr.s_addr = ntohl(0); // wildcard address 0.0.0.0
  int rv = connect(ClientFD, (const sockaddr *)&Address, sizeof(Address));
  if (rv) {
    std::string Msg = "connection to server failure";
    reportFails(Msg);
  }

  char WriteMsg[] = "hello";
  write(ClientFD, WriteMsg, sizeof(WriteMsg));

  char ReadMsg[READ_BUFF_SIZE] = {};
  ssize_t n = read(ClientFD, ReadMsg, sizeof(ReadMsg) - 1);
  if (n < 0) {
    string msg = "read fail from server.";
    reportFails(msg);
  }
  displayServerInfo(ReadMsg);
  close(ClientFD);
  return 0;
}