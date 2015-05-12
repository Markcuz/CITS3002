#include <stdio.h>

char* myType; // specifies what typoe fo data it analyses only


int registerService(Director director, dataType type);

int receiveData(eCent payment, dataType data, director Director); //receive data and eCent from Director

int depositPayment(eCent payment); //attempts to deposit payment (bank checks integrity) FIRST

int sendData(director Director); //sends the data back to the collector (through the director)


