#include "analyst.h"

int registerService(int type) {
    char sendMessage[100];
    
    char myName[20];
    
    gethostname(myName, sizeof(myName));
    
    sprintf(sendMessage, "%d%s%s", serviceType, ADD_ANALYST, myName);
    
    sendData(DIRECTORPORT, directorName, sendMessage);
    
    return 0;
}

int receiveDataToProcess() {
    char* recMessage;
    
    receiveData(DIRECTORPORT, recMessage);
    //parsing for the data from the director.
	    	char* collName;
    	char* colMessage;
	char msEnd[] = "Message_END";
	char* dN = strstr(recMessage, msEnd);
	char* dirName = dN+(strlen(msEnd));
		
	char coinN[30];
	strcpy(coinN, colMessage);
    //splitting up the message into eCent and message (after stripping layers)
    char* payment;//format(ownerBankID, coinID)
    char* message;//DATA
    
    if(depositPayment(payment)==0) {
        processData(message);
        return 0;
    }
    
    else {
        char* failMessage;
        sendData(DIRECTORPORT, directorName, failMessage);
        return 0;
    }
    return 1;
}

int processData(char* message) {
 	char* retMes = "Totally analysed";
	char* retEnc;
	
    //find fromName
    //char dirBack[strlen(message)+strlen(fromname)];
	//sprintf(dirBack, "%s%s",retEnc, fromname);
        sendData(DIRECTORPORT, directorName, message);

    //processes the data then modifies message for the processed Message
    return 0;
}

int depositPayment(char* payment) {
	char* dpayment;
    char bMessage[strlen(payment)+7];
	sprintf(bMessage, "deposit%s",payment);
	sendData(BANKPORT, bankName, dpayment);
    char* allGood = "Well done you";
    char* response;
    	char* dCode;
	receiveData(BANKPORT, response);
    	if(strcmp(dCode, allGood) == 0){
            return 0;
        }
	else{return 1;}    
}

int main(int argc, char* argv[]) {
    
    if(argc!=3) {
        printf("usage: bankName directorName");
    }
    
    argv[1] = bankName;
    argv[2]= directorName;
    
    
    return 0;
}


