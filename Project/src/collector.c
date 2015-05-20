
#include "collector.h"

int getBankNumber(){
	FILE *wallet;
	char* message = "";
	char myNameIs[50];
	gethostname(myNameIs, sizeof(myNameIs));
	char nameIs[57] = "initial";
	strcat(nameIs, myNameIs);
	char* marshalMathers = nameIs; //yeah, lunch was on my mind

// hard to keep "humour" out when hungry.
//	encrypt_CB(marshalMathers);
//	SSL_CB(marshalMathers);
//	sendData(BANKPORT, BANK_NAME, marshalMathers);
	
	char newNum[20];
	sprintf(newNum, "%s", message);
	newNum[19] = '\0';
	char numCoin[12];
	sprintf(numCoin, "%010d\n", 0);
	message = newNum;
	wallet = fopen("byteCoin", "w+");


//	receiveData(BANKPORT, message);
//	decrypt_BC(message);
	
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

int buy_eCent() {//we can change the void to an int input to spec how many
		     // ecents to buy.
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
	
	char bID[20];
	fgets(bID, 20, wallet);
	sprintf(bID, "%019d", 1);
	message = bID;
	sprintf(type, "collect%s", message);
	message = type;
//	encrypt_CB(message);
//	SSL_CB(message);
	sendData(BANKPORT, BANK_NAME, message);
	fclose(wallet);
	receiveeCent();	
	
        return 0;
}

int sending(int data, char* message) {

    //need to check the director
    if(checkDirector(data) != 0) {//someone else's problem ~Matt~
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
	message = outerLayer;
	
//    encrypt_CD(message);
    
    //adding SSL from Collector to Director
//    SSL_CD(message);
    
    //sending the string
    if(sendData(DIRECTORPORT, DIRECTOR_NAME, message)==0) {
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
   // char* message = "check";
    
    //adding encryption from Collector to Director (CD)
//    encrypt_CD(message);
    
    //adding SSL from Collector to Director
//    SSL_CD(message);
  /*  
    if(sendData(DIRECTORPORT, DIRECTOR_NAME, message)==0) {
        //returning success
        return 0;
    }
    else {
        //returning failure
        return 1;
    }
    */
    return 0;
}

//receiving the processed Data from the director
int receivingData() {

    char* message = "";
    
    //receiving on the director to collector port wait until has received this data
//    receiveData(DIRECTORPORT, message);
    
  //  decrypt_DC(message);
    
    ///printing for when done?
    printf("%s",message);
    
    return 0;
}

