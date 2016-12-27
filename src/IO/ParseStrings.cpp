/*
 * ParseStrings.cpp
 *
 *  Created on: 15 Dec 2016
 *      Author: matthias
 */


#include <avr/io.h>

#include <avr/pgmspace.h>

#include "ParseStrings.h"
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include "USART.h"



ParseStrings::ParseStrings() {
	// TODO Auto-generated constructor stub
	u = USART();
	cmdID=0;
	cmdP =0;
	PID=0;
	sprintf(pString,"%s"," ");


}
ParseStrings::ParseStrings(char* string) {
	// TODO Auto-generated constructor stub

	u = USART();
	Parse(string);



}

void ParseStrings::ClearAll(){

	cmdID = 0;
	cmdP = 0;
	PID = 0;
	sprintf(pString," ");



}

void ParseStrings::ParseMAC(){


	char *addToken;
	//strtoken(pString,(char*)":",addToken);

	uint8_t i = 0;
	char *tmp;
	char *tmpStr = strdup(pString);



	addToken = strtok_r(tmpStr, ":",&tmp);

	addr[0] = (uint8_t)strtoul(addToken,NULL,16);

	  while ( addToken ) {
	    i++;
	    addToken = strtok_r(NULL, ":",&tmp);
	    if(addToken!=NULL && i<5){
	    	addr[i] = (uint8_t)strtoul(addToken,NULL,16);
	    }


	  }

	  free(tmpStr);

}

void ParseStrings::getPropertyString(char *propString){
	sprintf(propString,"%s",pString);


}
uint8_t* ParseStrings::getMAC(){

return addr;

}

uint8_t ParseStrings::getMAC(uint8_t* a){

	for(uint8_t i=0;i<5;i++){
		a[i]=addr[i];
	}

	return 0;

}

uint8_t ParseStrings::getCmdID(){
	return cmdID;

}
uint8_t ParseStrings::getCmdProperty(){
	return cmdP;
}
uint8_t ParseStrings::getPropertyID(){
	return PID;
}


void ParseStrings::Parse(const char* string){

	ClearAll();

	char *token;

	uint8_t i = 0;
	char *tmp;
	char *tmpStr = strdup(string);

//strtoken(s,delimiter,token);

//	u.writeString((char*)string);
//	u.writeString(" | ");
///	u.writeString(tmpStr);
//	u.writeString(" |t:  ");

	  token = strtok_r((char*)string, "|",&tmp);

//	 u.writeString(token);
//	 u.writeString(" | ");
	  cmdID = atoi(token)/10;

	  cmdP = atoi(token)-(cmdID*10);



	//  char t[50];
	//  sprintf(t," ");
	//  sprintf(t," cmdID: %i | cmdP: %i \r\n",cmdID,cmdP);
	//  u.writeString(t);


	  while ( token ) {
		i++;
	    token = strtok_r(NULL, "|",&tmp);
//	    u.writeString(token);
//	 u.writeString(" . ");
	    if(token!=NULL){
	    	if(i==1){
	    		PID = atoi(token);
	    	}else if(i==2){
	    		sprintf(pString,"%s",token);
	    	}


	    }

	  }

	  free(tmpStr);



}


void ParseStrings::PrintVars(char* string){

	char buff[100];
	sprintf(string,"##### Config String ###### \r\n");

//	sprintf(buff,"MAC: %02X:%02X:%02X:%02X:%02X \r\n",addr[0],addr[1],addr[2],addr[3],addr[4]);strcat(string,buff);
	sprintf(buff,"pString: %s \r\n",pString); strcat(string,buff);

	sprintf(buff,"cmdID: %i \r\n",getCmdID()); strcat(string,buff);
	sprintf(buff,"cmdP: %i \r\n",getCmdProperty());strcat(string,buff);
	sprintf(buff,"PID: %i \r\n",getPropertyID());strcat(string,buff);
	sprintf(buff,"######################### \r\n");strcat(string,buff);



}

void ParseStrings::PrintMAC(char* string){

	char buff[100];

	sprintf(buff,"Pipe: %i MAC: %02X:%02X:%02X:%02X:%02X \r\n",getPropertyID(),addr[0],addr[1],addr[2],addr[3],addr[4]);strcat(string,buff);

	sprintf(buff,"######################### \r\n");strcat(string,buff);



}
