
#include "peer_conn.h"

typedef struct listen_args
	char* port_str;
	char* ip_addr;
} struct listen_args;

void* listen_servers ( void* arg ) {
	puts( "HWEWOTWEJIO" );
	pthread_exit();
}

void* listen_clients ( void* arg ) {
	puts( "HWEWOTWEJIsdfO" );
	pthread_exit();
}
