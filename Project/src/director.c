#include "director.h"

int forwardingToAnalyst(char* message){
    encrypt_DA(message);
    SSL_DA(message);
    
    //figure out where to send to via table
    
    sendData(DIRECTORPORT, toName, message);
    return 0;
}

int checkType(char* message, char* fromName){
    bool typeAvail;
    //checks the table to se if can send
    
    if(typeAvail) {
        char* retMessage;
        //create some message to tell available
        
        encrypt_DC(message);
        SSL_DC(message);
        
        sendData(DIRECTORPORT, fromName, retMessage);
        return 0;
    }
    else {
        char* retMessage;
        //create some message to tell unavailable
        
        encrypt_DC(message);
        SSL_DC(message);
        
        sendData(DIRECTORPORT, fromName, retMessage);
        return 0;
    }
    
    return 1;
}

int forwardingToCollector(char* message) {
    encrypt_DC(message);
    SSL_DC(message);
    
    //figure out who to send to via message
    sendData(DIRECTORPORT, toName, message);
    return 0;
}

int addAnalyst(char* message) {
    //add analyst to the table
    return 0;
}

int receiveDirectorMessage() {
    char* recMessage;
    
    receiveData(DIRECTORPORT, recMessage);
    
    decryptMessage(recMessage);
    
    int messageType;
    //read some layer to determine type to message
    
    switch(messageType) {
        case 0:
            forwardingToCollector(recMessage, fromName);
        case 1:
            checkType(recMessage, fromName);
        case 2:
            forwardingToAnalyst(recMessage, fromName);
        case 3:
            addAnalyst(recMessage);
    }
}
