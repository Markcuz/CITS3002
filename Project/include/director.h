
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

//store some sort of a table linking datatype to analyst
char* storage[2];

int forwardingToAnalyst(char* message, char* fromName);

int checkType(char* message, char* fromName); //checks for analyst available for data type

int forwardingToCollector(char* message, char* fromName);

int addAnalyst(char* message);

int receiveDirectorMessage(void);

int encrypt_DA(char* message);
int SSL_DA(char* message);
int encrypt_DC(char* message);
int SSL_DC(char* message);

int decryptMessageDirector(char* message);
