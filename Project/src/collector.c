
#include "collector.h"
char* getBankNumber(){
	FILE *wallet;
	char* message;
	char myNameIs[50];
	gethostname(myNameIs, sizeof(myNameIs));
	char nameIs[57] = "initial";
	strcat(nameIs, myNameIs);
	char* marshalMathers = nameIs; //yeah, lunch was on my mind
// hard to keep "humour" out when hungry.
	encrypt_CB(marshalMathers);
	SSL_CB(marshalMathers);
	sendData(BANKPORT, BANK_NAME, marshalMathers);
	wallet = fopen("byteCoin", "w+");
	receiveData(BANKPORT, message);
	decrypt_BC(message);
	char space[20];
	message = space;
	fwrite(message, 20, 1, wallet);
	fclose(wallet);
	return message; 
}


int receiveeCent(){
 	char* message;
	receiveData(BANKPORT, message);
	FILE *wallet;
	wallet = fopen("bytecoin", "r+");
	fseek(wallet, 31, SEEK_SET);
	decrypt_BC(message);
	if(message == "no soup for you!"){return 1;} //or something
	char coin[12];
	int z =0;
	while(sizeof(strcpy(coin, message+(z*11))) == 12){
		coin[11] = '\0';
		fwrite(&coin, 11, 1, wallet);
		z++;
	}
	message = '\0';
	fwrite(message, 1,1,wallet);
	fseek(wallet, 20, SEEK_SET);
	sprintf(coin, "%010d\n", z);
	fwrite(&coin, 11, 1, wallet);
	fwrite(message, 1,1,wallet);
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
	}
	else{
		char bID[20];
		fgets(bID, 20, wallet);
		message = bID;
	}
	sprintf(type, "collect%s", message);
	message = type;
	encrypt_CB(message);
	SSL_CB(message);
	sendData(BANKPORT, BANK_NAME, message);
	fclose(wallet);
	receiveeCent();	

        return 0;
}

int sending(int data, char* message) {

    //need to check the director
    if(checkDirector(3) != 0) {//someone else's problem ~Matt~
        //wait for a while? until director has an analyst?
    }
    
    //add something to front of string to identify a data exchange
    /* somethign to put together encrypted stirng*/
    //adding encryption from Collector to Director (CD)
	char myID[20];
	char witCoin[strlen(message) + 31];
	FILE *wallet;
	wallet = fopen("byteCoin", "r+");
	if(wallet == NULL){
		buy_eCent();
		wallet = fopen("byteCoin", "r+");
	}
	fgets(witCoin, 100, wallet);
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
	fread(witCoin, 10, 1, wallet);
	sprintf(witCoin+10,"%s%s", myID, message);
	witCoin[-1] ='\0';
	message = witCoin;
	encrypt_CA(message);
	SSL_CA(message);
	char myname[50];
	
	gethostname(myname, sizeof(myname));
	char outerLayer[strlen(message)+strlen(myname)];
	sprintf(outerLayer, "fromCol%d%s%s",data, message, myname);
	message = outerLayer;
	
    encrypt_CD(message);
    
    //adding SSL from Collector to Director
    SSL_CD(message);
    
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
    char* message = "check";
    
    //adding encryption from Collector to Director (CD)
    encrypt_CD(message);
    
    //adding SSL from Collector to Director
    SSL_CD(message);
    
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

//receiving the processed Data from the director
int receivingData() {

    char* message;
    
    //receiving on the director to collector port wait until has received this data
    receiveData(DIRECTORPORT, message);
    
    decrypt_DC(message);
    
    ///printing for when done?
    printf("%s",message);
    
    return 0;
}

