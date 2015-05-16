#include "bank.h"

//receives eCent and checks for integrity, replies to analyst if good, deposit else discard
int checkDeposit(char* recMessage, char* fromName){
	int retunType; //Will indicate success or failure
	
	int centLen = strlen(recMessage) - 47;//might be 45 or 46. I'll fiddle around and fix this. Might end up making it pad out to the full 10.
//	7 for transaction type, 20 for sender's bank ID, 20 is for the cent owner's bank ID
	char ownID[20];
	char centS[11];
	strcpy(cID,recMessage+27, 20);
	strcpy(cID, recMessage+47, centLen); 
	centS[10] = '\0';
	int centID = atoi(centS);
	char* sendMessage; // not initialised yet
	
	FILE *centList = fopen(centBank, "r+");
	eCent cent;
	fseek(centList, centID*sizeof(eCent), SEEK_SET);
	fread(&cent, sizeof(eCent),1, centList);
	if(strcmp(cent.owner, ownID)==0){ //handled the right code
		fclose(centList); //won't need it again for this transaction
		FILE *centBanked = fopen(centdepot, "r+"); //storing the eCent with the rest of the bank's cache.
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
	
        
        SSL_BA(sendMessage); // still going to have to do these
        
        sendData(BANKPORT, fromName, sendMessage);
        
        return returnType;
    }

}

int givePayment(char[] fromName) {
    char* sendMessage;
    FILE *centBanked = fopen(centdepot, "r+");
	eCent cent;
	
	int a;
	fread(&a, sizeof(int),1, centBanked);
	if(a == 0){ // Game Over. Insert coin/s to continue.
		fclose(centBanked);
		char* failstring = noDosh(); //gotta code something here;
		encrypt_BC(failstring);
		SSL_BC(failstring);
		sendData(BankPort, fromName, failstring);
		return 0; //indicates failure, ends function;
	}
	else if(a>10){ //more than enough cents to go around
		fseek(centBanked, 0, SEEK_SET);// Back to the start to edit the number of remaining cents
		int b = a - 10;
		fwrite(&b, sizeof(int), 1, centBanked); // Update
		fseek(centBanked, b*(sizeof(eCent)), SEEK_CUR); //jumpt to almost end of list
		a=10; //sending default amount of 10 for now. Will add something to allow for the collector to specify a number of eCents to obtain
	}
	else{ // either just enough, or not quite enough
		fseek(centBanked, 0, SEEK_SET);
		int b = 0; //'t weren't empty before, but it sure is now.
		fwrite(&b, sizeof(int),1,centBanked); //update
	}
	char cents[a*11 +1]; // compilation of the eCent IDs.
	FILE *centList = fopen(centBank, "r+"); //open the log of who owns which eCents
	
	for(int i=0; i<a; i++) ){
		fread(&cent, sizeof(eCent), 1, centBanked); //get ecent from the bank's cache
		cent.owner = fromName; //reallocating owner
		fseek(centList, cent.identifier*(sizeof(eCent)), SEEK_SET); //looking up eCent's entry in owner table
		fwrite(cent, sizeof(eCent), 1, centList); //update owner table
		char tempCent[12]; //temp string for strncat
		sprintf(tempCent, "%d\n", cent.identifier); //add eCent number to string
		strncat(cents,tempCent);
	}
	fclose(centBanked);
	fclose(centList);
	sendMessage = &cents;
	
	//REALLY IMPORTANT THING TO NOTE HERE
	//This is assuming that the collector will send his bank number to the analyst rather than the coin as a whole.
	//can easily send bank detail with each coin, but since it's going to be the same every time, we'd be doubling the data sent, and it'd all be superfluous.
    
    encrypt_BC(sendMessage); // gotta get some encryption coded
    SSL_BC(sendMessage);
    
    sendData(BANKPORT, fromName, sendMessage);
    
}

void econStart(){
	FILE *centBanked = fopen(centdepot, "W");
	FILE *centList = fopen(centBank, "W");
	int a = 10000;
	fwrite(&a, sizeof(int),1, centBanked);
	for(int i =0; i < 10000; i++){ //subject to change or alteration if I get a good idea
		eCent cent = {
			.identifier = i,
			.owner = "000"; // eventually code for reserved number for bank
		}
		fwrite(&cent, sizeof(eCent), 1, centBanked);
		fwrite(&cent, sizeof(eCent),1, centList);
	}
	fclose(centBanked);
	fclose(centList);
		
}

int deParse(char* mesg){
	char parse[8];
	strncpy(parse, mesg, 7);
	parse[7] = '\0';
	char an[] = "deposit";
	char col[] ="collect";
	if(strcmp(parse, an) == 0){ return 0;}
	else if(strcmp(parse, col)== 0){ return 1;}
	else{return -1;}
}
char findName(char* mesg){ //I'll fix this up soon, just want to poke about with some ideas first
	char name[20];
	strncpy(name, mesg+7,20); //maybe 19, depends if the '\0' is kept in the string 
	name[19] = '\0';
	return name;
}
int receiveBankMessage() {
    char* recMessage;
    receiveData(BANKPORT, recMessage);
    
    decrypt(recMessage);
    
	//transaction type
    int deposit = deParse(recMessage);
    
    //the incoming message addressName
    char fromName[20] = findName(recMessage);
	
	
    switch(deposit){
		case 0:
			checkDeposit(recMessage, fromName);
		case 1:
			
			givePayment(fromName);
		default:
			//send some sort of error to the sender's IP
			//because the transaction type is not recognised.
	}
}
