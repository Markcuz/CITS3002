#include"bank.h"
char banksID[20] = "0000000000000000000";
void econStart(){
	FILE *centBanked = fopen("centdepot", "w+");
	FILE *centList = fopen("centBank", "w+");
	FILE *lastAcc = fopen("bankAcc", "w+");
	int banknum =0;
	fwrite(&banknum, sizeof(int), 1, lastAcc);
	int a = 10000;
	fwrite(&a, sizeof(int),1, centBanked);
	for(int i =0; i < a; i++){ //subject to change or alteration if I get a good idea
		eCent cent;
		cent.identifier = i;
		strcpy(cent.owner, banksID); //reserved bank ID
	
		fwrite(&cent, sizeof(eCent), 1, centBanked);
		fwrite(&cent, sizeof(eCent),1, centList);
			}
	fclose(centBanked);
	fclose(centList);
	fclose(lastAcc);
}
//receives eCent and checks for integrity, replies to analyst if good, deposit else discard
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
	eCent cent;
	fseek(centList, centID*sizeof(eCent), SEEK_SET);
	fread(&cent, sizeof(eCent),1, centList);
	printf("%s\n",cent.owner);
	printf("%d\n",cent.identifier);

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
		char accept[] = "Well done you";
		sendMessage = accept; //something something ack message;
	}
	else{
		char decline[] = "You dun goofed, son.";//something something Nack message;
		sendMessage = decline;
	}
//	SSL_BA(sendMessage); // still going to have to do these
//	sendData(BANKPORT, fromName, sendMessage);
	printf("%s\n", sendMessage);
	return 0;
}




int givePayment(char* recMessage, char* fromName) {
	FILE *centBanked = fopen("centdepot", "r+");
	if(centBanked == NULL){
		econStart();
		centBanked = fopen("centdepot","r+");
	}
	char ownID[20];
	strncpy(ownID,recMessage+7, 19);
	ownID[19] = '\0';
	printf("%s\n", ownID);
	int a =0;
	int zz =0;
	FILE *lastAcc = fopen("bankAcc", "r+");
	int lAc;
	fread(&lAc,sizeof(int), 1 ,lastAcc);
	int b = atoi(ownID);
	int c = atoi(banksID);
	if(lAc>= b && c < b){
		zz =1;
		printf("placeholder%d\n",zz);
	}
	else{return 0;}
	fread(&a, sizeof(int), 1, centBanked);
	if(a == 0){ // Game Over. Insert coin/s to continue.
		fclose(centBanked);
		char noDosh[] = "No soup for you!";
		char* failstring = noDosh; //gotta code something here;
		printf("%s\n", failstring);
//		encrypt_BC(failstring);
//		SSL_BC(failstring);
//		sendData(BANKPORT, fromName, failstring);
		//alt = issue new(similar to econ start, but issues a new 10000 ecents
		return 0;
	}
	else if(a>10){ //more than enough cents to go around
		fseek(centBanked, 0, SEEK_SET);// Back to the start to edit the number of remaining cents
		int bd = a - 10;
		fwrite(&bd, sizeof(int), 1, centBanked); // Update
		fseek(centBanked, bd*(sizeof(eCent)), SEEK_CUR); //jumpt to almost end of list
		a=10; //sending default amount of 10 for now. 
//Will add something to allow for the collector to specify a number of eCents to obtain
	}	
	else{ // either just enough, or not quite enough
		fseek(centBanked, 0, SEEK_SET);
		int bd = 0; //'t weren't empty before, but it sure is now.
		fwrite(&bd, sizeof(int),1,centBanked); //update
	}
	printf("ISA%d\n",a);
	eCent cent;
	char* sendMessage;
	char cents[a*11 +1]; // compilation of the eCent IDs.
	FILE *centList = fopen("centBank", "r+"); //open the log of who owns which eCents
	for(int i=0; i<a; i++){
		fread(&cent, sizeof(eCent), 1, centBanked); //get ecent from the bank's cache
		printf("%s\n%d\n", cent.owner, cent.identifier);
		strcpy(cent.owner, ownID); //reallocating owner
		fseek(centList, (cent.identifier*(sizeof(eCent))), SEEK_SET); //looking up eCent's entry in owner table
		
		fwrite(&cent, sizeof(eCent), 1, centList); //update owner table
		char tempCent[12]; //temp string for strncat
		sprintf(tempCent, "%010d\n", cent.identifier); //add eCent number to string
		tempCent[11] = '\0';
		strncat(cents,tempCent, 11);
	}
	cents[-1] = '\0';
	fclose(centBanked);
	fclose(centList);
	sendMessage = cents;
//	encrypt_BC(sendMessage); // gotta get some encryption coded
//	SSL_BC(sendMessage);
//	sendData(BANKPORT, fromName, sendMessage);
	printf("%s\ncent\n", sendMessage);
	return 0;
}

int giveAccount(char* fromName){
	FILE *lastAcc = fopen("bankAcc", "r+");
	if(lastAcc == NULL){
		econStart();
		lastAcc = fopen("bankAcc", "r+");
	}
	int nID =0;
	fread(&nID,sizeof(int), 1, lastAcc);
	nID+=1;
	char acnum[20];
	sprintf(acnum, "%019d", nID);
	printf("lastAcc%d\n", nID);
	fseek(lastAcc,0, SEEK_SET);
	fwrite(&nID, sizeof(int), 1, lastAcc);
	
	fseek(lastAcc,0, SEEK_SET);
	fread(&nID,sizeof(int), 1, lastAcc);
	printf("lastacc%d\n", nID);
	char* sendMessage = acnum;
//	encrypt_BC(sendMessage); // gotta get some encryption coded
//	SSL_BC(sendMessage);
//	sendData(BANKPORT,fromName, sendMessage);
	printf("%s\n", sendMessage);
	return 0;
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
	
	int lngth = strlen(recMessage)+1;
	char serName[lngth - cas];
	strcpy(serName,recMessage +cas);
	serName[-1] = '\0';
	name = serName;
	return name;
	
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
			fromName = findName(recMessage, 36);
			checkDeposit(recMessage, fromName);
			break;
			
		case 1:
			fromName = findName(recMessage, 26);
			printf("%s\n", fromName);	
			givePayment(recMessage, fromName);
			break;
			
		case 2:
			fromName = findName(recMessage, 7);
			giveAccount(fromName);
			break;
			
		case 3:
			fromName = banksID;
			break;
//send some sort of error to the sender's IP
//because the transaction type is not recognised.
// or just ignore, and wait for the resend
	}
	return 0;
}

