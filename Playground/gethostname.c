/*
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    char hostname[50];
    gethostname(hostname, sizeof(hostname));
    printf("hostname: %s \n",hostname);
}