///*
// *  This program is free software: you can redistribute it and/or modify
// *  it under the terms of the GNU General Public License as published by
// *  the Free Software Foundation, either version 3 of the License, or
// *  (at your option) any later version.
// */
//
//#include <arpa/inet.h>
//#include <linux/if_packet.h>
//#include <linux/ip.h>
//#include <linux/udp.h>
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <sys/ioctl.h>
//#include <sys/socket.h>
//#include <net/if.h>
//#include <netinet/ether.h>
//#include <unistd.h>
//#include "chat.h"
//#include "packets.h"
//
//typedef enum {
//   QUERY_BROADCAST,
//   QUERY_UNICAST,
//   HELLO_RESPONSE,
//   CHAT,
//   CHAT_ACK,
//   EXITING,
//} en_pack_type;
//
//char hexToDec(int num){
//  char c;
//  if(num >=0 && num <= 9) c = (char) num + 48;
//  else c = (char) num + 55;
//  return c;
//}
//
//unsigned char *byteToHex(unsigned char *str){
//  int i;
//  unsigned char *result = malloc(12);
//  memset(result,0,12);
//  for(i=0; i<6; i++){
//    int num = (int) str[i];
//    result[2*i] = (unsigned char) hexToDec(num / 16);
//    result[2*i + 1] = (unsigned char) hexToDec(num % 16);
//  }
//
//  return result;
//}
//
//void save_mac(char *targetAddr, char *name, char *surname){
//  FILE *file = fopen("address.txt", "a");
//  if (file == NULL)
//  {
//      printf("Error opening file!\n");
//      exit(1);
//  }
//
//  unsigned char *mac;
//  mac=malloc(6);
//  memset(mac,0,6);
//
//  mac=byteToHex((unsigned char*)targetAddr);
//
//
//
//  /* print some text */
//  fprintf(file, "%s %s %s\n", name, surname, mac);
//
//  fclose(file);
//
//}
//
//#define BUF_SIZ		1024
//#define INTERFACE  "enp14s0"
//#define BCAST_MAC {0xff,0xff,0xff,0xff,0xff,0xff}
//#define ETHER_TYPE	0x1234
//
//unsigned char* DEST_MAC;
//
//int main(){
//	char sender[INET6_ADDRSTRLEN];
//	int sockfd, ret, i;
//	int sockopt;
//	ssize_t numbytes;
//	struct ifreq ifopts;	/* set promiscuous mode */
//	struct ifreq if_ip;	/* get ip addr */
//	struct sockaddr_storage their_addr;
//	uint8_t buf[BUF_SIZ];
//	char ifName[IFNAMSIZ];
//
//
//	strcpy(ifName, INTERFACE);
//
//	/* Header structures */
//	struct ether_header *eh = (struct ether_header *) buf;
//	struct iphdr *iph = (struct iphdr *) (buf + sizeof(struct ether_header));
//	struct udphdr *udph = (struct udphdr *) (buf + sizeof(struct iphdr) + sizeof(struct ether_header));
//
//	memset(&if_ip, 0, sizeof(struct ifreq));
//
//	/* Open PF_PACKET socket, listening for EtherType ETHER_TYPE */
//	if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETHER_TYPE))) == -1) {
//		perror("listener: socket");
//		return -1;
//	}
//
//	/* Set interface to promiscuous mode - do we need to do this every time? */
//	strncpy(ifopts.ifr_name, ifName, IFNAMSIZ-1);
//	ioctl(sockfd, SIOCGIFFLAGS, &ifopts);
//	ifopts.ifr_flags |= IFF_PROMISC;
//	ioctl(sockfd, SIOCSIFFLAGS, &ifopts);
//	/* Allow the socket to be reused - incase connection is closed prematurely */
//	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt) == -1) {
//		perror("setsockopt");
//		close(sockfd);
//		exit(EXIT_FAILURE);
//	}
//	/* Bind to device */
//	if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, ifName, IFNAMSIZ-1) == -1)	{
//		perror("SO_BINDTODEVICE");
//		close(sockfd);
//		exit(EXIT_FAILURE);
//	}
//
//repeat:	//printf("listener: Waiting to recvfrom...\n");
//	numbytes = recvfrom(sockfd, buf, BUF_SIZ, 0, NULL, NULL);
//	//printf("listener: got packet %lu bytes\n", numbytes);
//
//	/* Check the packet is for me */
//	if ((eh->ether_dhost[0] == 0x78 &&
//			eh->ether_dhost[1] == 0x84 &&
//			eh->ether_dhost[2] == 0x3c &&
//			eh->ether_dhost[3] == 0x38 &&
//			eh->ether_dhost[4] == 0x14 &&
//			eh->ether_dhost[5] == 0xf4) ||
//			(eh->ether_dhost[0] == 0Xff &&
//			eh->ether_dhost[1] == 0Xff &&
//			eh->ether_dhost[2] == 0Xff &&
//			eh->ether_dhost[3] == 0Xff &&
//			eh->ether_dhost[4] == 0Xff &&
//			eh->ether_dhost[5] == 0Xff)){
//		printf("Correct destination MAC address\n");
//	} else {
//		/*printf("Wrong destination MAC: %x:%x:%x:%x:%x:%x\n",
//						eh->ether_dhost[0],
//						eh->ether_dhost[1],
//						eh->ether_dhost[2],
//						eh->ether_dhost[3],
//						eh->ether_dhost[4],
//						eh->ether_dhost[5]);*/
//		ret = -1;
//		goto done;
//	}
//
//	/* Get source IP */
//	((struct sockaddr_in *)&their_addr)->sin_addr.s_addr = iph->saddr;
//	inet_ntop(AF_INET, &((struct sockaddr_in*)&their_addr)->sin_addr, sender, sizeof sender);
//
//	/* Look up my device IP addr if possible */
//	strncpy(if_ip.ifr_name, ifName, IFNAMSIZ-1);
//	if (ioctl(sockfd, SIOCGIFADDR, &if_ip) >= 0) { /* if we can't check then don't */
//		printf("Source IP: %s\n My IP: %s\n", sender,
//				inet_ntoa(((struct sockaddr_in *)&if_ip.ifr_addr)->sin_addr));
//		/* ignore if I sent it */
//		if (strcmp(sender, inet_ntoa(((struct sockaddr_in *)&if_ip.ifr_addr)->sin_addr)) == 0)	{
//			printf("but I sent it :(\n");
//			ret = -1;
//			goto done;
//		}
//	}
//
//	/* UDP payload length */
//	ret = ntohs(udph->len) - sizeof(struct udphdr);
//
//	/* Print packet */
//
//	char *data = malloc(numbytes-18);
//	char *mac=malloc(6);
//	memcpy(mac,&buf[6],6);
//	printf("mac address: %s\n",mac);
//	char type=buf[14];
//	size_t size;
//
//	switch(type){
//		case(QUERY_BROADCAST):
//			size=sizeof(Disc_Broadcast_q);
//			memcpy(data,&buf[14],size);
//			disc_broadcast_q dbq =(disc_broadcast_q)data;
//			printf("\nDQB - [%d][%s][%s]", dbq->type, dbq->req_name, dbq->req_surname);
//			if(get_mac(dbq->req_name)==NULL) save_mac(mac,dbq->req_name,dbq->req_surname);
//			send_hello_resp(dbq->req_name,dbq->req_surname);
//			break;
//		case(QUERY_UNICAST):
//			size=sizeof(Disc_Unicast_q);
//			memcpy(data,&buf[14],size);
//			disc_unicast_q duq =(disc_unicast_q)data;
//			printf("\nDUB - [%d][%s][%s][%s][%s]", duq->type, duq->req_name, duq->req_surname,duq->target_name,duq->target_surname);
//			send_hello_resp(duq->req_name,duq->req_surname);
//			break;
//		case(HELLO_RESPONSE):
//			size=sizeof(Hello_Response);
//			memcpy(data,&buf[14],size);
//			hello hello_resp =(hello)data;
//			printf("\nHello Response - [%d][%s][%s][%s][%s]", hello_resp->type, hello_resp->querier_name, hello_resp->querier_surname,hello_resp->responder_name,hello_resp->responder_surname);
//			break;
//		case(CHAT):
//			size=sizeof(Chat_t);
//			memcpy(data,&buf[14],size);
//			chat my_chat=(chat)data;
//			send_ack(my_chat->packet_id,mac);
//			printf("\nChat Message - [%d][%hu][%d][%s]", my_chat->type, my_chat->length, my_chat->packet_id,my_chat->message);
//			break;
//		case(CHAT_ACK):
//			size=sizeof(Chat_Ack_t);
//			memcpy(data,&buf[14],size);
//			chat_ack ack=(chat_ack)data;
//			printf("\nACK - [%d][%d]", ack->type, ack->packet_id);
//			break;
//	}
//
//
//
//	fflush(stdout);
//
//
//done:	goto repeat;
//
//	close(sockfd);
//	return 0;
//
//}
//
//
