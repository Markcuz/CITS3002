‪#‎include‬"bank.h"

/**
 * most of the send message calls are //'d out because I'm working on manual input
 * debugging to iron out the last couple of creases.
 */
 

char banksID[20] = "0000000000000000000";
void econStart(){
	FILE *centBanked = fopen("centdepot", "w+");
	FILE *centList = fopen("centBank", "w+");
	FILE *lastAcc = fopen("bankAcc", "w+");
	char banknum[20];
	sprintf(banknum,"%019d", 0);
	banknum[19] = '\0';
	fwrite(&banknum, 20, 1, lastAcc);
	int a = 10000;
	fwrite(&a, sizeof(int),1, centBanked);
	for(int i =0; i < 10000; i++){ //subject to change or alteration if I get a good idea
		eCent cent;
		cent.identifier = i;
		strcpy(cent.owner, banksID); // eventually code for reserved number for bank
		fwrite(&cent, sizeof(eCent), 1, centBanked);
		fwrite(&cent, sizeof(eCent),1, centList);
	}
	fclose(centBanked);
	fclose(centList);
	fclose(lastAcc);
}
//receives eCent and checks for integrity, replies to analyst if good, deposit else discard
int checkDeposit(char* recMessage, char* fromName){
	int returnType; //Will indicate success or failure
	char oldID[20];
	char centS[10];
	strncpy(oldID,recMessage+7, 20);
	strncpy(centS, recMessage+27, 10);
	int centID = atoi(centS);
	char* sendMessage; // not initialised yet
	FILE *centList;
	centList = fopen("centBank", "r+");
	if(centList == NULL){
		econStart();
		centList = fopen("centBank", "r+");
	}
	eCent cent;
	fseek(centList, centID*sizeof(eCent), SEEK_SET);
	fread(&cent, sizeof(eCent),1, centList);
	if((strcmp(cent.owner, oldID)==0) && (strcmp(cent.owner, banksID)!=0)){ //handled the right code
		strcpy(cent.owner, banksID);
		fseek(centList, centID*sizeof(eCent), SEEK_SET);
		fwrite(&cent, sizeof(eCent), 1, centList);
		fclose(centList); //won't need it again for this transaction
		FILE *centBanked = fopen("centdepot", "r+"); //storing the eCent with the rest of the bank's cache.
		int stash; //number of stored eCents
		fread(&stash, sizeof(int), 1, centBanked);
		fseek(centBanked, stash*sizeof(eCent), SEEK_CUR); //to the end of the queue of stored eCents
		fwrite(&cent, sizeof(eCent), 1, centBanked); //putting at end of queue
		fseek(centBanked, 0, SEEK_SET);//back to storage number
		stash++;
		fwrite(&stash, sizeof(int), 1, centBanked); //updating number of stored eCents
		fclose(centBanked);
		char accept[] = "Well done you"; // look, it's like 23:00, I'm about to call it for the night. bear with me.
		sendMessage = accept; //something something ack message;
		returnType = 0;
	}
	else{
		char decline[] = "You dun goofed, son.";
		sendMessage = decline;
		returnType =1;
	}
/** SSL_BA(sendMessage); // still going to have to do these
sendData(BANKPORT, fromName, sendMessage);
*/
	printf("%s\n", sendMessage);
	return returnType;
}
int givePayment(char* recMessage, char* fromName) {
	FILE *centBanked = fopen("centdepot", "r+");
	if(centBanked == NULL){
		econStart();
		centBanked = fopen("centdepot","r+");
	}
	int a;
	fread(&a, sizeof(int),1, centBanked);
	if(a == 0){ // Game Over. Insert coin/s to continue.
		fclose(centBanked);
//char* failstring = noDosh(); //gotta code something here;
//encrypt_BC(failstring);
//SSL_BC(failstring);
//sendData(BankPort, fromName, failstring);
		return 0; //indicates failure, ends function;
	}
	else if(a>10){ //more than enough cents to go around
		fseek(centBanked, 0, SEEK_SET);// Back to the start to edit the number of remaining cents
		int b = a - 10;
		fwrite(&b, sizeof(int), 1, centBanked); // Update
		fseek(centBanked, b*(sizeof(eCent)), SEEK_CUR); //jumpt to almost end of list
		a=10; //sending default amount of 10 for now. 
//Will add something to allow for the collector to specify a number of eCents to obtain
	}	
	else{ // either just enough, or not quite enough
		fseek(centBanked, 0, SEEK_SET);
		int b = 0; //'t weren't empty before, but it sure is now.
		fwrite(&b, sizeof(int),1,centBanked); //update
	}
	eCent cent;
	char ownID[20];
	strncpy(ownID,recMessage+7, 19);
	ownID[19] = '\0';
	printf("in give\n%s\n", ownID);
	char* sendMessage;
	char cents[a*11 +1]; // compilation of the eCent IDs.
	FILE *centList = fopen("centBank", "r+"); //open the log of who owns which eCents
	for(int i=0; i<a; i++){
		fread(&cent, sizeof(eCent), 1, centBanked); //get ecent from the bank's cache
		strcpy(cent.owner, ownID); //reallocating owner
		fseek(centList, cent.identifier*(sizeof(eCent)), SEEK_SET); //looking up eCent's entry in owner table
		fwrite(&cent, sizeof(eCent), 1, centList); //update owner table
		char tempCent[12]; //temp string for strncat
		sprintf(tempCent, "%010d\n", cent.identifier); //add eCent number to string
		tempCent[11] = '\0';
		strncat(cents,tempCent, 12);
	}
	fclose(centBanked);
	fclose(centList);
	sendMessage = cents;
	printf("%s\n", sendMessage);
	printf("%s\n", fromName);
	//REALLY IMPORTANT THING TO NOTE HERE
	//This is assuming that the collector will send his bank number to the analyst rather than the coin as a whole.
	//can easily send bank detail with each coin, but since it's going to be the same every time, we'd be doubling the data sent, and it'd all be superfluous.
	//encrypt_BC(sendMessage); // gotta get some encryption coded
	//SSL_BC(sendMessage);
	//sendData(BANKPORT, fromName, sendMessage);
	return 0;
}
int giveAccount(char* fromName){
	FILE *lastAcc = fopen("bankAcc", "r+");
	if(lastAcc == NULL){
		econStart();
		lastAcc = fopen("bankAcc", "r+");
	}
	char nID[20];
	fgets(nID,20, lastAcc);
	int acnum = atoi(nID);
	acnum++;
	sprintf(nID, "%019d", acnum);
	fseek(lastAcc,0, SEEK_SET);
	fwrite(&nID, 20, 1, lastAcc);
	char* sendMessage = nID;
	printf("%s\n", fromName);
	printf("%s\n", sendMessage);
	return 	0;
//encrypt_BC(sendMessage); // gotta get some encryption coded
//SSL_BC(sendMessage);
//sendData(yaddayadda);
}
int deParse(char* mesg){
	char parse[8];
	strncpy(parse, mesg, 7);
	parse[7] = '\0';
	char an[] = "deposit";
	char col[] ="collect";
	char acc[] ="initial";
	if(strcmp(parse, an) == 0){ return 0;}
	else if(strcmp(parse, col)== 0){ return 1;}
	else if(strcmp(parse, acc)==0){return 2;}
	else{return 3;}
}
char *findName(char* recMessage, int cas){ //I'll fix this up soon, just want to poke about with some ideas first
	char* name;
	char PULSR[10000];
	int lngth = sprintf(PULSR, "%s", recMessage);
	if(lngth == 10000){
		name = NULL;
	return name;
	}// unreasonably huge server name. Possibly a DOS attack. disregard message.
	else{
		char serName[lngth - cas];
		//strncpy(serName, PULSR+cas, lngth-cas);
		strcpy(serName,PULSR +cas);
		name = serName;
		printf("%s\n", serName);
		return name;
	}
}
int receiveBankMessage(char* recMessage) {
//receiveData(BANKPORT, recMessage);
//decrypt(recMessage);
//transaction type
	int deposit = deParse(recMessage);
//the incoming message addressName
	char* fromName;
	switch(deposit){
		case 0:
			fromName = findName(recMessage, 37);
			checkDeposit(recMessage, fromName);
		case 1:
			fromName = findName(recMessage, 27);	
			printf("In recmes \n %s\n", fromName);
			givePayment(recMessage, fromName);
		case 2:
			fromName = findName(recMessage, 7);
			giveAccount(fromName);
		case 3:
			fromName = banksID;
			return 0;
//send some sort of error to the sender's IP
//because the transaction type is not recognised.
// or just ignore, and wait for the resend
	}
	return 0;
}
