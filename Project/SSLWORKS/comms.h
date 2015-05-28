#define _GNU_SOURCE
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <resolv.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <linux/if_link.h>

#define MAXBUFLEN 1000
#define FAIL    -1

int sendData(char* port, char* hostname, char* message);

int receiveData(char* port, char* receivedMessage);

int myIP(char* myAddr);
