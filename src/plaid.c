/*
 * Copyright (c) 2018 int16h <int16h@openbsd.space>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <sys/socket.h>
#include <sys/stat.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <err.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "http.h"
#include "util.h"
#include "gui.h"

extern int stripHTMLTags(char *, size_t);
extern void gui_init();

/* #define DIR_MODE	(S_IRUSR | S_IWUSR | S_IXUSR | 
	S_IRGRP | S_IWGRP | S_IXGRP) */

char	*url = "", *host = "", *path = "/";
char	*ip;
char	sfn[28];

int	fd;
size_t	i;
size_t	httphsz = 0;
short	*port;

FILE * sfp;

struct	httphead *httph = NULL;
struct	httpget *hget;
struct	hostent *hp;

static void
usage()
{
	fprintf(stderr, "usage: %s [-u url]\n", getprogname()); 
	exit(1);
}

static void
plaid_host(char *url)
{
	if ((host = url2host(url, &port, &path)) == NULL)
		errx(1, "url2host failed");
	if (*path == '\0')
		path = "/";
	hp = gethostbyname(host);
	if (hp == NULL)
		errx(1, "gethostbyname");
	ip = inet_ntoa( *( struct in_addr*)( hp -> h_addr_list[0]));
	printf("Host: %s (%s)\tPort: %d\tPath: %s\n", host, ip, port, path);
}

static void
plaid_http(char *host, char *ip, short *port, char *path)
{
	hget = http_get(ip, host, port, path);
	if (hget == NULL)
		errx(1, "http_get");
	printf("Server at %s returns:\n", host);
	for (i = 0; i < httphsz; i++)
                printf("[%s]=[%s]\n", httph[i].key, httph[i].val); 
	printf("BodySz: [%zu bytes]\n", hget->bodypartsz);
char *nhtml = malloc(hget->bodypartsz+1);
nhtml = hget->bodypart;
/* printf("Body: %s\n", nhtml); */

int htconv = stripHTMLTags(nhtml, strlen(nhtml));
/* printf("Body: %s\n", nhtml); */
/* printf("htconv: %d\n", htconv); */
/*      printf("Body: %s\n", stripHTMLTags(hget->bodypart, hget->bodypartsz)); */
/*      printf("Body: %s\n", hget->bodypart); */
/*      printf("Body: %s\n", hget->headpart); */

	return;
}

static void
plaid_body()
{
	int n;
	int p;
	if (hget->bodypartsz <= 0)
		errx(1, "No body in reply from %s", host);
/*	mkdir("/tmp/plaid", DIR_MODE); */

	if(n = (strlcpy(sfn, "/tmp/plaid.XXXXXXXXXX", sizeof(sfn)) == NULL))
		errx(1, "strlcpy");

//	strlcpy(sfn, "/tmp/plaid.XXXXXXXXXX", sizeof(sfn));
/*        if (unveil(sfn, "rcw") == -1)
 *               err(1, "unveil");
 */	

	if ((fd = mkstemp(sfn)) == -1 || 
		(sfp = fdopen(fd, "w+")) == NULL) {
		if (fd != -1) {
			unlink(sfn);
			close(fd);
		}
                warn("%s", sfn);
	}

//	char sanitata = hget->bodypart;
// char sanitata = stripHTMLTags(hget->bodypart,sizeof(bodypart);

	if(p = fprintf(sfp, "%s", hget->bodypart) == -1)
		errx(1, "fprintf: %d chars written", p);
	fclose(sfp);
	printf("sfn: %s\n", sfn);
// gui_init(sfn);	
}

int 
main(int argc, char *argv[])
{
	/* tinfoilhat(); */
	if(argc < 2){
		usage();
	}
	if(argv[2])
		url=argv[2];
	
	plaid_host(url);
	plaid_http(host, ip, port, path);
                if (hget->code != 200) {
                        if(hget->code == 301) {
                                size_t   i;
                                char *v = "Location";
                                for (i = 0; i < hget->headsz; i++) {
                                        if (!strcmp(hget->head[i].key, v))
                                        {
                                         printf("redirecting to: %s\n",
                                                hget->head[i].val);
                                         plaid_host(hget->head[i].val);
                                         plaid_http(host, ip, port, path);
                                        }
                                }
                        }else{
                                errx(1, "%d from %s", hget->code, host);
                        }
                }

	plaid_body();
	gui_init(sfn);
}
