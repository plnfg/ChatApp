/*
 * packets.h
 *
 *  Created on: Jul 18, 2017
 *      Author: plnfg
 */

#ifndef CHAT_H_
#define CHAT_H_

//int hexToDec(char ch){
//  int dec;
//  if(ch >= 48 && ch <= 57) dec = ch - 48;
//  else dec = ch - 87;
//  return dec;
//}
//
//int hex_to_ascii(char c, char d){
//        int high = hexToDec(c) * 16;
//        int low = hexToDec(d);
//        return high+low;
//}

void send_all(char *dest_mac,char* packet,char type,char* interface);
void send_dbq();
void send_duq(char*,char*);
void send_hello_resp(char*,char*);
void send_message(char*,char*,char);
void send_ack(char,unsigned char*);
unsigned char* get_mac(char*);
int find_mac(char*);
unsigned char *hexMacToByte (char *s);
int hexToByte(char hex);
void char2short(unsigned char* pchar, unsigned short* pshort);
unsigned short get_msg_len(char* msg);

//void* get_struct_hex(char type){
//	disc_broadcast_q dbq;
//	disc_unicast_q duq;
//	hello hello_r;
//	chat new_chat;
//
//	unsigned char *hex;
//	unsigned char *pc;
//	int i,k=0;
//
//	switch(type){
//		case(QUERY_BROADCAST):
//			dbq=disc_broadcast_query_init();
//			 pc= (unsigned char*)dbq;
//			hex=malloc(sizeof(Disc_Broadcast_q)*2);
//
//			for (i = 0; i < sizeof(Disc_Broadcast_q); i++)  {
//			  printf("%02x",pc[i]);
//			  printf("  ");
//			  sprintf(&hex[k],"%02x",pc[i]);
//			  k+=2;
//			}
//			break;
//		case(QUERY_UNICAST):
//			duq=disc_unicast_query_init();
//			pc = (unsigned char*)duq;
//			hex=malloc(sizeof(Disc_Unicast_q)*2);
//
//			for (i = 0; i < sizeof(Disc_Unicast_q); i++)  {
//			  printf("%02x",pc[i]);
//			  printf("");
//			  sprintf(&hex[k],"%02x",pc[i]);
//			  k+=2;
//			}
//			break;
//		case(HELLO_RESPONSE):
//			hello_r=hello_init();
//			pc = (unsigned char*)hello_r;
//			hex=malloc(sizeof(Hello_Response)*2);
//
//			for (i = 0; i < sizeof(Hello_Response); i++)  {
//			  printf("%02x",pc[i]);
//			  printf("");
//			  sprintf(&hex[k],"%02x",pc[i]);
//			  k+=2;
//			}
//			break;
//		case(CHAT):
//			new_chat=chat_init();
//			pc=(unsigned char*)new_chat;
//			hex=malloc(sizeof(Chat_t)*2);
//
//			for (i = 0; i < sizeof(Chat_t); i++)  {
//			  printf("%02x",pc[i]);
//			  printf("");
//			  sprintf(&hex[k],"%02x",pc[i]);
//			  k+=2;
//			}
//			break;
//	}
//	free(pc);
//	return hex;
//}
//
//void* hex_to_struct(char type,unsigned char* hex){
//	int i,j;
//	disc_broadcast_q dbq_resp;
//	disc_unicast_q duq_resp;
//	hello hello_resp;
//	chat chat_resp;
//
//	size_t duq_size,hello_size;
//
//	switch(type){
//		case(QUERY_BROADCAST):
//			dbq_resp=(disc_broadcast_q)malloc(sizeof(Disc_Broadcast_q));
//
//			int count_dpq_name=0;
//			int count_dpq_surname=0;
//
//		    dbq_resp->type=hex_to_ascii(hex[0],hex[1]);
//
//		    char buf=0;
//		    int ascii;
//		    for(i = 2; i <sizeof(Disc_Broadcast_q)*2 ; i++){
//		    	if(i%2!=0){
//		    		ascii= hex_to_ascii(buf, hex[i]);
//		    		char c=(char)ascii;
//		    		if(i>=2 && i<=21){
//		    			dbq_resp->req_name[count_dpq_name]=c;
//		    			count_dpq_name++;
//		    		}else{
//		    			dbq_resp->req_surname[count_dpq_surname]=c;
//		    			count_dpq_surname++;
//		    		}
//		    	}else{
//		    		buf=hex[i];
//		    	}
//		    }
//
//		    return dbq_resp;
//			break;
//
//		case(QUERY_UNICAST):
//			duq_size=sizeof(Disc_Unicast_q);
//			duq_resp=(disc_unicast_q)malloc(duq_size);
//
//			int count_duq_name=0, count_duq_surname=0;
//			int count_t_name=0, count_t_surname=0;
//
//			duq_resp->type=hex_to_ascii(hex[0],hex[1]);
//			char buf_duq=0;
//			int ascii_duq;
//			for(i = 2; i <duq_size*2 ; i++){
//				if(i%2!=0){
//					ascii_duq= hex_to_ascii(buf_duq, hex[i]);
//					char c=(char)ascii_duq;
//
//					if(i>=2 && i<=21){
//						duq_resp->req_name[count_duq_name]=c;
//						count_duq_name++;
//					}else if(i>=22 && i<=41){
//						duq_resp->req_surname[count_duq_surname]=c;
//						count_duq_surname++;
//					}else if(i>=42 && i<=61){
//						duq_resp->target_name[count_t_name]=c;
//						count_t_name++;
//					}else{
//						duq_resp->target_surname[count_t_surname]=c;
//						count_t_surname++;
//					}
//				}else{
//					buf_duq=hex[i];
//				}
//
//			}
//			return duq_resp;
//			break;
//		case(HELLO_RESPONSE):
//			hello_size=sizeof(Hello_Response);
//			hello_resp=(hello)malloc(hello_size);
//
//			int count_resp_name=0, count_resp_surname=0;
//			int count_q_name=0, count_q_surname=0;
//
//			hello_resp->type=hex_to_ascii(hex[0],hex[1]);
//
//			char buf_hello=0;
//			int ascii_hello;
//			for(i = 2; i <hello_size*2 ; i++){
//				if(i%2!=0){
//					ascii_hello= hex_to_ascii(buf_hello, hex[i]);
//					char c=(char)ascii_hello;
//					if(i>=2 && i<=21){
//						hello_resp->responder_name[count_resp_name]=c;
//						count_resp_name++;
//					}else if(i>=22 && i<=41){
//						hello_resp->responder_surname[count_resp_surname]=c;
//						count_resp_surname++;
//					}else if(i>=42 && i<=61){
//						hello_resp->querier_name[count_q_name]=c;
//						count_q_name++;
//					}else{
//						hello_resp->querier_surname[count_q_surname]=c;
//						count_q_surname++;
//					}
//				}else{
//					buf_hello=hex[i];
//				}
//			}
//			return hello_resp;
//			break;
//		case(CHAT):
//			chat_resp=(chat)malloc(sizeof(Chat_t));
//			int count_msg=0;
//
//			chat_resp->type=hex_to_ascii(hex[0],hex[1]);
//			int high = hex_to_ascii(hex[2], hex[3]);
//			int low = hex_to_ascii(hex[4], hex[5]);
//			chat_resp->length = (low << 8) | high;
//			chat_resp->packet_id=hex_to_ascii(hex[6],hex[7]);
//
//		    char buf_chat=0;
//		    int ascii_chat;
//		    for(i = 8; i <sizeof(Chat_t)*2 ; i++){
//		    	if(i%2!=0){
//		    		ascii_chat= hex_to_ascii(buf_chat, hex[i]);
//		    		char c=(char)ascii_chat;
//		    		chat_resp->message[count_msg]=c;
//		    		count_msg++;
//		    	}else{
//		    		buf_chat=hex[i];
//		    	}
//		    }
//		    return chat_resp;
//		    break;
//	}
//
//	free(hex);
//	return 0;
//}




#endif /* CHAT_H_ */
