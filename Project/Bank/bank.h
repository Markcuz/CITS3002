#include <stdio.h>
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

#define BANKPORT "4951"

//format sendMessage: <
#include "comms.h"

typedef struct {
    int identifier;
    char owner[20];
} eCent;

int checkDeposit(); // checking eCent from Analyst

int givePayment(); //sends eCent to Collector

int receiveBankMessage();
