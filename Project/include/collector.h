
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

//defines for certain types
#define TEMP_DATA 1;
#define MASS_DATA 2;

#define MAXBUFLEN 100

#define DIRECTORPORT "4950"
#define BANKPORT "4951"

#define DIRECTOR_NAME "127.0.0.1"
#define BANK_NAME "192.168.1.2"

typedef char* software;
typedef int dataType;
typedef char* processedSoftware;

//Wallet wallet;

int sending(int payment, dataType data, software Software);//sends dataType and eCent to Director

int buy_eCent(void); //buys (gains an eCent from the bank)

int checkDirector(dataType data); //checks Director

int receiving(processedSoftware* software); //receive the data






