#ifndef _NETWORK_HTTPS_H_
#define _NETWORK_HTTPS_H_
#include <stddef.h>

int HttpsGetRequest(char *host, 
		char *path, char *response_buf, 
		size_t response_buf_size);

int status_check(char* r);

#endif
