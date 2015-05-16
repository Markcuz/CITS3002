#include "collector.h"

int sending(int payment, int data, char* message) {

    //need to check the director
    if(checkDirector() != 0) {
        //wait for a while? until director has an analyst?
    }
    
    //add something to front of string to identify a data exchange
    
    /* somethign to put together encrypted stirng*/
    //adding encryption from Collector to Director (CD)
    addEncrypt_CD(message);
    
    //adding SSL from Collector to Director
    addSSL_CD(message);
    
    //sending the string
    if(sendData(DIRECTPORT, DIRECTOR_NAME, message)==0) {
        //returning success
        return 0;
    }
    else {
        //returning failure
        return 1;
    }
    return 0;
}

int buy_eCent(void) {
    char* message = "request";
    
    //adding Encryption Collector to Bank
    encrypt_CB(message);
    
    //add SSL Colelctor to bank
    SSL_CB(message);
    
    //need to parse a string here
    if(sendData(BANKPORT, BANK_NAME, message) == 0) {
        //returning success
        return 0;
    }
    else {
        //returning failure
        return 1;
    }
        return 0;
}

int checkDirector(int type) {
    //something to send to Director to identify the request
    char* message = "check";
    
    //adding encryption from Collector to Director (CD)
    addEncrypt_CD(message);
    
    //adding SSL from Collector to Director
    addSSL_CD(message);
    
    if(sendData(DIRECTPORT, DIRECTOR_NAME, message)==0) {
        //returning success
        return 0;
    }
    else {
        //returning failure
        return 1;
    }
    
    return 0;
}

//receiving the processed Data from the director
int receivingData() {

    char* message;
    
    //receiving on the director to collector port
    receiveData(DIRECTPORT, message);
    
    decrypt_DC(message);
    
    ///printing for when done?
    printf(message);
    
    return 0;
}

int receiveeCent(){
    
    char* message;
    
    receiveData(BANKPORT, message);
    
    decrypt_BC(message);
    
    storeeCent(message);
    
    return 0;
}

