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

void displayClientInfo(char *ReadBuff) {
  while (*ReadBuff != '\0') {
    cout << *ReadBuff;
    ReadBuff++;
  }
}

void communicateWithServer(int CliConnFD) {
  char ReadBuff[READ_BUFF_SIZE] = {};
  ssize_t n = read(CliConnFD, ReadBuff, sizeof(ReadBuff) - 1);
  if (n < 0) {
    std::string msg = "read error";
    reportFails(msg);
  }

  displayClientInfo(ReadBuff);

  char WriteBuff[] = "world";
  write(CliConnFD, WriteBuff, sizeof(WriteBuff));
}

int main() {
  // TCP connection.
  int ServerFD = socket(AF_INET, SOCK_STREAM, 0);

  int Val = 1;
  setsockopt(ServerFD, SOL_SOCKET, SO_REUSEADDR, &Val, sizeof(Val));

  // Setup socket
  struct sockaddr_in Address = {};
  Address.sin_family = AF_INET;
  Address.sin_port = ntohs(1234);
  Address.sin_addr.s_addr = ntohl(0); // wildcard address 0.0.0.0
  int rv = bind(ServerFD, (const sockaddr *)&Address, sizeof(Address));
  if (rv) {
    std::string Msg = "bind failure";
    reportFails(Msg);
  }

  rv = listen(ServerFD, SOMAXCONN);

  if (rv) {
    std::string Msg = "listen failure";
    reportFails(Msg);
  }

  while (true) {
    // accept
    struct sockaddr_in ClientAddr = {};
    socklen_t CliSkLen = sizeof(ClientAddr);
    int CliConnFD = accept(ServerFD, (struct sockaddr *)&ClientAddr, &CliSkLen);
    if (CliConnFD < 0) {
      continue; // error
    }
    communicateWithServer(CliConnFD);
    close(CliConnFD);
  }

  return 0;
}