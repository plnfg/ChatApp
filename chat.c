#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "packets.h"
#include "chat.h"

#define BUF_SIZ		1024
#define INTERFACE  "enp14s0"
#define BCAST_MAC {0xff,0xff,0xff,0xff,0xff,0xff}
#define MAC_WAITING_COUNT 2
#define BRCAST_WAITING_TIME 1
#define BRCAST_WAITING_COUNT 3


static const char filename[] = "address.txt";
static FILE *fp;

typedef enum {
   QUERY_BROADCAST,
   QUERY_UNICAST,
   HELLO_RESPONSE,
   CHAT,
   CHAT_ACK,
   EXITING,
} en_pack_type;

void send_all(char *dest_mac,char* packet,char type,char* interface){

	int sockfd;
	struct ifreq if_idx;
	struct ifreq if_mac;
	int tx_len = 0;
	char sendbuf[BUF_SIZ];
	struct ether_header *eh = (struct ether_header *) sendbuf;
	struct iphdr *iph = (struct iphdr *) (sendbuf + sizeof(struct ether_header));
	struct sockaddr_ll socket_address;
	char ifName[IFNAMSIZ];


	strcpy(ifName, interface);

	/* Open RAW socket to send on */
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
	    perror("socket");
	}

	/* Get the index of the interface to send on */
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
	    perror("SIOCGIFINDEX");
	/* Get the MAC address of the interface to send on */
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
	    perror("SIOCGIFHWADDR");

	/* Construct the Ethernet header */
	memset(sendbuf, 0, BUF_SIZ);
	/* Ethernet header */
	eh->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
	eh->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
	eh->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
	eh->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
	eh->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
	eh->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
	eh->ether_dhost[0] = dest_mac[0];
	eh->ether_dhost[1] = dest_mac[1];
	eh->ether_dhost[2] = dest_mac[2];
	eh->ether_dhost[3] = dest_mac[3];
	eh->ether_dhost[4] = dest_mac[4];
	eh->ether_dhost[5] = dest_mac[5];
	/* Ethertype field */
	eh->ether_type = htons(0x1234);
	tx_len += sizeof(struct ether_header);

	disc_broadcast_q dbq;
	disc_unicast_q duq;
	hello hello_resp;
	chat my_msg;
	chat_ack ack;
	size_t size;

	switch(type){
		case(QUERY_BROADCAST):
			dbq=(disc_broadcast_q)packet;
			size=sizeof(Disc_Broadcast_q);
			break;
		case(QUERY_UNICAST):
			duq=(disc_unicast_q)packet;
			size=sizeof(Disc_Unicast_q);
			break;
		case(HELLO_RESPONSE):
			hello_resp=(hello)packet;
			size=sizeof(Hello_Response);
			break;
		case(CHAT):
			my_msg=(chat)packet;
			size=sizeof(Chat_t);
		case(CHAT_ACK):
			ack=(chat_ack)packet;
			size=sizeof(Chat_Ack_t);
	}

	int i;
	for(i=0;i<size;i++){
		sendbuf[tx_len++] = packet[i];
	}


	/* Index of the network device */
	socket_address.sll_ifindex = if_idx.ifr_ifindex;
	/* Address length*/
	socket_address.sll_halen = ETH_ALEN;
	/* Destination MAC */
	socket_address.sll_addr[0] = dest_mac[0];
	socket_address.sll_addr[1] = dest_mac[1];
	socket_address.sll_addr[2] = dest_mac[2];
	socket_address.sll_addr[3] = dest_mac[3];
	socket_address.sll_addr[4] = dest_mac[4];
	socket_address.sll_addr[5] = dest_mac[5];

	int count=0;
	while(1){
		if(count==BRCAST_WAITING_COUNT) break;
	    /* Send packet */
		if (sendto(sockfd, sendbuf, tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0)
		printf("Send failed\n");
	    sleep(BRCAST_WAITING_TIME);
	    count++;
     }
}

void send_dbq(){
	disc_broadcast_q dbq;
	dbq=disc_broadcast_query_init();
	char mac[6]=BCAST_MAC;
	send_all(mac,(char*)dbq,QUERY_BROADCAST,INTERFACE);
}

void send_duq(char* target_name,char* target_surname){
	unsigned char *dest_mac=get_mac(target_name);
	if(dest_mac==NULL){
		printf("Target not found.Broadcasting...\n");
		fflush(stdout);
		if(find_mac(target_name)==1){
			dest_mac=get_mac(target_name);
			disc_unicast_q duq;
			duq=disc_unicast_query_init();
			strcpy(duq->target_name,target_name);
			strcpy(duq->target_surname,target_surname);
			send_all((char*)dest_mac,(char*)duq,QUERY_UNICAST,INTERFACE);
		}else if(find_mac(target_name)==0){
			printf("No response from target.");
			fflush(stdout);
		}
	}
	else{
		disc_unicast_q duq;
		duq=disc_unicast_query_init();
		strcpy(duq->target_name,target_name);
		strcpy(duq->target_surname,target_surname);
		send_all((char*)dest_mac,(char*)duq,QUERY_UNICAST,INTERFACE);
	}
}

void send_hello_resp(char* querier_name,char* querier_surname){
	char dest_mac[6]=BCAST_MAC;
	hello hello_resp=hello_init();
	strcpy(hello_resp->querier_name,querier_name);
	strcpy(hello_resp->querier_surname,querier_surname);
	send_all(dest_mac,(char*)hello_resp,HELLO_RESPONSE,INTERFACE);
}

void send_message(char* target_name,char *msg,char packet_id){
	chat my_chat;
	unsigned char *dest_mac=get_mac(target_name);
	if(dest_mac==NULL){
		printf("Target not found.Broadcasting...\n");
		fflush(stdout);
		if(find_mac(target_name)==1){
			dest_mac=get_mac(target_name);
			my_chat=chat_init();
			unsigned short len=get_msg_len(msg);
			my_chat->length=len;
			my_chat->packet_id=packet_id;
			memcpy(my_chat->message,msg,len);
			send_all((char*)dest_mac,(char*)my_chat,CHAT,INTERFACE);
		}else if(find_mac(target_name)==0){
			printf("No response from target.");
			fflush(stdout);
		}
	}else{
		my_chat=chat_init();
		unsigned short len=get_msg_len(msg);
		my_chat->length=len;
		my_chat->packet_id=packet_id;
		memcpy(my_chat->message,msg,len);
		send_all((char*)dest_mac,(char*)my_chat,CHAT,INTERFACE);
	}

}

void send_ack(char packet_id,unsigned char* dest_mac){
	chat_ack ack;
	ack=chat_ack_init();
	ack->packet_id=packet_id;
	send_all(dest_mac,(char*)ack,CHAT_ACK,INTERFACE);
}

unsigned char* get_mac (char *my_target_name) {
	char *target_name;
	char *target_surname;
	char *mac;

	fp = fopen(filename, "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	char line[100];

	while(fgets(line, 100, fp) != NULL){
		target_name=malloc(10);
		memset(target_name,0,10);

		target_surname=malloc(10);
		memset(target_surname,0,10);

		mac=malloc(12);
		memset(mac,0,12);

		sscanf(line, "%s%s%s", target_name, target_surname,mac);
		if(strcmp(target_name,my_target_name)==0){
			printf("Target address is found");
			fflush(stdout);
			free(target_name);
			free(target_surname);
			break;
		}else{
			free(target_name);
			free(target_surname);
			mac=NULL;
		}
		//printf("%s %s %s", target_name,target_surname,target_mac);
    }

	fclose(fp);
	if(mac!=NULL){
		unsigned char *target_mac=hexMacToByte(mac);
		return target_mac;
	}else{
		return NULL;
	}
}

int find_mac(char *target_name){
	unsigned char *dest_mac;
	int count=0;
	while(1){
		send_dbq();
		if(count<MAC_WAITING_COUNT){
			dest_mac=get_mac(target_name);
			if(dest_mac!=NULL){
				printf("Info: Target is found.");
				fflush(stdout);
				return 1;
			}
		}else{
			break;
		}
		count++;
	}
	return 0;
}

unsigned char *hexMacToByte (char *s) {
  int i;
  unsigned char *result = malloc(6);
  for(i=0; i<6; i++) result[i] = (unsigned char) (hexToByte(s[2*i]) << 4 | hexToByte(s[2*i+1]));
  return result;
}

int hexToByte(char hex){
  char result;
  if (isdigit(hex))
    result = hex - '0';
  else if (isupper(hex))
    result = hex - 55;
  else if (islower(hex))
    result = hex - 87;
  return result;
}

void char2short(unsigned char* pchar, unsigned short* pshort){
  *pshort = (pchar[1] << 8) | pchar[0];
}

unsigned short get_msg_len(char* msg){
	size_t chat_len=strlen(msg);
	unsigned char len_char=(unsigned char)chat_len;
	unsigned short msg_len;
	char2short(&len_char,&msg_len);
	printf("length: %hu",msg_len);
	return msg_len;
}
