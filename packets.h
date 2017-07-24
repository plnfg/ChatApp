#ifndef PACKETS_H_
#define PACKETS_H_

typedef struct{
	char type;
	char req_name[10];
	char req_surname[10];
}Disc_Broadcast_q[1],*disc_broadcast_q;

typedef struct{
	char type;
	char req_name[10];
	char req_surname[10];
	char target_name[10];
	char target_surname[10];
}__attribute__((packed)) Disc_Unicast_q[1],*disc_unicast_q;

typedef struct{
	char type;
	char responder_name[10];
	char responder_surname[10];
	char querier_name[10];
	char querier_surname[10];
} Hello_Response,*hello;

typedef struct{
	char type;
	short length;
	char packet_id;
	char message[100];
}__attribute__((packed)) Chat_t[1],*chat;

typedef struct{
	char type;
	char packet_id;
}Chat_Ack_t[1],*chat_ack;

typedef struct{
	char type;
	char name[10];
	char surname[10];
}Exiting_t[1],*exiting;

disc_broadcast_q disc_broadcast_query_init();
disc_unicast_q disc_unicast_query_init();
hello hello_init();
chat chat_init();
chat_ack chat_ack_init();
exiting exiting_init();

#endif /* PACKETS_H_ */
