#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
printf("avant fork");
fork();
printf("après fork \n");
return 0;
}
