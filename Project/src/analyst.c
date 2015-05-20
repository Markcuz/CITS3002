#include "analyst.h"
#include "comms.h"
int serviceType = 3; //I dunno, set this up sometime.
int registerService(int type) {
    char* sendMessage;
    
    //create a message to send to register the service
    
    encrypt_AD(sendMessage);
    SSL_AD(sendMessage);
    
    sendData(DIRECTORPORT, DIRECTOR_NAME, sendMessage);
    
    return 0;
}

int receiveDataToProcess() {
   /* char* recMessage;
    
    receiveData(DIRECTORPORT, recMessage);
    //parsing for the data from the director.
    decrypt_DA(recMessage);
	    	char* collName;
    	char* colMessage;
	char msEnd[] = "Message_END";
	char* dN = strstr(recMessage, msEnd);
	char* dirName = dN+(strlen(msEnd));
		

	//now dealing with the message from the collector.
	decrypt_CA(colMessage);
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
        //create a fail messgae to return
        encrypt_AC(failMessage);
	char* failHopper;
        SSL_AD(failHopper);
        sendData(DIRECTORPORT, DIRECTOR_NAME, failMessage);
        return 0;
    }*/
    return 1;
}

int processData(char* message) {
 	char* retMes = "Totally analysed";
	char* retEnc;
	SSL_write(retEnc, retMes, strlen(retMes));
	char dirBack[strlen(message)+strlen(fromname)];
	sprintf(dirBack, "%s%s",retEnc, fromname);
        SSL_write(message, dirBack, strlen(dirBack));
        sendData(DIRECTORPORT, DIRECTOR_NAME, message);

    //processes the data then modifies message for the processed Message
    return 0;
}

int depositPayment(char* payment) {
	char* dpayment;
    char bMessage[strlen(payment)+7];
	sprintf(bMessage, "deposit%s",payment);
    	SSL_write(dpayment, bMessage, strlen(bMessage));
	sendData(BANKPORT, BANK_NAME, dpayment);
    char* allGood = "Well done you";
    char* response;
    	char* dCode;
	receiveData(BANKPORT, response);
	SSL_read(response, dCode, strlen(response));
    	if(strcmp(dCode, allGood) == 0){return 0}
	else{return 1;}    
}



