#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <string.h>
#include <linux/if_link.h>

#define MAXBUFLEN 1000

int sendData(char* port, char* hostname, char* message);

int receiveData(char* port, char* receivedMessage);

int myIP(char* myAddr);