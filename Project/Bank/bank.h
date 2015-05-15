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

#define ANALYSTPORT "4950"
#define BANKPORT "4951"

#define ANALYST_NAME "127.0.0.3"//subject to change
#define BANK_NAME "192.168.1.2"
typedef struct {
    int identifier;
    char* owner;
} eCent;

int checkDeposit(eCent payment); // checking eCent from Analyst

int givePayment(); //sends eCent to Collector
int givePayment(); //sends eCent to Collector
