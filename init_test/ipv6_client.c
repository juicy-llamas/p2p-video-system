
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
	int fd = socket( AF_INET6, SOCK_STREAM, IPPROTO_TCP );
    // Server addr info
    struct addrinfo hint = {};
	struct addrinfo* res;
	// Listen options
	int opt_ipv6only = 0;
	char* port_no_str = "8888";
	// For storing getaddrinfo error
	int err_num = 0;

	if ( fd == -1 ) err_fun( "sock init failed", errno );

	if ( setsockopt( fd, IPPROTO_IPV6, IPV6_V6ONLY, (const void*)&opt_ipv6only, sizeof( int ) ) == -1 ) err_fun( "setsockopt failed", errno );

	hint.ai_family = AF_INET6;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = IPPROTO_TCP;

	if ( ( err_num = getaddrinfo( "::0", port_no_str, &hint, &res ) ) == -1 ) err_fun( gai_strerror( err_num ), errno );

	if ( connect( fd, res->ai_addr, res->ai_addrlen ) == -1 ) err_fun( "connecting to address failed", errno );
}
