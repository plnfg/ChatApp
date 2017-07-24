/*
 * main.c
 *
 *  Created on: Jul 20, 2017
 *      Author: plnfg
 */

#include <unistd.h>
#include "packets.h"
#include "chat.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char packet_id;

int main(){
	//send_dbq();

	packet_id=0;
	char *msg;
	msg=malloc(100);
	memset(msg,0,100);
	scanf("%s",msg);
	send_message("ugur",msg,packet_id);
	// send_duq("ugur","ilter");
	//send_hello_resp("ugur","ilter");



	return 0;
}
