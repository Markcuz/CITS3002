/**
 * The collector
 * \author Matthew Fitzpatrick 21297239
 * \author Marcus Pham 20495924
 */
#include "collector.h"

/**
 * prints the data in the endUser file
 */
int showData(){
    /*FILE *listData;
    FILE *wallet =fopen("byteCoin", "r");
    
    if(wallet== NULL){
        fopen("byteCoin", "w+");
        printf("Nothing in File");
        return 1;
    }
    
    listData= fopen("endUser", "r+");
    
    if(listData== NULL){
        listData= fopen("endUser", "w+");
        fclose(listData);
        fclose(wallet);
        printf("Nothing in File");
        return 1;
    }
    
    char buff[100];
    while(fgets(buff, 100, listData) != NULL){
        printf("%s\n", buff);
        
    }
    while(fgets(buff, 100, wallet) != NULL){
        printf("%s\n", buff);
    }
    
    fclose(listData);
    fclose(wallet);*/
    return 0;
}
/**
 * gets the bank number (initialises account)
 * send Message: [initial] <myHostname>
 * receive Message:
 * writes message to text file "byteCoin"
 */
int getBankNumber(){
    FILE *wallet;
    char* message;
    
    char myNameIs[50];
    myIP(myNameIs);
    char nameIs[57] = "initial";
    strcat(nameIs, myNameIs);
    char* marshalMathers = nameIs;
    
    printf("Requesting bank number\n");
    sendData(BANKPORT, bankName, marshalMathers);
    
    char newNum[20];
    newNum[19] = '\0';
    
    char numCoin[12];
    sprintf(numCoin, "%010d\n", 0);
    message = newNum;
    wallet = fopen("byteCoin", "w+");
    
    receiveData(BANKPORT, message);
    fwrite(message, 19, 1, wallet);
    fwrite(&numCoin, 12, 1, wallet);
    
    char TESTCHAR[20];
    fseek(wallet, 0, SEEK_SET);
    fread(&TESTCHAR, 19, 1, wallet);
    TESTCHAR[19] = '\0';
    fclose(wallet);
    return 0;
}

/**
 * receives and eCent from the bank
 * rec Message :
 * stores eCent inside "byteCoin" for later use
 */
int receiveeCent(){
    char coin[999];
    char* message;
    message= coin;
    
    
    receiveData(BANKPORT, message);
    FILE *wallet;
    wallet = fopen("byteCoin", "r+");
    
    if(wallet == NULL){
        return 1;
    }
    
    fseek(wallet, 29, SEEK_SET);
    char* noDosh = "NO SOUP FOR YOU!";
    char* badID = "Bad ID";
    
    if(strcmp(message, noDosh) == 0){
        return 2;// indicating bank has no reserve of eCents left
    }
    
    else if(strcmp(message, badID) == 0){
        // indicating badID
        return 3;
    }
    
    fwrite(&coin, sizeof(coin), 1, wallet);//writing eCents to wallet
    fseek(wallet, 19, SEEK_SET);
    sprintf(coin, "%010d", 10);//number of stored ecents
    fwrite(&coin, 10, 1, wallet);
    fclose(wallet);
    
    return 0;
}

/**
 * requests an eCent from the bank
 * sendMessage: <accNumber> <hostname>
 * receives the eCent
 */
int buy_eCent() {
    char mesgB[200];//memory for message to Bank
    char* message;
    message= mesgB;
    FILE *wallet;
    wallet = fopen("byteCoin", "r+");//opening hard storage of eCent numbers
    char type[27];
    if(wallet == NULL){
        getBankNumber();
        wallet = fopen("byteCoin", "r+");
    }
    
    char myNameIs[50];
    myIP(myNameIs);
    
    printf("my hostname: %s\n", myNameIs);
    
    char bID[20];
    fread(&bID, 19,1, wallet);//bank number
    bID[19] = '\0';
    message = bID;
    sprintf(type, "collect%s", message);
    message = mesgB;
    sprintf(message, "%s%s", type, myNameIs);

    
    usleep(100000);
    sendData(BANKPORT, bankName, message);
    receiveeCent();
    fclose(wallet);
    
    
    
    return 0;
}

/**
 * sends the actual data requiring analysis
 * send Message: <dataType> <payment> <data> [to_analyst] <myHostname>
 */
int sending(char* data, char* message) {
    char daType = data[0];
    //need to check if the director has an analyst of the correct type
    if(checkDirector(daType) != 0) {
        printf("Not valid type");
        return 1;
        
    }
    
    
    char myID[20];
    char walSize[11];
    char witCoin[strlen(message) + 30];
    FILE *wallet;
    showData();
    wallet = fopen("byteCoin", "r+");
    if(wallet == NULL){
        buy_eCent();
        wallet = fopen("byteCoin", "r+");
    }
    fread(&myID, 19, 1, wallet);
    fread(&walSize, 10,1, wallet);//using wallet as a stack
    if(atoi(walSize) == 0){
        buy_eCent();
        fseek(wallet, 19, SEEK_SET);
        fread(&walSize, 10,1, wallet);
    }
    int z = atoi(walSize);
    z--;
    
    sprintf(walSize, "%010d", z);
    fseek(wallet, 19, SEEK_SET);
    fwrite(&walSize, 10, 1, wallet);
    fseek(wallet, z*(10), SEEK_CUR);
    char coinID[11];
    fread(coinID, 10, 1, wallet);
    coinID[10] = '\0';
    sprintf(witCoin,"%s%s%s", myID, coinID, message);
    witCoin[-1] ='\0';
    message = witCoin;
    
    
    fclose(wallet);
    char myname[50];
    myIP(myname);
    char outerLayer[strlen(message)+strlen(myname)+9]; 
    sprintf(outerLayer, "%c%sto_analyst%s",daType, message, myname);
    
    message = outerLayer;
    usleep(1000000);
    if(sendData(DIRECTORPORT, directorName, message)==0) {
        printf("Sent data");
        receivingData();
        return 0;
    }
    else {
        printf("failed to send");
        return 1;
    }
    return 0;
}

/**
 * asks the director if he can forward the dataType to any known analyst
 * send Message: <type> [check_type] <myHostname>
 * rec Message: "success" or "failure"
 * returns 0 on success, 1 on failure
 */
int checkDirector(char type) {
    char myName[50];
    myIP(myName);
    
    char message[100];
    sprintf(message, "%c%s%s", type, CHECK_TYPE, myName);
    
    sendData(DIRECTORPORT, directorName, message);
    
    char recMesg[100];
    receiveData(DIRECTORPORT, recMesg);
    
    char sucS[] = "success";
    if(strcmp(recMesg, sucS) == 0){
        return 0;
    }
    else{
        return 1;
    }
    return 1;
    
}

/**
 * receives the processed Data from the director
 * rec Message: <data>
 * prints data to file "endUser"
 */
int receivingData() {
    FILE *listData;
    listData = fopen("endUser", "a");
    
    if(listData == NULL){
        listData = fopen("endUser", "w+");
    }
    char datalink[200];
    char* message;
    message = datalink;
    receiveData(DIRECTORPORT, message);
    fwrite(message, strlen(message), 1, listData);
    fwrite("\n",sizeof(char),1, listData);
    fclose(listData);
    return 0;
}



/**
 * the main function
 * initialises the hostnames of bank and director
 * specifies how many eCents to buy
 * then prompts user for sending messages
 */
int main(int argc, char* argv[]) {
    
    if(argc!= 4) {
        printf("usage: %s directorName bankName walletSize\n", argv[0]);
        return 1;
    }
    
    directorName = argv[1];
    bankName = argv[2];
    
    
    getBankNumber();
    
    
    int i;
    for(i=0; i<atoi(argv[3]); i++) {//designed to allow for larger wallet sizes Present funcionality limits size to 1
        printf("buying eCent package no. %d\n", (i+1));
        usleep(10000);
        buy_eCent();
    }
    
    char type[2000];
    char* message = malloc(1000*sizeof(char));
    
    while(1) {
        printf("type:   ");
        scanf("%s", type);
        if(strlen(type )!= 1){
            printf("Ending collector script.\n\n");
            //showData(); 
            return 0;
        }
        

        printf("message, without spaces:    ");
        scanf("%s", message);
        
        printf("Sending...\n\n");
        sending(type, message);
        usleep(100000);
        continue;
    }
    
    
    return 0;
}

