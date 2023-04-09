#ifndef _DDNS_REGISTER_H_
#define _DDNS_REGISTER_H_
		
struct DDNS_OPTION{
	char *key;
	char *host;
	char *subdomain;
};
typedef struct DDNS_OPTION DDNS_OPTION;

int DDnsWite(DDNS_OPTION d_o);

#endif 
