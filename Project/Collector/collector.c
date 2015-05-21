
#include "collector.h"

int getBankNumber(){
	FILE *wallet;
	char* message = "";
	char myNameIs[50];
	gethostname(myNameIs, sizeof(myNameIs));
	char nameIs[57] = "initial";
	strcat(nameIs, myNameIs);
	char* marshalMathers = nameIs; //yeah, lunch was on my mind

    printf("sending number!\n");
	sendData(BANKPORT, bankName, marshalMathers);
    printf("sent number\n");
	
	char newNum[20];
	sprintf(newNum, "%s", message);
	newNum[19] = '\0';
	char numCoin[12];
	sprintf(numCoin, "%010d\n", 0);
	message = newNum;
	wallet = fopen("byteCoin", "w+");
    
    printf("receiving number\n");
	receiveData(BANKPORT, message);
    printf("got number!");
	
	fwrite(message, 19, 1, wallet);
	fwrite(&numCoin, 12, 1, wallet);

	fclose(wallet);
	return 0;
}


int receiveeCent(){
 	char* message = "";
	receiveData(BANKPORT, message);
	FILE *wallet;
	wallet = fopen("byteCoin", "r+");
	if(wallet == NULL){
		return 1;
	}
	fseek(wallet, 30, SEEK_SET);
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
	char coin[strlen(message)];
	sprintf(coin, "%s", message);
	fwrite(&coin, sizeof(coin), 1, wallet);
	fseek(wallet, 19, SEEK_SET);
	sprintf(coin, "%010d\n", 10);
	fwrite(&coin, 11, 1, wallet);
	fclose(wallet);
	return 0;
}

int buy_eCent() {
    char* message ;
	FILE *wallet; 
	/**
	wallet config:
	Bank Account Number: 19 chars + '\0'
	Number of eCoins stored: 10 chars + '\0'
	eCoin numbers: 10 chars + '\n'
	end byte: '\0'
	*/
	wallet = fopen("byteCoin", "r+");//opening hard storage of eCent numbers
	char type[27];
	if(wallet == NULL){
		getBankNumber();
		wallet = fopen("byteCoin", "r+");
	}
	
    char myNameIs[50];
    gethostname(myNameIs, sizeof(myNameIs));
    
	char bID[20];
	fgets(bID, 20, wallet);
	sprintf(bID, "%019d", 1);
	message = bID;
	sprintf(type, "collect%s", message);
    sprintf(message, "%s%s", type, myNameIs);
	sendData(BANKPORT, bankName, message);
    usleep(10000);
	fclose(wallet);
	receiveeCent();	
	
        return 0;
}

int sending(int data, char* message) {

    //need to check the director
    if(checkDirector(data) != 0) {//someone else's problem ~Matt~
        printf("not valid type");
        return 1;
        //wait for a while? until director has an analyst?
    }
    
    //add something to front of string to identify a data exchange
    /* somethign to put together encrypted stirng*/
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
	z--;
	sprintf(witCoin, "%010d\n", z);
	fread(myID, 20, 1, wallet);
	fwrite(witCoin, 11, 1, wallet);
	fseek(wallet, z*(11), SEEK_CUR);
	sprintf(witCoin,"%s", myID);
	fread(witCoin+19, 10, 1, wallet);
	sprintf(witCoin+30,"%s", message);
	witCoin[-1] ='\0';
	message = witCoin;
//	encrypt_CA(message);
//	SSL_CA(message);
	char myname[50];
	
	gethostname(myname, sizeof(myname));
	char outerLayer[strlen(message)+strlen(myname)+9];
	sprintf(outerLayer, "%sfromCol%d%s",myname, data, message);
/*
	Here's the ID string: "fromCol"
*/

	message = outerLayer;
    
    //sending the string
    if(sendData(DIRECTORPORT, directorName, message)==0) {
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
    char myName[50];
    gethostname(myName, sizeof(myName));

    char* message;
    sprintf(message, "%d%s%s", type, TO_ANALYST, myName);
  
    sendData(DIRECTORPORT, directorName, message);
    usleep(10000);
    
    char recMesg[100];
    receiveData(DIRECTORPORT, recMesg);
//	SSL_read(recMesg, decMesg);
	
	char sucS[] = "success";
	if(strcmp(recMesg, sucS) == 0){
		return 0;
	}
	else{
		return 1;
	}
}


//receiving the processed Data from the director
int receivingData() {
	FILE *listData;
	int list;
	listData = fopen("endUser", "r+");
	if(listData == NULL){
		listData = fopen("endUser", "w+");
		list = 0;
		fwrite(&list, sizeof(int), 1, listData);
	}
	else{fread(&list, sizeof(int), 1, listData);}
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

int showData(){
	FILE *listData;
	listData= fopen("endUser", "r");
	fseek(listData, sizeof(int), SEEK_SET);
	char buff[100];
	while(fgets(buff, 100, listData) != NULL){
		printf("%s\n", buff);
	}
	return 0;
 }

int main(int argc, char* argv[]) {
    
    if(argc!= 4) {
        printf("usage: %s directorName bankName walletSize\n", argv[0]);
        return 1;
    }
    
    directorName = argv[1];
    bankName = argv[2];
    
    int stash;
    
    printf("getting bank no.\n");
    getBankNumber();
    printf("got bank no.\n\n");
    
    for (stash = 0; stash<atoi(argv[3]); stash++) {
        printf("buying eCent %d\n", stash);
        usleep(10000);
        buy_eCent();
    }

    int type;
    char* message = malloc(100*sizeof(char));
    
    while(1) {
        printf("type:   ");
        scanf("%d", &type);
        printf("message:    ");
        scanf("%s", message);
        
        printf("Sending...\n\n");
        //sending(type, message);
        continue;
    }
    
    free(message);
    return 0;
}

