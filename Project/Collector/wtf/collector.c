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
    FILE *listData;
    FILE *wallet =fopen("byteCoin", "r");
    listData= fopen("endUser", "r+");
    char buff[100];
    while(fgets(buff, 100, listData) != NULL){
        printf("%s\n", buff);
        
    }
    while(fgets(buff, 100, wallet) != NULL){
        printf("%s\n", buff);
    }
    fclose(listData);
    fclose(wallet);
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
	//sprintf(newNum, "%s", message); // seems pointless, as message should be empty
	newNum[19] = '\0';
    
	char numCoin[12];
	sprintf(numCoin, "%010d\n", 0);
	message = newNum;
	wallet = fopen("byteCoin", "w+");
    
    printf("Receiving bank number\n");
	receiveData(BANKPORT, message);
    printf("Got number!");
    printf("\nNumber is: %s\n", message); 
	
	fwrite(message, 19, 1, wallet);
	fwrite(&numCoin, 12, 1, wallet);
    
    char TESTCHAR[20];
    fseek(wallet, 0, SEEK_SET);
    fread(&TESTCHAR, 19, 1, wallet);
    TESTCHAR[19] = '\0';
    printf("WALLET: %s\n",TESTCHAR);

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
    printf("Received eCent: %s\n", message);
 
	FILE *wallet;
	wallet = fopen("byteCoin", "r+");
 
	if(wallet == NULL){
		return 1;
	}

	fseek(wallet, 29, SEEK_SET);
//	decrypt_BC(message);
	char* noDosh = "NO SOUP FOR YOU!";
	char* badID = "Bad ID";
 
	if(strcmp(message, noDosh) == 0){//or something
	// wait a bit and try again
		return 2;
	}
 
	else if(strcmp(message, badID) == 0){
		// indicating badID
		//Try again.
		return 3;
	}
 
	fwrite(&coin, sizeof(coin), 1, wallet);
	fseek(wallet, 19, SEEK_SET);
	sprintf(coin, "%010d", 10);
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
  char mesgB[200];
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
	fread(&bID, 19,1, wallet);
	bID[19] = '\0';
	message = bID;
	sprintf(type, "collect%s", message);
	printf("CheckThing%s\n", type);
	message = mesgB;
    //MATTHEW FITZPATRICK
    sprintf(message, "%s%s", type, myNameIs);
    
    printf("message Sent: %s", message);
    
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
    //need to check the director
    if(checkDirector(daType) != 0) {
        printf("Not valid type");
        return 1;
        //wait for a while? until director has an analyst?
    }
    
    //add something to front of string to identify a data exchange
     //somethign to put together encrypted stirng
    //adding encryption from Collector to Director (CD)
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
	fread(&walSize, 10,1, wallet);
	if(atoi(walSize) == 0){
		buy_eCent();
		fseek(wallet, 19, SEEK_SET);
		fread(&walSize, 10,1, wallet);
	}
	int z = atoi(walSize);
	z--;
	
	sprintf(walSize, "%010d", z);
	printf("WALLET_SIZE: %d\n", z);
	fseek(wallet, 19, SEEK_SET);
	fwrite(&walSize, 10, 1, wallet);
	fseek(wallet, z*(10), SEEK_CUR);
	char coinID[11];
	fread(coinID, 10, 1, wallet);
	coinID[10] = '\0';
	printf("COINID: %s\n", coinID);
	sprintf(witCoin,"%s%s%s", myID, coinID, message);
	printf("WITCOIN: %s\n", witCoin);
	witCoin[-1] ='\0';
	message = witCoin;

    
    fclose(wallet);
    
	char myname[50];
	myIP(myname);
	char outerLayer[strlen(message)+strlen(myname)+9];
	sprintf(outerLayer, "%c%sto_analyst%s",daType, message, myname);

	message = outerLayer;
	
    printf("sending: %s", message);
    
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
    //something to send to Director to identify the request
    char myName[50];
    myIP(myName);
    
    char message[100];
    sprintf(message, "%c%s%s", type, CHECK_TYPE, myName);
    printf("Sending message: %s\n", message);
    
    sendData(DIRECTORPORT, directorName, message);
    
	char recMesg[100];
    receiveData(DIRECTORPORT, recMesg);
    
    printf("got to here");
	char sucS[] = "success";
	if(strcmp(recMesg, sucS) == 0){//change to decMesg once decryption's up
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
      
    printf("getting bank no.\n");
    getBankNumber();
    printf("got bank no.\n\n");
    
    int i;
    for(i=0; i<atoi(argv[3]); i++) {
        printf("buying eCent package no. %d\n", 1);
        usleep(10000);
        buy_eCent();
    }
    
    char type[2000];
    char* message = malloc(100*sizeof(char));
    
    while(1) {
        printf("type:   ");
        scanf("%s", type);
        if(strlen(type )!= 1){
            printf("Ending collector script. Printing received messages\n\n");
            showData();
            return 0;
        }
        
        //checkdirector
        printf("message:    ");
        scanf("%s", message);
        
        printf("Sending...\n\n");
        sending(type, message);
        usleep(100000);
        continue;
    }
 
    showData();
    return 0;
}


