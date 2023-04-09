#include <stdio.h>
#include "../register.h"

int main(){
	DDNS_OPTION d_o;
	int status;
	
	/*
type
	 */
	 
	char *key = "<key>";
	char *host = "<host>";
	char *subdomain = "<subdomain>";

	/*
format:
	https://njal.la/update/?h=<subdomain>&k=<key>&a=<>
	 */	
	
	d_o.key = key;
	d_o.host = host;
	d_o.subdomain = subdomain;

	status = DDnsWite(d_o);
	switch(status)
	{
		case 200:
			printf("%s to set %s", d_o.subdomain, d_o.host);
			break;
		default:
			printf("Could not set. status code  %d", status);
	}

	return 0;
}
