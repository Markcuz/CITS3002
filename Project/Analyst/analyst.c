/**
 * The analyst
 * \author Marcus Pham 20495924
 * \author Matthew Fitzpatrick 21297239
 */

#include "analyst.h"

/**
 * registers the service to the director
 * sends message : <type> [add_analyst] <AnalystHostname>
 */
int registerService(int type) {
    char sendMessage[100];
    
    char myName[20];
    myIP(myName);
    
    sprintf(sendMessage, "%d%s%s", serviceType, ADD_ANALYST, myName);

    sendData(DIRECTORPORT, directorName, sendMessage);
    
    return 0;
}

/**
 * receives incoming data from the director to process
 * recMessage: <payment> <data> [to_analyst] <collectorHostname>
 * attempts to deposit the payment to the back
 * on success processData
 * on failure returns "failure" [to_collector] <collectorHostname>
 */
int receiveDataToProcess() {
    char recMessage[200];
    
    printf("Receiving data\n");
    receiveData(DIRECTORPORT, recMessage);
    
    char collName[100];
    
	char* dN = strstr(recMessage, TO_ANALYST);
	char* dirName = dN+(strlen(TO_ANALYST));// actually the collector's IP
	char colMessage[(strlen(recMessage)-strlen(dN) - 28)];
    
	strncpy(colMessage, recMessage+29, sizeof(colMessage)-1);
    sprintf(collName, "%s", dirName);
    
    
    //splitting up the message into eCent and message (after stripping layers)
	char payment[30];//format(ownerBankID, coinID)
    colMessage[(strlen(recMessage)-strlen(dN) - 28)-1] ='\0';
    //have to check this
    strncpy(payment, recMessage,29);
    payment[29] = '\0';
   
    
    char* message = malloc(100 * sizeof(char));//DATA
    if(depositPayment(payment)==0) {
        processData(colMessage,collName);
        free (message);
        return 0;
    }

    else {
      char failMessage[8+strlen(collName)];
      sprintf(failMessage, "failure%s",collName);
        sendData(DIRECTORPORT, directorName, failMessage);
        free (message);
        return 1;
    }
    return 1;
}

/**
 * processes the data places the type at the end of the message
 * message format: <data>
 * hostname: <collName>
 * sends: <data> <serviceType> [to_collector] <collName>
 */
int processData(char* message, char* collName) {
    printf("Processing data\n");
    char sendMessage[256];
    char dMesg[strlen(message)+1];
    sprintf(dMesg, "%s", message);
    
   // dMesg[strlen(message)-1] = '\0';
    sprintf(sendMessage, "%s%d%s%s", dMesg, serviceType, TO_COLLECTOR, collName);
    
    sendData(DIRECTORPORT, directorName, sendMessage);
    return 0;
}

/**
 * deposits the payment into the bank
 * returns 0 on success, 1 on failure
 * send Message: "deposit" <payment> <AnalystHostname>
 * returns 0 on success 1 on failure reply
 */
int depositPayment(char* payment) {
    
    char name[50];
    myIP(name);
    
    char* dpayment;
    char bMessage[strlen(payment)+7+strlen(name)];
    
    //need to strip off the coll hostname
    
    
    sprintf(bMessage, "deposit%s%s",payment, name);
    dpayment = bMessage; // You forgot this
    printf("sending deposit\n");
    sendData(BANKPORT, bankName, dpayment);
    
    //cehcking for the success message
    char allGood[] = "Well done you";
    char response[100];
	receiveData(BANKPORT, response);
    
    if(strcmp(response, allGood) == 0){
        return 0;
    }
	else{
        return 1;
    }
    return 1;
}

/**
 * the main function
 * initialises bankName and directorName along with the analyst dataType
 * registers service to the director 
 * then infinite loop receiving the data from the director
 */
int main(int argc, char* argv[]) {
    
    if(argc!=4) {
        printf("usage: bankName directorName dataType\n");
        return 1;
    }

    bankName = argv[1];
    directorName = argv[2];
    
    printf("Type: %s\n", argv[3]);
    serviceType = atoi(argv[3]);
    
    printf("Registering...\n");
    registerService(serviceType);
    printf("Registered\n");

    while(1) {
        printf("Processing new data\n");
        receiveDataToProcess();
        usleep(10000);
    }
    
    return 0;
}


