#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
using namespace std;





int main(){
    int fd = socket(AF_INET,SOCK_STREAM,0);
    int val = 1;
    struct sockaddr_in Address = {};
    Address.sin_family = AF_INET;
    

}