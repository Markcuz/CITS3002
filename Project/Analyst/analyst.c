#include "analyst.h"

int registerService(int type) {
    char sendMessage[100];
    
    char myName[20];
    gethostname(myName, sizeof(myName));
    
    sprintf(sendMessage, "%d%s%s", serviceType, ADD_ANALYST, myName);
    
    printf("directorName: %s\n", directorName);
    printf("sendMessage: %s\n", sendMessage);
    sendData(DIRECTORPORT, directorName, sendMessage);
    
    return 0;
}

int receiveDataToProcess() {
    char* recMessage = malloc(100 * sizeof(char));
    
    printf("receiving data\n");
    receiveData(DIRECTORPORT, recMessage);
    printf("received data\n");
    
    printf("got message: ");
    //parsing for the data from the director.
    
    char* collName = malloc(100 * sizeof(char));
    char* colMessage = malloc(100 * sizeof(char));
    
	char msEnd[] = "Message_END";
    
	char* dN = strstr(recMessage, msEnd);
	char* dirName = dN+(strlen(msEnd));
    
    free(recMessage);
		
	char coinN[30];
	strcpy(coinN, colMessage);
    //splitting up the message into eCent and message (after stripping layers)
    char* payment = malloc(100 * sizeof(char));//format(ownerBankID, coinID)
    
    //NEED TO split into payment by no. bytes
    
    char* message = malloc(100 * sizeof(char));//DATA
    
    if(depositPayment(payment)==0) {
        processData(message,collName);
        free(collName);
        free(colMessage);
        free (message);
        free(payment);
        return 0;
    }

    else {
        char failMessage[]="failure";
        sendData(DIRECTORPORT, directorName, failMessage);
        return 0;
    }
    
    return 1;
}

int processData(char* message, char* collName) {
    printf("processing\n");
    char sendMessage[256];
    
    sprintf(sendMessage, "%s%d%s%s", message, serviceType, TO_COLLECTOR, collName);
    
    printf("sending: %s\n", sendMessage);
    
    sendData(DIRECTORPORT, directorName, sendMessage);

    //processes the data then modifies message for the processed Message
    return 0;
}

int depositPayment(char* payment) {
    
	char* dpayment;
    char bMessage[strlen(payment)+7];
    
	sprintf(bMessage, "deposit%s",payment);
	sendData(BANKPORT, bankName, dpayment);
    char allGood[] = "Well done you";
    char* response = "";
	receiveData(BANKPORT, response);
    	if(strcmp(response, allGood) == 0){
            return 0;
        }
	else{return 1;}
    return 1;
}

int main(int argc, char* argv[]) {
    
    if(argc!=4) {
        printf("usage: bankName directorName dataType\n");
        return 1;
    }
    
    bankName = argv[1];
    directorName = argv[2];
    
    printf("type: %s\n", argv[3]);
    serviceType = atoi(argv[3]);
    
    printf("registering...\n");
    registerService(serviceType);
    printf("registered\n");
    
    while(1) {
        printf("processing new data\n");
        receiveDataToProcess();
        usleep(10000);
    }
    
    return 0;
}


