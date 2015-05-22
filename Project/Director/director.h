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

#include "comms.h"

#define DIRECTORPORT "4950"
#define MAXBUFLEN 100

#define TO_COLLECT "to_collect"
#define TO_ANALYST "to_analyst"
#define CHECK_TYPE "check_type"
#define ADD_ANALYST "add_analyst"

int forwardingToAnalyst(char* message);
int checkType(char* message); 
int forwardingToCollector(char* message);
int addAnalyst(char* message);
int receiveDirectorMessage(void);
int deParseMessage(char* message);
