#include <stdio.h>
#include "comms.h"

#define BANKPORT "4951"
#define DIRECTORPORT "4950"

#define ADD_ANALYST "add_analyst"

char* myType; // specifies what type fo data it analyses only

int registerService(int type);

int receiveDataToProcess(void); //receive data and eCent from Director

int depositPayment(char* message); //attempts to deposit payment (bank checks integrity) FIRST

int processData(char* message);

int serviceType;
