#include <unistd.h>
#include <string.h>
void a(char *s)
{
    write(1, s, strlen(s));
}

int main(){
    printf("hello world\n");
}
