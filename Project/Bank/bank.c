
#include "bank.h"
/**
 * The bank
 * \author Matthew Fitzpatrick 21297239 (main source)
 * \author Marcus Pham 20495924 (debugging and skeleton)
 */

//the banks own ID. Reserved to note reserve of eCents
char banksID[20] = "0000000000000000000";

/**
 * starts the economy
 * creates 3 text files centdepot centbank bankacc
 * stores all relevant econmy data inside files to cross check for authenticity later
 */
void econStart(){
	FILE *centBanked = fopen("centdepot", "w+");
	FILE *centList = fopen("centBank", "w+");
	FILE *lastAcc = fopen("bankAcc", "w+");
    
	int banknum =0;
	fwrite(&banknum, sizeof(int), 1, lastAcc);
	int a = 10000;
	fwrite(&a, sizeof(int),1, centBanked);
    
    
	for(int i = 0; i < a; i++){ //subject to change or alteration if I get a good idea
		eCent cent;
		cent.identifier = i;
		strcpy(cent.owner, banksID); //reserved bank ID
		fwrite(&cent, sizeof(eCent), 1, centBanked);
    	}
    
    //Did two 'for' loops due to seg fault on MACOSX
    for(int i = 0; i < a; i++){
        eCent cent2;
        cent2.identifier = i;
        strcpy(cent2.owner, banksID); //reserved bank ID
    
        fwrite(&cent2, sizeof(eCent), 1, centList);
    }
    
	fclose(centBanked);
	fclose(centList);
	fclose(lastAcc);
}

/**
 * checks the deposit coming from the analyst
 * Rec Message: [deposit] <payment> <analystHostname>
 * Sent message: "Well done you" on success, "You dun goofed, son" on failure
 */
int checkDeposit(char* recMessage, char* fromName){
	 char oldID[20];
	char centS[11];
	strncpy(oldID,recMessage+7, 19);
	oldID[19]= '\0';
	strncpy(centS, recMessage+26, 10);
	centS[10]='\0';
	int centID = atoi(centS);

	char* sendMessage;
	FILE *centList;
	centList = fopen("centBank", "r+");
	if(centList == NULL){
		econStart();
		centList = fopen("centBank", "r+");
	}
	char accept[] = "Well done you";
    	char decline[] = "You dun goofed, son.";// Nack message;
    
	eCent cent;
	fseek(centList, centID*sizeof(eCent), SEEK_SET);
	fread(&cent, sizeof(eCent),1, centList);
	if((strcmp(cent.owner, oldID)==0) && (strcmp(cent.owner, banksID)!=0)){ //got the correct previous owner
		strcpy(cent.owner, banksID);
	    	fseek(centList, centID*sizeof(eCent), SEEK_SET);
		fwrite(&cent, sizeof(eCent), 1, centList); //updating owner to bank.
		fclose(centList); 
	    	FILE *centBanked = fopen("centdepot", "r+"); //storing the eCent with the rest of the bank's cache.
		int stash; //number of stored eCents
		fread(&stash, sizeof(int), 1, centBanked);
	        fseek(centBanked, stash*sizeof(eCent), SEEK_CUR); //to the stored eCents stack
	    	fwrite(&cent, sizeof(eCent), 1, centBanked); //pushing to top of stack
	        fseek(centBanked, 0, SEEK_SET);//back to storage number
	        stash++;
		fwrite(&stash, sizeof(int), 1, centBanked); //updating number of stored eCents
		fclose(centBanked);
	
	        sendMessage = accept; //something something ack message;
	}
	else{
		
		sendMessage = decline;
	}
	
    usleep(1000000);
	sendData(BANKPORT, fromName, sendMessage);
	return 0;
}

/**
 * gives eCents to the collector
 * Rec Message: [collect] <collectorHostname>
 */
int givePayment(char* recMessage, char* fromName) {
	FILE *centBanked = fopen("centdepot", "r+");
	if(centBanked == NULL){
		econStart();
		centBanked = fopen("centdepot","r+");
	}
	char ownID[20];//owner ID
	strncpy(ownID,recMessage+7, 19);
	ownID[19] = '\0';
	int a =0;
	
	FILE *lastAcc = fopen("bankAcc", "r+");
	int lAc;
	fread(&lAc,sizeof(int), 1 ,lastAcc);
	int b = atoi(ownID);
	int c = atoi(banksID);
	if(lAc>= b && c < b){
		a=a;
	}
    
	else {
		char badID[] = "Bad ID";
		char* ssld;//in expectation of SSL method
		ssld = badID;
		sendData(BANKPORT, fromName, ssld);
		return 0;
	}
	fread(&a, sizeof(int), 1, centBanked);
	if(a == 0){ // Game Over. Insert coin/s to continue.
		fclose(centBanked);
		char noDosh[] = "No soup for you!";
		char* failstring;
		failstring = noDosh;
		sendData(BANKPORT, fromName, failstring);
		return 0;
	}
	else if(a>10){ //more than enough cents to go around
		fseek(centBanked, 0, SEEK_SET);// Back to the start to edit the number of remaining cents
		int bd = a - 10;
		fwrite(&bd, sizeof(int), 1, centBanked); // Update
		fseek(centBanked, bd*(sizeof(eCent)), SEEK_CUR); //jumpt to almost end of list
		a=10; //sending default amount of 10

	}	
	else{ // either just enough, or not quite enough
		fseek(centBanked, 0, SEEK_SET);
		int bd = 0; //i't wasn't empty before, but it sure is now.
		fwrite(&bd, sizeof(int),1,centBanked); //update
	}
	
	eCent cent;
	char* sendMessage;
	char cents[a*11 +1]; // compilation of the eCent IDs.
	cents[0]= '\0';
	FILE *centList = fopen("centBank", "r+"); //open the log of who owns which eCents
	for(int i=0; i<a; i++){
		fread(&cent, sizeof(eCent), 1, centBanked); //get ecent from the bank's cache
		strcpy(cent.owner, ownID); //reallocating owner
		fseek(centList, (cent.identifier*(sizeof(eCent))), SEEK_SET); //looking up eCent's entry in owner table
		
		fwrite(&cent, sizeof(eCent), 1, centList); //update owner table
		char tempCent[11]; //temp string for strncat
		sprintf(tempCent, "%010d", cent.identifier); //add eCent number to string
		tempCent[10] = '\0';
		strncat(cents,tempCent, 11);
	}
	
	fclose(centBanked);
	fclose(centList);
    
   	sendMessage = cents;

    usleep(100000);
	sendData(BANKPORT, fromName, sendMessage);
	return 0;
}

/**
 * initialises an account for the collector
 * Rec Message: [initial] <collectorHostname>
 * Send Message: the ID of the bank
 */
int giveAccount(char* fromName){
	FILE *lastAcc = fopen("bankAcc", "r+");// last account to have been created
    
	if(lastAcc == NULL){
		econStart();
		lastAcc = fopen("bankAcc", "r+");
	}
	int nID =0;
	fread(&nID,sizeof(int), 1, lastAcc);
	nID+=1;
	char acnum[20];
	sprintf(acnum, "%019d", nID);
	fseek(lastAcc,0, SEEK_SET);
	fwrite(&nID, sizeof(int), 1, lastAcc);
	
	fseek(lastAcc,0, SEEK_SET);
	fread(&nID,sizeof(int), 1, lastAcc);
    
    usleep(1000000);
    

	sendData(BANKPORT, fromName, acnum);
	return 0;
}

/**
 * parses the incoming message and returns an int corresponding to the action required
 */
int deParse(char* mesg){
	char parse[8];
	strncpy(parse, mesg, 7);//message header contains 7 byte code corresponding to desired transaction type
	parse[7] = '\0';
	char an[] = "deposit";
	char col[] ="collect";
	char acc[] ="initial";//A collector wants to create a new bank account

	if(strcmp(parse, an) == 0){
        return 0;
    }
	else if(strcmp(parse, col)== 0){
        return 1;
    }
	else if(strcmp(parse, acc)==0){
        return 2;
    }
	else{
        printf("Not found");
        return 3;
    }
}

/**
 * finds the hostname of the person requiring service
 * returns the pointer to the hostname string
 * Rec message: <identifier> <data> <hostname>
 */
char *findName(char* recMessage, int cas){
    char* name;
    char serName[100];
    strcpy(serName,recMessage+cas);
    name = &serName[0];
    return name;
}

/**
 * receives the messages to the bank and parses them
 * message: <type> <data> <fromName>
 * returns 0 on success 1 on failure
 */
int receiveBankMessage() {
	char deMec[100];
	char datMec[100];
	char* recMessage; // for received transmission
	recMessage = deMec;
	receiveData(BANKPORT, recMessage);
	recMessage = deMec;
	int deposit = deParse(recMessage);
	char* fromName;
	if(deposit == 0){
			fromName = findName(recMessage, 36);
			sprintf(datMec, "%s", fromName);
			fromName = datMec;
			checkDeposit(recMessage, fromName);
			return 0;
	}
	else if(deposit ==1){
			fromName = findName(recMessage, 26);
			sprintf(datMec, "%s", fromName);
			fromName = datMec;
			givePayment(recMessage, fromName);
			return 0;
	}
	else if(deposit == 2){
			fromName = findName(recMessage, 7);
			sprintf(datMec, "%s", fromName);
			fromName = datMec;
			giveAccount(fromName);
			return 0;
	}
	else {
		return 1;
    }
	return 0;
}

/**
 * the main function
 * prints its own hostname for use
 * starts economy
 * then begins an infinite loop receiving messages to the bank, processes them accordingly
 * returns 1 on error, otherwise never returns
 */
int main() {
    char hostname[100];
    myIP(hostname);
    printf("Bank IP: %s\n",hostname);
                  
    printf("Economy started\n");
    econStart();
    printf("Economy finished\n");
    
    while(1) {
        if(receiveBankMessage()!=0) {
            printf("Bad bank message");
                return 1;
        }
        else {
            printf("Completed Transaction! \n\n");
            printf("Bank Hostname: %s\n",hostname);
        }
    }
    return 1;
}
              
              
       

