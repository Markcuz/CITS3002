#include <stdio.h>
#include "comms.h"

#define BANKPORT "4951"
#define DIRECTORPORT "4950"

#define DIRECTOR_NAME "127.0.0.1"
#define BANK_NAME "192.1.1.1"

char* myType; // specifies what type fo data it analyses only

int registerService(int type);

int receiveDataToProcess(void); //receive data and eCent from Director

int depositPayment(char* message); //attempts to deposit payment (bank checks integrity) FIRST

int processData(char* message);

int encrypt_AD(char* message);
int SSL_AD(char* message);

int encrypt_AD(char* payment);
int SSL_AD(char* payment);

int decrypt_BA(char* message);

int decrypt_DA(char recMessage);