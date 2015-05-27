/**
* The analyst
* \author Marcus Pham 20495924
* \author Matthew Fitzpatrick 21297239
*/
#include <stdio.h>
#include "comms.h"
#define BANKPORT "4951"
#define DIRECTORPORT "4950"
#define ADD_ANALYST "add_analyst"
#define TO_ANALYST "to_analyst"
#define TO_COLLECTOR "to_collect"
char* directorName;
char* bankName;
char* myType; // specifies what type fo data it analyses only
int registerService(int type);
int receiveDataToProcess(void); //receive data and eCent from Director
int depositPayment(char* message); //attempts to deposit payment (bank checks integrity) FIRST
int processData(char* message, char* collName);
int serviceType;
