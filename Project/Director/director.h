/**
 * The director
 * \author Marcus Pham 20495924
 * \debugging Matthew Fitzpatrick 21297239
 */
#include "director.h"

/**
 * pushes the data stream from the collector to the appropriate analyst
 * searches the text file for the type
 * Rec message: <type> <data> [to_analyst] <collectorHostname>
 * Sent message: <data> [to_analyst] <collectiorHostname>
 */
int forwardingToAnalyst(char* recMessage){
    FILE* table;
    ssize_t read;
    size_t len = 0;
    char* toName;
    
    //finds the type and corresponding analyst hostname
    table = fopen("typeTable", "r+");
    char* line = NULL;
    while((read = getline(&line,&len,table)) != -1) {
        if(line[0]==recMessage[0]) {
            printf("found Analyst!\n");
            strtok(line, " \n");
            toName = strtok(NULL, " \n");
            break;
        }
    }
    
    fprintf(stdout, "Analyst hostname: %s\n", toName);
    
    //stripping type from message
    char* sendMessage = recMessage+1;
    sendData(DIRECTORPORT, toName, sendMessage);
    return 0;
}

/**
 * checks to see if the incoming type has a registered analyst
 * expecting message: <type> [to_analyst] <Collector_hostname>
 * send message: "success" or "failure"
 */
int checkType(char* message){
    int typeAvail = 0;
    FILE* table;
    ssize_t read;
    size_t len = 0;
    
    table = fopen("typeTable", "r+");
    char* line = NULL;
    while((read = getline(&line,&len,table)) != -1) {
        if(line[0]==message[0]) {
            typeAvail = 1;
            break;
        }
    }
    fclose(table);
    
    //finds the collector name
    char* fromName = strstr(message, CHECK_TYPE)+sizeof(CHECK_TYPE)-1;
    fprintf(stdout, "Collector hostname: %s\n", fromName);
    
    if(typeAvail) {
        char retMessage[15] = "success";
        fprintf(stdout, "successful?: %s\n", retMessage);
        usleep(100000);
        sendData(DIRECTORPORT, fromName, retMessage);
        return 0;
    }
    else {
        char retMessage[10] = "failure";
        retMessage[7]='\0';
        fprintf(stdout, "failure");
        usleep(100000);
        sendData(DIRECTORPORT, fromName, retMessage);
        return 0;
    }
    return 1;
}

/**
 * clears off the dataType character then forwards to the original collector
 * expecting message: <data> [to_collector] <Collector_hostname>
 * sending: <data>
 */
int forwardingToCollector(char* recMessage) {
    char *junk;
    junk = strstr(recMessage, TO_COLLECT);
    
    int len = strlen(recMessage)-strlen(junk);
    char sendString[len];
    
    strncpy(sendString, recMessage, len);
    
    fprintf(stdout, "Sending message: %s\n", sendString);
    
    char* toName = junk+strlen(TO_COLLECT)+2;
    
    printf("sendingTo: %s", toName);
    sendData(DIRECTORPORT, toName, sendString);
    return 0;
}

/**
 * registers a new analyst and adds the type and hostname to text file
 * expecting message: <type> [add_analyst] <Analyst_hostname>
 * writes <typeChar> " " <AnalystHostname> to "typeTable"
 */
int addAnalyst(char* message) {

    FILE *table = fopen("typeTable", "a");
    
    if(table == NULL) {
        table = fopen("typeTable", "w+");
        char initialise[]="Director: \n";
        fwrite(initialise, sizeof(char), sizeof(initialise), table);
    }
    
    char type[1];
    type[0]= message[0];
    
    fwrite(type,sizeof(char), 1, table);
    
    char* start;
    start = strstr(message, ADD_ANALYST);
    
    fwrite(" ",sizeof(char),1, table);
    
    char hostname[(strlen(message)+1)-strlen(ADD_ANALYST)];
    strcpy(hostname,start+strlen(ADD_ANALYST));
    
    fprintf(stdout, "Hostname: %s\n", hostname);
    
    fwrite(hostname,sizeof(char),sizeof(hostname), table);
    fwrite("\n",sizeof(char),1, table);
    
    fclose(table);
    
    return 0;

}

/**
 * checks the message for the barriers/identifiers
 * expecting message: <data> [identifier] <hostname>
 * returns an int to identify where to go
 */
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
    return -1;
}

/**
 * receives the data
 * sends the data to the correct path via deparsing the message
 * \return 0 for success, 1 failure
 */
int receiveDirectorMessage() {
    char recMessage[200];
    receiveData(DIRECTORPORT, recMessage);
    
    printf("Received Message: %s\n", recMessage);
    
    int messageType = deParseMessage(recMessage);
    
    printf("Message Type: %d\n", messageType);
    
    switch(messageType) {
        case 0:
            forwardingToCollector(recMessage);
            return 0;
            break;
        case 1:
            checkType(recMessage);
            printf("finished");
            return 0;
            break;
        case 2:
            forwardingToAnalyst(recMessage);
            return 0;
            break;
        case 3:
            addAnalyst(recMessage);
            return 0;
            break;
        default:
            return 1;
    }
    return 0;
}

/**
 * the main function
 * infinite loop to continuously receive data and process them accordingly
 * only returns 1 on failure
 */
int main() {
    char hostname[100];
    myIP(hostname);
    printf("Director Hostname: %s\n",hostname);
    
    while(1) {
        receiveDirectorMessage();
        printf("Completed Action\n");
        printf("Director Hostname: %s\n\n",hostname);
    }
    
    return 1;
}

