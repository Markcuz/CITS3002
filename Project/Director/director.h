#include <stdio.h>

//store some sort of a table linking datatype to analyst
char* storage[2];


int forwardToAnalyst(dataType type, Software software, eCent payment, Collector collector);

int checkType(dataType type); //checks for analyst available for data type

int forwardToCollector(dataType type, Software software, Analyst analyst);

int addAnalyst(dataType type);