
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

#define MAXBUFLEN 100

#define DIRECTORPORT "4950"
#define BANKPORT "4951"

#define TO_ANALYST "to_analyst"

char* directorName;
char* bankName;

int sending(int data, char* Software);//sends dataType and eCent to Director

int buy_eCent(void); //buys (gains an eCent from the bank)

int checkDirector(int dataType); //checks Director

int receiving(void); //receive the data

//splits the message into to corresponding eCent and stores it for later use
int storeeCent(char* message);


