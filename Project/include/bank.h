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

#define ANALYST_NAME "127.0.0.3"//subject to change
#define BANK_NAME "192.168.1.2"

#include "comms.h"

typedef struct {
    int identifier;
    char* owner;
} eCent;

int checkDeposit(); // checking eCent from Analyst

int givePayment(); //sends eCent to Collector

int encrypt_BA(char* sendMessage);
int SSL_BA(char* sendMessage);

int encrypt_BC(char* sendMessage);
int SSL_BC(char* sendMessage);

int decrypt(char* recMessage);

int receiveBankMessage();