#include "packets.h"
#include <string.h>
#include <stdlib.h>

typedef enum {
   QUERY_BROADCAST,
   QUERY_UNICAST,
   HELLO_RESPONSE,
   CHAT,
   CHAT_ACK,
   EXITING,
} en_pack_type;


disc_broadcast_q disc_broadcast_query_init(){
	disc_broadcast_q dbq;
	dbq=(disc_broadcast_q)malloc(sizeof(Disc_Broadcast_q));
	memset(dbq,0,sizeof(Disc_Broadcast_q));
	dbq->type=(char)QUERY_BROADCAST;
	strcpy(dbq->req_name,"pelin");
	strcpy(dbq->req_surname,"fidangul");
	return dbq;
}

disc_unicast_q disc_unicast_query_init(){
	disc_unicast_q duq;
	duq=(disc_unicast_q)malloc(sizeof(Disc_Unicast_q));
	memset(duq,0,sizeof(Disc_Unicast_q));
	duq->type=(char)QUERY_UNICAST;
	strcpy(duq->req_name,"pelin");
	strcpy(duq->req_surname,"fidangul");
	return duq;
}

hello hello_init(){
	hello hello_resp;
	hello_resp=(hello)malloc(sizeof(Hello_Response));
	memset(hello_resp,0,sizeof(Hello_Response));
	hello_resp->type=(char)HELLO_RESPONSE;
	strcpy(hello_resp->responder_name,"pelin");
	strcpy(hello_resp->responder_surname,"fidangul");
	return hello_resp;
}

chat chat_init(){
	chat my_chat;
	my_chat=(chat)malloc(sizeof(Chat_t));
	memset(my_chat,0,sizeof(Chat_t));
	my_chat->type= (char) CHAT;
	return my_chat;
}

chat_ack chat_ack_init(){
	chat_ack chat_resp;
	chat_resp=(chat_ack)malloc(sizeof(Chat_Ack_t));
	memset(chat_resp,0,sizeof(Chat_Ack_t));
	chat_resp->type=(char)CHAT_ACK;
	chat_resp->packet_id=0;
	return chat_resp;
}

exiting exiting_init(){
	exiting exiting_me=(exiting)malloc(sizeof(Exiting_t));
	memset(exiting_me,0,sizeof(Exiting_t));
	exiting_me->type=(char)EXITING;
	strcpy(exiting_me->name,"pelin");
	strcpy(exiting_me->surname,"fidangul");
	return exiting_me;

}
