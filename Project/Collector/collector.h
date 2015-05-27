/**
 * The bank
 * \author Matthew Fitzpatrick 21297239
 * \author Marcus Pham 20495924 
 */

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
#define BANKPORT "4951"

#define TO_ANALYST "to_analyst"
#define CHECK_TYPE "check_type"

char* directorName;
char* bankName;

int sending(char* data, char* Software);//sends dataType and eCent to Director

int buy_eCent(void); //buys (gains an eCent from the bank)

int checkDirector(char dataType); //checks Director

int receiving(void); //receive the data

//splits the message into to corresponding eCent and stores it for later use
int storeeCent(char* message);

int receivingData();

