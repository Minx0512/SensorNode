/**
****************************************************************************
* Project: SensorNode
*
* ParseStrings.cpp
*
* Created on: 15 Dec 2016
*
* Author: Matthias Minx
*
* Revision: 0.2
*
*
****************************************************************************/
/** \file ParseStrings.cpp
 *
 */


#include <avr/io.h>
#include <stdint.h>
#include <avr/pgmspace.h>

#include "ParseStrings.h"
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
//#include "USART.h"



ParseStrings::ParseStrings() {
	// TODO Auto-generated constructor stub
	//u = USART();
	ClearAll();


}
ParseStrings::ParseStrings(char* string) {
	// TODO Auto-generated constructor stub

	//u = USART();
	Parse(string);



}

void ParseStrings::ClearAll(){

	cmdID = 0;
	cmdP = 0;
	sprintf(PID,"%s"," ");
	sprintf(pString,"%s"," ");



}

void ParseStrings::ParseMAC(){


	char *addToken;
	//strtoken(pString,(char*)":",addToken);

	uint8_t i = 0;
	char *tmp;
	char tmpStr[36];
	sprintf(tmpStr,pString);
	//strdup(pString);


//	u.writeString(pString);
	//u.writeString("\r\n");
	addToken = strtok_r(tmpStr, ":",&tmp);

//	u.writeString(addToken);

	addr[0] = (uint8_t)strtoul(addToken,NULL,16);

	  while ( addToken ) {
	    i++;
	    addToken = strtok_r(NULL, ":",&tmp);
	    if(addToken!=NULL && i<5){
	    	addr[i] = (uint8_t)strtoul(addToken,NULL,16);
	    }


	  }
	//  u.writeString(pString);
	//  free(tmpStr);

}

void ParseStrings::getPropertyString(char *propString){
	sprintf(propString,"%s",pString);


}
void ParseStrings::getPropertyIDAsMAC(uint8_t propString[8]){

	char *addToken;
		//strtoken(pString,(char*)":",addToken);

	uint8_t i = 0;
	char *tmp;
	char tmpStr[36];
	sprintf(tmpStr,PID);



	addToken = strtok_r(tmpStr, ":",&tmp);

	propString[0] = (uint8_t)strtoul(addToken,NULL,16);

	while ( addToken ) {
		i++;
		addToken = strtok_r(NULL, ":",&tmp);
		if(addToken!=NULL && i<8){
			propString[i] = (uint8_t)strtoul(addToken,NULL,16);
		}


	}

	//free(tmpStr);




}


uint64_t ParseStrings::getMAC(){

uint64_t value = reinterpret_cast<uint64_t>(&addr[0]) |
		reinterpret_cast<uint64_t>(&addr[1]) << 8 |
		reinterpret_cast<uint64_t>(&addr[2]) << 16 |
		reinterpret_cast<uint64_t>(&addr[3]) << 24 |
		reinterpret_cast<uint64_t>(&addr[4]) << 32;




	return (value);

}

uint8_t ParseStrings::getCmdID(){
	return (cmdID);

}
uint8_t ParseStrings::getCmdProperty(){
	return (cmdP);
}
void ParseStrings::getPropertyID(char *PIDString){
	sprintf(PIDString,"%s",PID);
}


void ParseStrings::Parse(const char* string){

	ClearAll();

	char *token;
	char* p;
	uint8_t i = 0;
	char *tmp;
	char *tmpStr = strdup(string);

//strtoken(s,delimiter,token);

//	u.writeString((char*)string);
//	u.writeString(" | ");
//	u.writeString(tmpStr);
//	u.writeString(" |t:  ");

	  token = strtok_r((char*)string, "|",&tmp);

	// u.writeString(token);
//	 u.writeString(" | ");
	  cmdID = uint8_t(strtoul(token,&p,16)/(0x10))*0x10;

	  cmdP = uint8_t(strtoul(token,&p,16))-cmdID;



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
	    		sprintf(PID,"%s",token);
	    	//	u.writeString("\t");
	    	//	u.writeString(PID);
	    	//	PID = atoi(token);
	    	}else if(i==2){
	    		sprintf(pString,"%s",token);
	    	}


	    }

	  }

	  free(tmpStr);



}


void ParseStrings::PrintVars(char* string){

	char buff[100];
	sprintf(string,"\r\n##### Config String ###### \r\n");

//	sprintf(buff,"MAC: %02X:%02X:%02X:%02X:%02X \r\n",addr[0],addr[1],addr[2],addr[3],addr[4]);strcat(string,buff);
	sprintf(buff,"pString: %s \r\n",pString); strcat(string,buff);

	sprintf(buff,"cmdID: %02X \r\n",getCmdID()); strcat(string,buff);
	sprintf(buff,"cmdP: %02X \r\n",getCmdProperty());strcat(string,buff);
	sprintf(buff,"PID: %s \r\n",PID);strcat(string,buff);
	sprintf(buff,"######################### \r\n");strcat(string,buff);



}

void ParseStrings::PrintMAC(char* string){

	char buff[100];

	sprintf(buff,"Pipe: %s MAC: %02X:%02X:%02X:%02X:%02X \r\n",PID,addr[0],addr[1],addr[2],addr[3],addr[4]);strcat(string,buff);

	sprintf(buff,"######################### \r\n");strcat(string,buff);



}
