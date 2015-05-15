#include "collector.h"

int sending(int payment, dataType data, software Software) {

    //need to check the director
    checkDirector();
    
    //need to parse the string of bytes to send
    char* message;
    
    //sending the string
    if(sendData(DIRECTPORT, DIRECTOR_NAME, message)==0) {
        //returning success
        return 0;
    }
    else {
        //returning failure
        return 1;
    }
}

int buy_eCent(void) {
    
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

int checkDirector(dataType type) {
    
    return 0;
}

int receiving(processedSoftware* software) {
        return 0;
}
