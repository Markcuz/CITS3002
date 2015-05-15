
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

#define COLLECTORPORT "4950"
#define MAXBUFLEN 100

//store some sort of a table linking datatype to analyst
char* storage[2];

//int forwardToAnalyst(dataType type, software Software, eCent payment, Collector collector);

int checkType(int type); //checks for analyst available for data type

//int forwardToCollector(dataType type, Software software, Analyst analyst);

//int addAnalyst(dataType type);