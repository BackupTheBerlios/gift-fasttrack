/*
 * $Id: sniff.c,v 1.1 2003/07/27 01:17:33 hex Exp $
 *
 * Based on printall.c from libnids/samples, which is
 * copyright (c) 1999 Rafal Wojtczuk <nergal@avet.com.pl>. All rights reserved.
*/

/* 
 * To compile:
 * 1. install libnids
 * 2. comment out line #include "fst_fasttrack.h" in crypt/fst_crypt.c
 * 3. gcc -g -Wall -o sniff sniff.c -I. -Icrypt crypt/enc_type_*.c md5.c -lnids -lnet -lpcap
 *
 * To run (as root):
 * ./sniff [interface]
 *
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

#include <nids.h>

#include "crypt/fst_crypt.h"

#define FST_WARN(fmt)
#define FST_WARN_1(fmt,a)
#define FST_WARN_2(fmt,a,b)
#define FST_WARN_3(fmt,a,b,c)
#define FST_ERR(fmt)
#define FST_ERR_1(fmt,a)
#define FST_ERR_2(fmt,a,b)
#define FST_ERR_3(fmt,a,b,c)
#define FST_DBG(fmt)
#define FST_DBG_1(fmt,a)
#define FST_DBG_2(fmt,a,b)
#define FST_DBG_3(fmt,a,b,c)
#define FST_DBG_4(fmt,a,b,c,d)
#define FST_DBG_5(fmt,a,b,c,d,e)
#define FST_HEAVY_DBG(fmt)
#define FST_HEAVY_DBG_1(fmt,a)
#define FST_HEAVY_DBG_2(fmt,a,b)
#define FST_HEAVY_DBG_3(fmt,a,b,c)
#define FST_HEAVY_DBG_4(fmt,a,b,c,d)
#define FST_HEAVY_DBG_5(fmt,a,b,c,d,e)

#define TRUE 1
#define FALSE 0

#include "crypt/fst_crypt.c"


#define EVIL /* mmm... evil... */

enum {
	STATE_CLIENT_KEY,
	STATE_SERVER_KEY,
	STATE_SERVER_NETNAME,
	STATE_CLIENT_NETNAME,
	STATE_ESTABLISHED,
	STATE_UNSUPPORTED,
	STATE_HTTP,
	STATE_PUSH
};

static int id=0;
static time_t last_time=0;

struct session {
	FSTCipher *in_cipher;
        FSTCipher *out_cipher;
        unsigned int in_xinu;
        unsigned int out_xinu;
	int state;
	unsigned long rand;
	int id;
};

#define INT(x) ntohl(*((unsigned long*)(data+x)));

#define int_ntoa(x)	inet_ntoa(*((struct in_addr *)&x))

char *
adres (struct tuple4 *addr, int id)
{
  static char buf[256];
  strcpy (buf, int_ntoa (addr->saddr));
  sprintf (buf + strlen (buf), ":%d -> ", addr->source);
  strcat (buf, int_ntoa (addr->daddr));
  sprintf (buf + strlen (buf), ":%d", addr->dest);
  if (id)
	  sprintf (buf + strlen (buf), " [%d]", id);
  return buf;
}

void print_bin_data(unsigned char * data, int len)
{
        int i;
        int i2;
        int i2_end;

//      printf("data len %d\n", data_len);

        for (i2 = 0; i2 < len; i2 = i2 + 16)
        {
                i2_end = (i2 + 16 > len) ? len: i2 + 16;
                for (i = i2; i < i2_end; i++)
                        if (isprint(data[i]))
                                fprintf(stderr, "%c", data[i]);
                        else
                        fprintf(stderr, ".");
                for ( i = i2_end ; i < i2 + 16; i++)
                        fprintf(stderr, " ");
                fprintf(stderr, " | ");
                for (i = i2; i < i2_end; i++)
                        fprintf(stderr, "%02x ", data[i]);
                fprintf(stderr, "\n");
        }
}

int verify_port (int p) {
	return !(p==1217 ||
		 p==1216 ||
		 p==1215 ||
		 p>5000 ||
		 p<1000);
}


void tcp_callback (struct tcp_stream *tcp, struct session **conn) {
	char buf[32];

	struct session *c=*conn;

	switch (tcp->nids_state) {
	case NIDS_JUST_EST:
	{
		if (!verify_port (tcp->addr.dest))
			return;

		c=malloc(sizeof(struct session));
		if (!c)
			abort();

		c->in_xinu=c->out_xinu=0x51;
		c->in_cipher=c->out_cipher=NULL;
		c->state=STATE_CLIENT_KEY;
		c->id=0;

		*conn=c;
		
		tcp->client.collect++; // we want data received by a client
		tcp->server.collect++; // and by a server, too

		//fprintf (stderr, "%s established\n", buf);
		return;
	}
	case NIDS_CLOSE:
		// connection has been closed normally
		if (c && c->id) {
			fprintf (stderr, "[%d] closing\n", c->id);
			free(c->in_cipher);
			free(c->out_cipher);
			free(c);
			*conn=NULL;
		}
		return;
	case NIDS_RESET:
		// connection has been closed by RST
		if (c && c->id) {
			fprintf (stderr, "[%d] reset\n", c->id);
			free(c->in_cipher);
			free(c->out_cipher);
			free(c);
			*conn=NULL;
		}
		break;
	case NIDS_DATA:
	{
		// new data has arrived; gotta determine in what direction
		// and if it's urgent or not

		struct half_stream *hlf;
		int done=0;
		int len;
		unsigned char *data;
		int this_len;
		int read;
		int done_read=0;
		int server;

		if (tcp->client.count_new)
			hlf = &tcp->client; 
		else
			hlf = &tcp->server;
		server = !!tcp->client.count_new;

		len = hlf->count - hlf->offset;
		this_len=hlf->count_new;
		data=hlf->data;

		if (c->state != STATE_UNSUPPORTED) {
			time_t this_time=time(NULL);
			if (this_time-last_time>120) {
				/* print the time every 2 minutes */
				fprintf(stderr, "%s", ctime(&this_time));
				last_time=this_time;
			}
		}

		do {
			sprintf (buf, "[%d %s] ",c->id, server?"<-":"->");
			read=0;
		switch (c->state) {
		case STATE_CLIENT_KEY:
			fprintf(stderr, "%s\n", adres (&tcp->addr, c->id=++id));
			sprintf (buf, "[%d %s] ",c->id, server?"<-":"->");

			if (len>=5 && !server) {
				/* first check if this looks like a transfer */
				if (!memcmp(data, "GET ",4) ||
				    !memcmp(data, "HEAD ",5)) {
					c->state=STATE_HTTP;
					break;
				}
				if (!memcmp(data, "GIVE ",5)) {
					c->state=STATE_PUSH;
					break;
				}
			}

			if (len>=12 && !server) {
				unsigned long rand, seed, enc_type;

				rand=INT(0);
				seed=INT(4);
				enc_type=INT(8);
				
				c->rand=rand;

				c->out_cipher=fst_cipher_create();

				//for(i=0;i<len;i++)
				//	fprintf(stderr, "%02x ",data[i]);
				c->out_cipher->enc_type=fst_cipher_decode_enc_type(seed, enc_type);
				c->out_cipher->seed=seed;
				
				if (c->out_cipher->enc_type & ~0xff) {
					fprintf(stderr, "%s ???\n", buf);
					print_bin_data (data, len);
					c->state=STATE_UNSUPPORTED;
					done=1;
				} else {
					read=12;
					c->state++;
				}
			} else
				done=1;

			break;

		case STATE_SERVER_KEY:
			if (len>=8 && server) {
				unsigned long seed, enc_type;
				seed=INT(0);
				enc_type=INT(4);
				c->out_cipher->seed^=seed;
				c->in_cipher=fst_cipher_create();
				enc_type=fst_cipher_decode_enc_type(seed, enc_type);

				if (fst_cipher_init(c->out_cipher, c->out_cipher->seed, c->out_cipher->enc_type) &&
				    fst_cipher_init(c->in_cipher, seed, enc_type)) {
					fprintf(stderr, "[%d] in: %x, out %x [rand %04lx]\n", c->id, c->in_cipher->enc_type,c->out_cipher->enc_type, c->rand);
					c->state++;
				} else {
					fprintf(stderr,"[%d] init failed (%x,%x)\n", c->id, c->in_cipher->enc_type,c->out_cipher->enc_type);
#ifdef EVIL
					/* ensure we can eavesdrop on all traffic, by evilly
					   resetting the connection if decryption fails :) */
					nids_killtcp (tcp);
#endif
					c->state=STATE_UNSUPPORTED;
				}
				read=8;
			} else
				done=1;

			break;
		case STATE_SERVER_NETNAME:
			if (server) {
				//fprintf(stderr, "[<-] decrypting %d of %d bytes\n", this_len, len);
				fst_cipher_crypt (c->in_cipher, data + len - this_len, this_len);
				this_len=0;

				if (memchr(data, 0, len)) {
					fprintf(stderr, "%s network name: '%s'\n",buf, data);
					read=strlen(data)+1;
					c->state++;
				} else {
					fprintf(stderr, "buffering partially decrypted netname (%d)\n", len);
					done=1;
				}
			} else
				done=1;
			break;

		case STATE_CLIENT_NETNAME:
			if (!server) {
				//fprintf(stderr, "[->] decrypting %d of %d bytes\n", this_len, len);
				fst_cipher_crypt (c->out_cipher, data + len - this_len, this_len);
				this_len=0;

				if (memchr(data, 0, len)) {
					fprintf(stderr, "%s network name: '%s'\n",buf, data);
					read=strlen(data)+1;
					c->state++;
				} else {
					fprintf(stderr, "buffering partially decrypted netname (%d)\n", len);
					done=1;
				}
			} else
				done=1;
			break;

		case STATE_ESTABLISHED:
			if (this_len>0) {
				//fprintf(stderr, "%s decrypting %d of %d bytes\n", buf, this_len, len);
				fst_cipher_crypt (server? c->in_cipher : c->out_cipher, data + len - this_len, this_len);
				this_len=0;
			}

			switch (*data) {
			case 0x50:
				fprintf(stderr, "%s ping?\n", buf);
				read=1;
				break;
			case 0x52:
				fprintf(stderr, "%s pong\n", buf);
				read=1;
				break;
			case 0x4B:
				if (len>=5) {
					unsigned int *xinu = server?&c->in_xinu:&c->out_xinu;
					int xtype = *xinu % 3;
					int type, msglen;
					switch(xtype) {
					case 0:
						type=data[1];
						msglen=(data[3]<<8)+data[4];
						break;
					case 1:
						type=data[3];
						msglen=(data[2]<<8)+data[4];
						break;
					case 2:
						type=data[4];
						msglen=(data[3]<<8)+data[2];
						break;
					}
					if (len>=msglen+5) {
						read=msglen+5;
						fprintf(stderr, "%s message type %02x, len %d\n", buf, type, msglen);
						*xinu ^= ~(type + msglen);
						print_bin_data(data+5,msglen);
					} else {
						fprintf(stderr, "%s (%02x %d/%d... [%d: %02x %02x %02x %02x])\n", buf, type, len, msglen, xtype, data[1], data[2], data[3], data[4]);
						done=1;
					}
				} else
					done=1;
				break;
			default:
				fprintf(stderr, "%s unknown packet type %x [len %d]\n", buf, *data, len);
				{
					int i;
					for(i=0;i<len;i++)
						fprintf(stderr, "%02x ",data[i]);
				}
				c->state=STATE_UNSUPPORTED;
				break;
			}
			break;
		case STATE_HTTP:
			if (!server) {
				fprintf(stderr, "%s HTTP request:\n", buf);
				fwrite(data, len, 1, stderr);
				read=len;
				done=1;
			} else {
				int i;
				for(i=0;i<len;i++) {
					if ((i<=len-4 && !memcmp(data+i, "\r\n\r\n", 4)) ||
					    (i<=len-2 && !memcmp(data+i, "\n\n", 2))) {
						fprintf(stderr, "%s HTTP reply:\n", buf);
						fwrite(data, i+2, 1, stderr);
						c->state=STATE_UNSUPPORTED; /* ignore the body */
						read=i+2;
						done=1;
						break;
					}
				}
				done=1;
			}	
			break;
		case STATE_PUSH:
			fprintf(stderr, "%s PUSH\n", buf);
			print_bin_data (data, len);
			read=len;
			done=1;
			break;
		default:
			/* fprintf(stderr, "%s %d skipped\n", buf,len); */
			done_read=-1;
			done=1;
		}
		this_len-=read;
		len-=read;
		data+=read;
		done_read+=read;
		if (len<0)
			fprintf(stderr, "WARNING: len=%d, this_len=%d\n", len, this_len);
		} while (!done && len>0);
		if (done_read>=0)
			nids_discard(tcp, done_read);

	}
	}
	return;
}

void udp_callback(struct tuple4 *addr, char *buf, int len, void* iph) {
	if (verify_port (addr->dest)) {
		fprintf(stderr, "%s [UDP]\n", adres (addr, 0));
		print_bin_data (buf, len);
	}
}

void syslog (int type, int errnum, void *iph, void *data) {
	/* TODO */
}

int main (int argc, char **argv) {
	if (argc>1)
		nids_params.device=argv[1];

	nids_params.syslog=syslog; /* don't flood syslog */

	if (!nids_init ()) {
		fprintf(stderr,"%s\n",nids_errbuf);
		exit(1);
	}
	nids_register_tcp (tcp_callback);
	nids_register_udp (udp_callback);
	nids_run ();
	return 0;
}
