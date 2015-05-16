#include "analyst.h"

int registerService(int type) {
    char* sendMessage;
    
    //create a message to send to register the service
    
    encrypt_AD(sendMessage);
    SSL_AD(sendMessage);
    
    sendData(DIRECTORPORT, DIRECTOR_NAME, sendMessage);
    
    return 0;
}

int receiveDataToProcess() {
    char* recMessage;
    
    receiveData(DIRECTORPORT, recMessage);
    
    decrypt_DA(recMessage);
    
    //splitting up the message into eCent and message (after stripping layers)
    char* payment;
    char* message;
    
    if(depositPayment(payment)==0) {
        processData(message);
        encrypt_AD(message);
        SSL_AD(message);
        sendData(DIRECTORPORT, DIRECTOR_NAME, message);
        return 0;
    }
    
    else {
        char* failMessage;
        //create a fail messgae to return
        encrypt_AD(failMessage);
        SSL_AD(failMessage);
        sendData(DIRECTORPORT, DIRECTOR_NAME, failMessage);
        return 0;
    }
    return 1;
}

int processData(char* message) {
    //processes the data then modifies message for the processed Message
    return 0;
}

int depositPayment(char* payment) {
    
    encrypt_AB(payment);
    SSL_AB(payment);
    sendData(BANKPORT, BANK_NAME, payment);
    
    char* response;
    receiveData(BANKPORT, response);
    
    decrypt_BA(response);
    
    bool valid;
    //read response and see if true or fale
    
    if(valid) {
        return 0;
    }
    else {
        return 1;
    }
    
}



