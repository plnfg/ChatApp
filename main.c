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
#include <pthread.h>




int main(){
	pthread_t receiver;

	pthread_create(&receiver, NULL,receive, NULL);

	char *msg,*target_name;
	char packet_id=0;

	target_name=malloc(10);
	memset(target_name,0,10);
	printf("Target name: \n");
	scanf("%s",target_name);
	printf("From Pelin: ");

	while(1){
		msg=malloc(100);
		memset(msg,0,100);
		fgets(msg,100,stdin);
		send_message(target_name,msg,packet_id);
		free(msg);
	}

	free(target_name);



//	send_dbq();
	// send_duq("ugur","ilter");
	//send_hello_resp("ugur","ilter");



	return 0;
}
