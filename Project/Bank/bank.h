#include <stdio.h>

typedef struct {
    int identifier;
    char* owner;
} eCent;

int checkDeposit(eCent payment); // checking eCent from Analyst

int givePayment(); //sends eCent to Collector
