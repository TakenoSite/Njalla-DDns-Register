#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "https.h"

#define HTTP_RESPONSE_BUF_SIZE 256
#define HTTPS "https"
#define HTTP_HEADER "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n"

static void SSLCrear(SSL *ssl, SSL_CTX *ctx)
{
	SSL_shutdown(ssl);
	SSL_free(ssl);
	SSL_CTX_free(ctx);
	ERR_free_strings();
}

int SockTimeout(int sockfd,int sec, int usec)
{
	struct timeval tv;
	int net_select;
	fd_set readfds;
	
	tv.tv_sec = sec;
	tv.tv_usec = usec;
	
	FD_ZERO(&readfds);
	FD_SET(sockfd, &readfds);

	net_select = select(sockfd + 0x01, &readfds, NULL, NULL, &tv);
	return net_select;
};


int HttpsGetRequest(char *host, char *path, char *response_buf, size_t response_buf_size)
{
    int sockfd;
    struct addrinfo hints, *res;
	int pending;
	int timeout;
	int err;

	size_t len;
	size_t cpy_len = 0;	
    char *http_proto;
	char *body;
	char buf[HTTP_RESPONSE_BUF_SIZE];
    int read_size;
	
	SSL *ssl;
    SSL_CTX *ctx;

    memset(&hints, 0, sizeof(hints));
	memset(buf, 0, sizeof(buf));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
		
    err = 0;
	// ip address resolve  
    if ((err = getaddrinfo(host, HTTPS, &hints, &res)) != 0) return -1;
	// sock generate 
	if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) return -2;
		
	if (connect(sockfd, res->ai_addr, res->ai_addrlen) != 0)
	{
		close(sockfd);
		return -3;
	}
	
    SSL_load_error_strings();
    SSL_library_init();
	
	// ssl generate 
    ctx = SSL_CTX_new(SSLv23_client_method());
    ssl = SSL_new(ctx);
    err = SSL_set_fd(ssl, sockfd);
    SSL_connect(ssl);
	
	// http proto msg buf
	len = sizeof(char)*strlen(path) + sizeof(char)*strlen(host) + 32;
	http_proto = (char*)malloc(len);
	
	// create msg 
    sprintf(http_proto ,HTTP_HEADER, path, host);
	// send
	
	SSL_write(ssl, http_proto, strlen(http_proto));
	
	do {
		pending = SSL_pending(ssl); 
		timeout = SockTimeout(SSL_get_fd(ssl), 5, 0);
		if(!pending && timeout == 0)
		{
			SSLCrear(ssl, ctx);
			free(http_proto);
			close(sockfd);
				
			// timeout if -4
			return -4;
		}
		
		read_size = SSL_read(ssl, buf, HTTP_RESPONSE_BUF_SIZE);

		if(response_buf_size < cpy_len+strlen(buf))
		{
			SSLCrear(ssl,ctx);
			free(http_proto);
			close(sockfd);
		
			// over buffer
			
			return -5;
		}
		strcat(response_buf, buf);
		cpy_len += strlen(buf);
	} while(read_size > 0);

	SSLCrear(ssl,ctx);
	free(http_proto);
    close(sockfd);
	
	return 0;
}

static int char_int(char* str_)
{
	int to_int;
	
	if(str_ == NULL) return -1;
	sscanf(str_,"%d",&to_int);
	return to_int;
}

int status_check(char* r)
{
	short int_status_code;
	char* r_cpy = (char*)malloc(sizeof(char) * strlen(r)+32);
	if(r_cpy == NULL){
		return -1;
	};

	sprintf(r_cpy,"%s",r);

	char *ptr = strtok(r_cpy, "\r\n");
	char *status_code = strtok(ptr, " ");
	status_code = strtok(NULL, " ");
	int_status_code = char_int(status_code);
	free(r_cpy);

	return int_status_code;
}
// end
