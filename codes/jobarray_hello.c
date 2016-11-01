#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
  int id;
  char hostname[80];

  if ( argc == 2 ) {
    id = atoi(argv[1]);
    gethostname(hostname, 80);
    printf("Hello master, I am slave %d running on %s!\n", id, hostname);
    exit(0);
  }
  else {
    printf("Usage: %s id\n", argv[0]);
    printf("where id is an integer\n");
    exit(1);
  }
}
