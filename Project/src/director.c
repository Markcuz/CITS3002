#include "director.h"

//expecting message: <type> <data> <identifier> <collectorHostname>
int forwardingToAnalyst(char* recMessage){
    
    fprintf(stdout, "incoming message: %s\n", recMessage);

    FILE* table;
    ssize_t read;
    size_t len = 0;
    char* toName;
    
    table = fopen("typeTable", "r+");
    char* line = NULL;
    while((read = getline(&line,&len,table)) != -1) {
        if(line[0]==recMessage[0]) {
            strtok(line, " \n");
            toName = strtok(NULL, " \n");
            break;
        }
    }
    
    fprintf(stdout, "analyst hostname: %s\n", toName);
    
    //stripping type
    /*
    char* sendMessage = recMessage+1;
    fprintf(stdout, "message: %s\n", sendMessage);
    */
    
    sendData(DIRECTORPORT, toName, recMessage);
    
    return 0;
}

//expecting message: <type> <identifier> <Collector_hostname>
int checkType(char* message){
    
    fprintf(stdout, "incoming message: %s\n", message);
    
    int typeAvail = 0;
    //checks the table to see if can send
    FILE* table;
    ssize_t read;
    size_t len = 0;
    
    table = fopen("typeTable", "r+");
    char* line = NULL;
    while((read = getline(&line,&len,table)) != -1) {
        if(line[0]==message[0]) {
            fprintf(stdout, "line: %s\n", line);
            typeAvail = 1;
            break;
        }
    }
    
    char* fromName = strstr(message, CHECK_TYPE)+sizeof(CHECK_TYPE)-1;
    fprintf(stdout, "from name: %s\n", fromName);
    
    if(typeAvail) {
        char retMessage[10] = "success";
        //create some message to tell available
        fprintf(stdout, "success");
        //sendData(DIRECTORPORT, fromName, retMessage);
        return 0;
    }
    else {
        char retMessage[10] = "failure";
        fprintf(stdout, "failure");
        //sendData(DIRECTORPORT, fromName, retMessage);
        return 0;
    }
    return 1;
}

//clears off the dataType character then forwards to the first analyst that cna analyse it
//expecting message: <data> <identifier> <Collector_hostname>
//sending: <data>
int forwardingToCollector(char* recMessage) {

    fprintf(stdout, "incoming message: %s\n", recMessage);
    
    char *junk;
    junk = strstr(recMessage, TO_COLLECT);
    
    int len = strlen(recMessage)-strlen(junk);
    
    char sendString[len];
    
    strncpy(sendString, recMessage, len);
    
    fprintf(stdout, "sending message: %s\n", sendString);
    
    //figure out who to send to via message
    //sendData(DIRECTORPORT, toName, recMessage);
    return 0;
}

//expecting message: <type> <identifier> <Analyst_hostname>
//writes <typeChar> " " <AnalystHostname> to "typeTable"
int addAnalyst(char* message) {
    
    fprintf(stdout, "incoming message: %s\n", message);
    
    //add analyst to the table
    FILE *table = fopen("typeTable", "a");
    
    if(table == NULL) {
        table = fopen("typeTable", "w+");
        //add in maybe a number for the director no.
        char initialise[]="Director: \n";
        fwrite(initialise, sizeof(char), sizeof(initialise), table);
    }
    
    char type[1];
    type[0]= message[0];

    fwrite(type,sizeof(char), 1, table);
    
    char* start;
    start = strstr(message, ADD_ANALYST);
    
    fprintf(stdout, "start hostname: %s\n", start);
    
    fwrite(" ",sizeof(char),1, table);
    
    char hostname[(strlen(message)+1)-strlen(ADD_ANALYST)];
    strcpy(hostname,start+strlen(ADD_ANALYST));
    
    fprintf(stdout, "hostname: %s\n", hostname);
    
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
    return -1;
}

int receiveDirectorMessage() {
    //char* recMessage = "cadd_analystmyHostname"; testing adding analyst
    //char* recMessage = "ccheck_typemyHostname"; testing check
    //char* recMessage = "cto_analysthostname";
    //char* recMessage = "daat.to_collectorhostname";
    char* fromName;
    
    //receiveData(DIRECTORPORT, recMessage);
    int messageType = deParseMessage(recMessage);
    
    fprintf(stdout, "type: %d\n", messageType);
    
    switch(messageType) {
        case 0:
            forwardingToCollector(recMessage);
            break;
        case 1:
            checkType(recMessage);
            break;
        case 2:
            forwardingToAnalyst(recMessage);
            break;
        case 3:
            addAnalyst(recMessage);
            break;
        default:
            return 1;
    }
    return 1;
}

int main() {
    receiveDirectorMessage();
    fprintf(stdout, "done\n");
    return 0;
}
