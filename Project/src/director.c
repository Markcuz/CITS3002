#include "director.h"

//expecting message: <type> <data> <identifier> <collectorHostname>
int forwardingToAnalyst(char* recMessage){
    
    /*FILE* table;
    ssize_t read;
    size_t len = 0;
    char* toName;
    
    table = fopen("typeTable", "r+");
    char* line = NULL;
    while((read = getLine(&line,&len,table)) != -1) {
        if(line[0]=recMessage[0]) {
            strtok(line, " \n");
            toName = strtok(NULL, " \n");
            break;
        }
    }
    
    char* sendMessage;
    sendMessage = strstr(recMessage, TO_ANALYST);
    */
    //sendData(DIRECTORPORT, toName, recMessage++);
    return 0;
}

//expecting message: <type> <identifier> <Collector_hostname>
int checkType(char* message){
    int typeAvail = 0;
    //checks the table to see if can send
    FILE* table;
    ssize_t read;
    size_t len = 0;
    
    table = fopen("typeTable", "r+");
    char* line = NULL;
    while((read = getLine(&line,&len,table)) != -1) {
        if(line[0]=message[0]) {
            typeAvail = 1;
            break;
        }
    }
    
    char* fromName = strstr(message, CHECK_TYPE)+sizeof(CHECK_TYPE);
    
    if(typeAvail) {
        char retMessage[10];
        //create some message to tell available
        retMessage = "avail"
        //sendData(DIRECTORPORT, fromName, retMessage);
        return 0;
    }
    else {
        char retMessage[10];
        //create some message to tell unavailable
        retMessage = "notAvail";
        //sendData(DIRECTORPORT, fromName, retMessage);
        return 0;
    }
    
    return 1;
}

//clears off the dataType character then forwards to the first analyst that cna analyse it
//expecting message: <data> <identifier> <Collector_hostname>
int forwardingToCollector(char* recMessage) {

    
    
    /*
    int count;
    char* currentMessage = recMessage;
     currentMessage++;
    
    char* startID = strstr(recMessage, TO_COLLECT);
    
    while(currentMessage!=startID) {
        currentMessage++;
        count++;
    }
    */
    //figure out who to send to via message
    //sendData(DIRECTORPORT, toName, recMessage);
    return 0;
}

//expecting message: <type> <identifier> <Analyst_hostname>
//writes <typeChar> " " <AnalystHostname> to "typeTable"
int addAnalyst(char* message) {
    //add analyst to the table
    FILE *table = fopen("typeTable", "a");
    
    if(table == NULL) {
        table = fopen("typeTable", "w+");
        //add in maybe a number for the director no.
        char initialise[]="Director: \n";
        fwrite(initialise, sizeof(char), sizeof(initialise), table);
    }

    fwrite(message[0],sizeof(char), 1, table);
    
    char* start;
    start = strstr(message, ADD_ANALYST);
    
    fwrite(" ",sizeof(char),1, table);
    
    char hostname[(strlen(message)+1)-strlen(ADD_ANALYST)];
    strcpy(hostname,start+strlen(ADD_ANALYST)+1);
    hostname[-1]='\0';
    
    fwrite(hostname,sizeof(char),sizeof(hostname), table);
    
    fwrite("\n",sizeof(char),1, table);
    
    return 0;
}

int deParseMessage(char* recMessage) {
    if(strstr(recMessage, TO_COLLECT)!=NULL) {
        return 0;
    }
    if(strstr(recMessage, TO_ANALYST)!=NULL) {
        return 2;
    }
    if(strstr(recMessage, CHECK_TYPE)!=NULL) {
        return 1;
    }
    if(strstr(recMessage, ADD_ANALYST)!=NULL) {
        return 3;
    }
    
}

int receiveDirectorMessage() {
    char* recMessage = "cadd_analystmyHostname";
    fprintf(stdout, "incoming message: %s", recMessage);
    char* fromName;
    
    //receiveData(DIRECTORPORT, recMessage);
    int messageType = deParseMessage(recMessage);
    
    fprintf(stdout, "type: %d", messageType);
    
    switch(messageType) {
        case 0:
            forwardingToCollector(recMessage, fromName);
            break;
        case 1:
            checkType(recMessage, fromName);
            break;
        case 2:
            forwardingToAnalyst(recMessage, fromName);
            break;
        case 3:
            addAnalyst(recMessage);
            break;
        default:
            return 1;
    }
    
}

int main() {
    receiveDirectorMessage();
    fprintf(stdout, "done");
    return 0;
}
