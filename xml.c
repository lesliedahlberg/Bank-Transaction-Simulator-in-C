/*
 Created by: Leslie Dahlberg (ldg14001), Mattias Johansson (mjn14021)
 Projekt: Banksystem
 Mälardalens högskola
 DVA117 - Programmering i C, HT14
 30 September 2014
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bank.h"   //BANK SYSTEM
#include "xml.h"    //HEADER
#include "shared.h" //GLOBAL VARS

//READ XML DATA FILE
int readXML(char filePath[]){

    //INITIALIZATION
	//VARS FOR TAGS AND LINES
	char *entry;
	char *tag;
	char *line;

    //FILE
	FILE *file;
	char *currentFile;

    //LINE READER VARS
	int lineLength;
	int isEOF = 0;
	int readingLine;

    //FILE INTERPET. LOOP VARS
    int isSetFileEntry;
    int isSetRecordEntry;
    
	int infoStructIndex;
	int userStructIndex;
	int accountStructIndex;
	int transactionStructIndex;
	int requestStructIndex;
    
	//FILE READER LOOP VARS
	int tagOpen;
	int tagClosed;
	int entryClosed;
	int tagIndex;
	int entryIndex;
	int endTagOpen;
	int endTagIndex;
	int endCategoryTag;

    //DECLARATION
	//FILE INTERPET. LOOP VARS
	isSetFileEntry = 0;
	isSetRecordEntry = 0;

	infoStructIndex = 0;
	userStructIndex = 0;
	accountStructIndex = 0;
	transactionStructIndex = 0;
	requestStructIndex = 0;

	//ALLOC. STRUCTS
	info = calloc(1, sizeof(struct Info));
	user = calloc(1, sizeof(struct User));
	account = calloc(1, sizeof(struct Account));
	transaction = calloc(1, sizeof(struct Transaction));
	request = calloc(1, sizeof(struct Request));
    
    currentFile = calloc(20, sizeof(char));

	//OPEN FILE
	file = fopen(filePath, "r");
	if(file == NULL){
		return 0;
	}
    
	//LOOP THROUGH FILE LINES
	for(int row = 0; !isEOF; row++){
        
        //INIT. LOOP VARS
        tagOpen = 0;
        tagClosed = 0;
        entryClosed = 0;
        tagIndex = 0;
        entryIndex = 0;
        endTagOpen = 0;
        endTagIndex = 0;
        endCategoryTag = 0;
        
        //ALLOC ENTRY AND TAG
        entry = calloc(1, sizeof(char));
        tag = calloc(1, sizeof(char));
				
		//SAVE CURRENT LINE
		line = calloc(1, sizeof(char));
		readingLine = 1;
		lineLength = 0;
        
		for (int i = 0; readingLine; i++){
	            line[i] = fgetc(file);
	            if (line[i] == EOF) {
	                isEOF = 1;
	                readingLine = 0;
	            }else if (line[i] != '\n') {
	                lineLength++;
	            }else{
	            	readingLine = 0;
	            }
	            line = realloc(line, (2 + i)*sizeof(char));
	    }

	    //LOOP THROUGH LINE
	    for(int i = 0; i < lineLength; i++){
			
	  		//SAVE XML TAGS AND ENTRIES
	  		if(!tagOpen && !tagClosed){
	  			
	  			if(line[i] == '<' && line[i + 1] != '/'){
	  				tagOpen = 1;
	  			}else if(line[i] == '<' && line[i + 1] == '/'){
	  				endCategoryTag = 1;
	  			}
	  		}
	    	else if(tagOpen && !tagClosed){
	    		if(line[i] == '>'){
	    			tagClosed = 1;
	    		}else{
	    			tag[tagIndex] = line[i];
	    			tagIndex++;
	    			tag = realloc(tag, (1 + tagIndex)*sizeof(char));
	    			tag[tagIndex] = '\0';
	    		}
		    }else if(tagClosed){
				entry[entryIndex] = line[i];
				entryIndex++;
				entry = realloc(entry, (1 + entryIndex)*sizeof(char));
				entry[entryIndex] = '\0';
			}
	    }

	    //EVALUATE TAGS AND ENTRIES	    
    	if(!strcmp(tag, "FILE")){
    		isSetFileEntry = 1;
    	}else if(!strcmp(tag, "RECORD")){
    		isSetRecordEntry = 1;
	    }
        
	    if(entryIndex > 0){
	    	if(isSetFileEntry){
	    		if(!strcmp(tag, "FILE_NAME")){
	    			strcpy(currentFile, entry);
	    		}
	    	}
	    	if(isSetRecordEntry){
	    		//DETECT TAG TYPE AND SAVE ENTRY TO STRUCT
                
	    		if(!strcmp(currentFile, "INFO")){
	    			if(!strcmp(tag, "BANK_NAME")){
	    				strncpy(info[infoStructIndex].bank_name, entry, entryIndex);
	    			}
	    			else if(!strcmp(tag, "LAST_USER_ID")){
	    				info[infoStructIndex].last_user_id = atoi(entry);
	    			}
	    			else if(!strcmp(tag, "LAST_ACCOUNT_ID")){
	    				info[infoStructIndex].last_account_id = atoi(entry);
	    			}
                    
	    		}else if(!strcmp(currentFile, "USER")){
	    			if(!strcmp(tag, "USER_ID")){
	    				user[userStructIndex].user_id = atoi(entry);
	    			}
	    			else if(!strcmp(tag, "PERSONAL_NUMBER")){
	    				strncpy(user[userStructIndex].personal_number, entry, entryIndex);
	    			}
	    			else if(!strcmp(tag, "USERNAME")){
                                    	
                                    strncpy(user[userStructIndex].username, entry, entryIndex);
                                    
	    			}
	    			else if(!strcmp(tag, "FIRST_NAME")){
	    				strcpy(user[userStructIndex].first_name, entry);
	    			}
	    			else if(!strcmp(tag, "LAST_NAME")){
	    				strncpy(user[userStructIndex].last_name, entry, entryIndex);
	    			}
	    			else if(!strcmp(tag, "ADDRESS")){
	    				strncpy(user[userStructIndex].address, entry, entryIndex);
	    			}
	    			else if(!strcmp(tag, "USER_TYPE")){
	    				strncpy(user[userStructIndex].user_type, entry, entryIndex);
	    			}
	    			else if(!strcmp(tag, "PASSWORD")){
	    				strncpy(user[userStructIndex].password, entry, entryIndex);
	    			}
	    			else if(!strcmp(tag, "ACTIVE")){
	    				user[userStructIndex].active = (short) atoi(entry);
	    			}
                    
	    		}else if(!strcmp(currentFile, "ACCOUNT")){
	    			if(!strcmp(tag, "ACCOUNT_ID")){
	    				account[accountStructIndex].account_id = (short) atoi(entry);
	    			}
	    			else if(!strcmp(tag, "ACCOUNT_NUMBER")){
	    				account[accountStructIndex].account_number = (int) atoi(entry);
	    			}
	    			else if(!strcmp(tag, "BALANCE")){
	    				account[accountStructIndex].balance = (int) atoi(entry);
	    			}
	    			else if(!strcmp(tag, "USER_ID")){
	    				account[accountStructIndex].user_id = atoi(entry);
	    			}
	    			else if(!strcmp(tag, "ACTIVE")){
	    				account[accountStructIndex].active = (short) atoi(entry);
	    			}
                    
	    		}else if(!strcmp(currentFile, "TRANSACTION")){
	    			if(!strcmp(tag, "FROM")){
	    				transaction[transactionStructIndex].from = (short) atoi(entry);
	    			}
	    			else if(!strcmp(tag, "TO")){
	    				transaction[transactionStructIndex].to = (short) atoi(entry);
	    			}
	    			else if(!strcmp(tag, "DATE")){
	    				strncpy(transaction[transactionStructIndex].date, entry, entryIndex);
	    			}
	    			else if(!strcmp(tag, "AMOUNT")){
	    				transaction[transactionStructIndex].amount = (int) atoi(entry);
	    			}
	    			else if(!strcmp(tag, "USER_ID")){
	    				transaction[transactionStructIndex].user_id = (int) atoi(entry);
	    			}
	    			else if(!strcmp(tag, "ACTIVE")){
	    				transaction[transactionStructIndex].active = (short) atoi(entry);
	    			}
                    
	    		}else if(!strcmp(currentFile, "REQUEST")){
	    			if(!strcmp(tag, "USER_ID")){
	    				request[requestStructIndex].user_id = (short) atoi(entry);
	    			}
	    			else if(!strcmp(tag, "ACTION")){
	    				strncpy(request[requestStructIndex].action, entry, entryIndex);
	    			}
	    			else if(!strcmp(tag, "DATE")){
	    				strncpy(request[requestStructIndex].date, entry, entryIndex);
	    			}
	    			else if(!strcmp(tag, "ACTIVE")){
	    				request[requestStructIndex].active = (short) atoi(entry);
	    			}
	    			else if(!strcmp(tag, "ACCOUNT_ID")){
	    				request[requestStructIndex].account_id = (short) atoi(entry);
	    			}
                    
	    		}
	    	}

	    }else if(endCategoryTag){
	    	
            //CLOSE TAG AND ALLOC MORE SPACE FOR STRUCT
	    	if(isSetRecordEntry){
                
                //RESET RECORD TAG
	    		isSetRecordEntry = 0;
	    		
                if(!strcmp(currentFile, "USER")){
	    			userStructIndex++;
	    			user = realloc(user, (1 + userStructIndex)*sizeof(struct User));
	    		
                }else if(!strcmp(currentFile, "ACCOUNT")){
	    			accountStructIndex++;
	    			account = realloc(account, (1 + accountStructIndex)*sizeof(struct Account));
	    		
                }else if(!strcmp(currentFile, "TRANSACTION")){
	    			transactionStructIndex++;
	    			transaction = realloc(transaction, (1 + transactionStructIndex)*sizeof(struct Transaction));
	    		
                }else if(!strcmp(currentFile, "REQUEST")){
	    			requestStructIndex++;
	    			request = realloc(request, (1 + requestStructIndex)*sizeof(struct Request));
	    		}

	    	}else{
	    		//RESET FILE TAG
	    		isSetFileEntry = 0;
	    		currentFile[0] = '\0';
	    	}
	    }
	   
        //FREE TAGS IN ROWS LOOP
	   	free(line);
		free(tag);
		free(entry);

	}
    
    //FREE PSEUDO-GLOBAL VARS
    free(currentFile);
	
    //SAVE STRUCT ITEM COUNT
	infoCount = infoStructIndex;
	userCount = userStructIndex;
	accountCount = accountStructIndex;
	transactionCount = transactionStructIndex;
	requestCount = requestStructIndex;

    //CLOSE FILE
    fclose(file);
    
    //RETURN
	return 1;
}

//WRITE DATA TO XML FILE
int writeToXML(char filePath[]){

    //OPEN FILE
	FILE *file;
	file = fopen(filePath, "w");
	if(file == NULL){
		return 0;
	}

	//WRITE INFO
	fputs("<FILE>\n", file);
	fputs("	<FILE_NAME>INFO\n", file);

	fputs("	<RECORD>\n", file);

	fputs("		<BANK_NAME>", file);
	fputs(info[0].bank_name, file);
	fputs("\n", file);

	fputs("		<LAST_USER_ID>", file);
	fprintf(file, "%d", info[0].last_user_id);
	fputs("\n", file);

	fputs("		<LAST_ACCOUNT_ID>", file);
	fprintf(file, "%d", info[0].last_account_id);
	fputs("\n", file);

	fputs("	</RECORD>\n", file);
	
	fputs("</FILE>\n", file);


	//WRITE USER
	fputs("<FILE>\n", file);
	fputs("	<FILE_NAME>USER\n", file);
	
	for(int i = 0; i < userCount; i++){
        if(user[i].active == 1){

			fputs("	<RECORD>\n", file);

			fputs("		<USER_ID>", file);
			fprintf(file, "%d", user[i].user_id);
			fputs("\n", file);

			fputs("		<PERSONAL_NUMBER>", file);
			fputs(user[i].personal_number, file);
			fputs("\n", file);

			fputs("		<USERNAME>", file);
			fputs(user[i].username, file);
			fputs("\n", file);

			fputs("		<FIRST_NAME>", file);
			fputs(user[i].first_name, file);
			fputs("\n", file);

			fputs("		<LAST_NAME>", file);
			fputs(user[i].last_name, file);
			fputs("\n", file);

			fputs("		<ADDRESS>", file);
			fputs(user[i].address, file);
			fputs("\n", file);

			fputs("		<USER_TYPE>", file);
			fputs(user[i].user_type, file);
			fputs("\n", file);

			fputs("		<PASSWORD>", file);
			fputs(user[i].password, file);
			fputs("\n", file);

			fputs("		<ACTIVE>1\n", file);

			fputs("	</RECORD>\n", file);
        }
	}

	fputs("</FILE>\n", file);


	//WRITE ACCOUNT
	fputs("<FILE>\n", file);
	fputs(" <FILE_NAME>ACCOUNT\n", file);
	
	for(int i = 0; i < accountCount; i++){
        if(account[i].active == 1){
		
			fputs(" <RECORD>\n", file);

			fputs("		<ACCOUNT_ID>", file);
			fprintf(file, "%hd", account[i].account_id);
			fputs("\n", file);

			fputs("		<ACCOUNT_NUMBER>", file);
			fprintf(file, "%d", account[i].account_number);
			fputs("\n", file);

			fputs("		<BALANCE>", file);
			fprintf(file, "%d", account[i].balance);
			fputs("\n", file);

			fputs("		<USER_ID>", file);
			fprintf(file, "%d", account[i].user_id);
			fputs("\n", file);

			fputs("		<ACTIVE>1\n", file);

			fputs("	</RECORD>\n", file);
        }
	}

	fputs("</FILE>\n", file);


	//WRITE TRANSACTION
	fputs("<FILE>\n", file);
	fputs("	<FILE_NAME>TRANSACTION\n", file);
	
	for(int i = 0; i < transactionCount; i++){
        if(transaction[i].active == 1){
		fputs("	<RECORD>\n", file);

		fputs("		<FROM>", file);
		fprintf(file, "%hd", transaction[i].from);
		fputs("\n", file);

		fputs("		<TO>", file);
		fprintf(file, "%hd", transaction[i].to);
		fputs("\n", file);

		fputs("		<DATE>", file);
		fputs(transaction[i].date, file);
		fputs("\n", file);

		fputs("		<AMOUNT>", file);
		fprintf(file, "%d", transaction[i].amount);
		fputs("\n", file);

		fputs("		<USER_ID>", file);
		fprintf(file, "%d", transaction[i].user_id);
		fputs("\n", file);

		fputs("		<ACTIVE>1\n", file);

		fputs("	</RECORD>\n", file);
        }
	}

	fputs("</FILE>\n", file);


	//WRITE REQUEST
	fputs("<FILE>\n", file);
	fputs("	<FILE_NAME>REQUEST\n", file);
	
	for(int i = 0; i < requestCount; i++){
		if(user[i].active == 1){
			fputs("		<RECORD>\n", file);

			fputs("		<USER_ID>", file);
			fprintf(file, "%d", request[i].user_id);
			fputs("\n", file);

			fputs("		<ACCOUNT_ID>", file);
			fprintf(file, "%d", request[i].account_id);
			fputs("\n", file);

			fputs("		<ACTION>", file);
			fputs(request[i].action, file);
			fputs("\n", file);

			fputs("		<DATE>", file);
			fputs(request[i].date, file);
			fputs("\n", file);

			fputs("		<ACTIVE>1\n", file);

			fputs("	</RECORD>\n", file);
        }
	}

	fputs("</FILE>\n", file);
    
    //CLOSE FILE
    fclose(file);

    //RETURN
	return 1;
}

