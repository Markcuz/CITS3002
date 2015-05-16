
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

#include "director.h"
#include "bank.h"

//defines for certain types
#define TEMP_DATA 1;
#define MASS_DATA 2;

#define MAXBUFLEN 100

#define DIRECTORPORT "4950"
#define BANKPORT "4951"

#define DIRECTOR_NAME "127.0.0.1"
#define BANK_NAME "192.168.1.2"


//Wallet wallet;

int sending(int payment, int data, char* Software);//sends dataType and eCent to Director

int buy_eCent(void); //buys (gains an eCent from the bank)

int checkDirector(int dataType); //checks Director

int receiving(void); //receive the data

int encrypt_CB(char* message);

int SSL_CB(char* message);

int encrypt_CD(char* message);

int SSL_CD(char* message);

int decrypt_BC(char* message);

int decrypt_DC(char* message);

//splits the message into to corresponding eCent and stores it for later use
int storeeCent(char* message);


