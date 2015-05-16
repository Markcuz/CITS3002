#include "bank.h"

//receives eCent and checks for integrity, replies to analyst if good, deposit else discard
int checkDeposit(char* recMessage, char* fromName){
    bool valid;
    //do some analysis on the message to see if good
    
    
    char* sendMessage;
    
    if(valid) {
        //create some valid string
        
        //encrypt the message
        
        SSL_BA(sendMessage);
        
        sendData(BANKPORT, fromName, sendMessage);
        
        //deposit the coin (ie remove from database?)
        
        return 0;
    }
    else {
        //create some invalid string
        
        //encrypt the message
        encrypt_BA(sendMessage);
        
        SSL_BA(sendMessage);
        
        sendData(BANKPORT, fromName, sendMessage);
        
        return 0;
    }
    return 1;
}

int givePayment(char* fromName) {
    char* sendMessage;
    
    //some char* for the eCent
    
    encrypt_BC(sendMessage);
    SSL_BC(sendMessage);
    
    sendData(BANKPORT, fromName, sendMessage);
    
}

int receiveBankMessage() {
    char* recMessage;
    receiveData(BANKPORT, recMessage);
    
    decrypt(recMessage);
    
    bool deposit;
    //some checker to see what type of request
    
    //the incoming message addressName
    char* fromName;
    
    if(deposit) {
        checkDeposit(message, fromName);
    }
    
    else {
        givePayment(fromName);
    }
}
