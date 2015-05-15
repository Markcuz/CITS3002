#include <stdio.h>

typedef char* software;
typedef int dataType;
typedef char* processedSoftware;

char* myType; // specifies what typoe fo data it analyses only


int registerService(int type);

int receiveData(int payment, int data); //receive data and eCent from Director

int depositPayment(int payment); //attempts to deposit payment (bank checks integrity) FIRST

int returnData(); //sends the data back to the collector (through the director)


