#include <stdio.h>
#include "https.h"
#include "register.h"

#define NJALLA_DOMAIN "njal.la"
#define URN_BUF 128
#define RES_BUF 12000
#define PAYLOAD "/update/?h=%s&k=%s&a=%s" // set ipv4


int DDnsWite(DDNS_OPTION d_o)
{
	int e;
	int status;
	char urn[URN_BUF];
	char response_buf[RES_BUF];
	
	snprintf(urn, sizeof(urn), PAYLOAD,
			d_o.subdomain, d_o.key, d_o.host);

#ifdef DEBUG 
	printf("urn : %s\n", urn);
#endif 
	e = HttpsGetRequest(NJALLA_DOMAIN, urn, 
			response_buf, sizeof(response_buf));
		
	switch(e)
	{
		case 0:
			status = status_check(response_buf);
#ifdef DEBUG
			printf("DDnsWite ->status :%d\nResponse :%s\n", status,response_buf);
#endif
			return status;
		default:
			return status;
	}		
}
