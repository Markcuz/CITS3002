/**
 * The collector
 * \author Matthew Fitzpatrick 21297239
 * \author Marcus Pham 20495924
 */
#include "collector.h"

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
	sprintf(coin, "%010d\n", 10);
	fwrite(&coin, 11, 1, wallet);
	fclose(wallet);
 
	return 0;
}

/**
 * requests an eCent from the bank
 * sendMessage: <accNumber> <hostname>
 * receives the eCent
 */
int buy_eCent() {
    char message[200];
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
	fgets(bID, 20, wallet);
	sprintf(bID, "%019d", 1);
	message = bID;
	sprintf(type, "collect%s", message);
    
    //MATTHEW FITZPATRICK
    sprintf(message, "%shi %s", type, myNameIs);
    
    printf("message Sent: %s", message);
    
    usleep(100000);
	sendData(BANKPORT, bankName, message);
	receiveeCent();
	fclose(wallet);
    
    free(message);

    return 0;
}

/**
 * sends the actual data requiring analysis
 * send Message: <dataType> <payment> <data> [to_analyst] <myHostname>
 */
int sending(int data, char* message) {

    //need to check the director
    if(checkDirector(data) != 0) {
        printf("Not valid type");
        return 1;
        //wait for a while? until director has an analyst?
    }
    
    //add something to front of string to identify a data exchange
     //somethign to put together encrypted stirng
    //adding encryption from Collector to Director (CD)
	char myID[20];
	char witCoin[strlen(message) + 30];
	FILE *wallet;
	wallet = fopen("byteCoin", "r+");
	if(wallet == NULL){
		buy_eCent();
		wallet = fopen("byteCoin", "r+");
	}
	fseek(wallet, 19, SEEK_SET);
	fgets(witCoin, 11, wallet);
	if(atoi(witCoin) == 0){
		buy_eCent();
		fseek(wallet, 20, SEEK_SET);
		fgets(witCoin, 11, wallet);
	}
	int z = atoi(witCoin);
	z--;/*
	sprintf(witCoin, "%010d\n", z);
	fread(myID, 20, 1, wallet);
	fwrite(witCoin, 11, 1, wallet);
	fseek(wallet, z*(11), SEEK_CUR);
	sprintf(witCoin,"%s", myID);
	fread(witCoin+19, 10, 1, wallet);
	sprintf(witCoin+30,"%s", message);
	witCoin[-1] ='\0';
	message = witCoin;*/
	
	sprintf(witCoin, "%010d\n", z);
	fread(myID, 20, 1, wallet);
	fwrite(witCoin, 11, 1, wallet);
	fseek(wallet, z*(11), SEEK_CUR);
	char coinID[11];
	fread(coinID, 10, 1, wallet);
	coinID[10] = '\0';
	sprintf(witCoin,"%s%s%s", myID, coinID, message);
    printf("WITCOIN: %s\n", witCoin);
	witCoin[-1] ='\0';
	message = witCoin;

    
    fclose(wallet);
    
	char myname[50];
	myIP(myname);
	char outerLayer[strlen(message)+strlen(myname)+9];
	sprintf(outerLayer, "%d%sto_analyst%s",data, message, myname);

	message = outerLayer;
	
    printf("sending: %s", message);
    
    usleep(1000000);
    if(sendData(DIRECTORPORT, directorName, message)==0) {
        printf("Sent data");
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
int checkDirector(int type) {
    //something to send to Director to identify the request
    char myName[50];
    myIP(myName);
    
    char message[100];
    sprintf(message, "%d%s%s", type, CHECK_TYPE, myName);
    printf("Sending message: %s\n", message);
    
    sendData(DIRECTORPORT, directorName, message);
    
	char recMesg[100];
    receiveData(DIRECTORPORT, recMesg);
//	SSL_read(recMesg, decMesg);
	
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
	int list;
	listData = fopen("endUser", "r+");
	if(listData == NULL){
		listData = fopen("endUser", "w+");
		list = 0;
		fwrite(&list, sizeof(int), 1, listData);
	}
    
	else{
        fread(&list, sizeof(int), 1, listData);
    }
	char* gotData;
	char* message;
	receiveData(DIRECTORPORT, gotData);
//	SSL_read(gotData, message, strlen(gotData));
	fseek(listData, list*strlen(message), SEEK_CUR);
	list++;
	fwrite(&message, strlen(message), 1, listData);
	fseek(listData, 0, SEEK_SET);
	fwrite(&list, sizeof(int), 1, listData);
	fclose(listData);
	return 0;
}

/**
 * prints the data in the endUser file
 */
int showData(){
	FILE *listData;
	FILE *wallet =fopen("byteCoin", "r");
	listData= fopen("endUser", "w+");
//	if(listData == NULL){return 1;}
	fseek(listData, sizeof(int), SEEK_SET);
	char buff[100];
	while(fgets(buff, 100, listData) != NULL){
		printf("%s\n", buff);
		
	}
	while(fgets(buff, 100, wallet) != NULL){
		printf("%s\n", buff);
	}
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
    
    int type;
    char* message = malloc(100*sizeof(char));
    
    while(1) {
        printf("type:   ");
        scanf("%d", &type);
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


