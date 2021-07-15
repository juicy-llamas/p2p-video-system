
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

void err_fun ( const char* msg, int err_no ) {
	puts( "Application Failed!" );
	printf( "Message: %s\n", msg );
	printf( "Errno Message: %s\n", strerror( err_no ) );
	exit( 1 );
}

int main () {
	// Listen socket
	int lfd = socket( AF_INET6, SOCK_STREAM, IPPROTO_TCP );
	// Client socket / address for connection
	struct sockaddr_in6 client_address;
	int cfd = 0;
	char cl_addr_str[ INET6_ADDRSTRLEN ];
	socklen_t addrlen = sizeof( struct sockaddr_in6 );
	// Listen options
	int opt_ipv6only = 0;
	char* port_no_str = "8888";
	// For storing getaddrinfo error
	int err_num = 0;

	if ( lfd == -1 ) err_fun( "sock init failed", errno );

	if ( setsockopt( lfd, IPPROTO_IPV6, IPV6_V6ONLY, (const void*)&opt_ipv6only, sizeof( int ) ) == -1 ) err_fun( "setsockopt failed", errno );

	struct addrinfo hint = {};
	struct addrinfo* res;
	hint.ai_family = AF_INET6;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = IPPROTO_TCP;

	if ( ( err_num = getaddrinfo( "::0", port_no_str, &hint, &res ) ) == -1 ) err_fun( gai_strerror( err_num ), errno );

	if ( bind( lfd, res->ai_addr, res->ai_addrlen ) == -1 ) err_fun( "binding to address failed", errno );

	if ( listen( lfd, SOMAXCONN ) == -1 ) err_fun( "listening to address failed", errno );

	while ( ( cfd = accept( lfd, (struct sockaddr*) &client_address, &addrlen ) ) != -1 ) {
		inet_ntop(AF_INET6, &(client_address.sin6_addr),
				cl_addr_str, sizeof(cl_addr_str ) );
		printf("New connection from: %s:%d!!!!!!!!!!!!!!!\n",
				cl_addr_str,
				ntohs(client_address.sin6_port));

		close( cfd );
		cfd = 0;
	}

	if ( cfd == -1 ) err_fun( "accept failed", errno );
	close( lfd );
	return 0;
}
