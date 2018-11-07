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

static void usage()
{
	fprintf(stderr, "usage: %s [-u url]\n", getprogname()); 
	exit(1);
}

int main(int argc, char *argv[])
{
	char *url = "", *host = "", *path = "/";
/*	char *cafile = get_path_ca(); */
/*	int i; */
	size_t i;
/*	http_request *request = NULL; */
	size_t httphsz = 0; 
	char    *ip;
	struct httphead	*httph = NULL;
	struct httpget *hget;
	short port;
/*	if (pledge("stdio inet rpath dns", NULL) == -1)
		err(1, "pledge");
*/
/*	if (unveil("/tmp", "w") == -1)
		err(1, "unveil");
	if (unveil(cafile, "r") == -1)
		err(1, "unveil");	
*/
	if(argc < 2){
		usage();
	}
	if(argv[2])
		url=argv[2];

	if ((host = url2host(url, &port, &path)) == NULL)
		errx(1, "url2host failed");
	if (*path == '\0')
		path = "/";

/*	char *upath; */
/*	memcpy(upath, path, strlen(path+1)); */
/*	printf("Path: %s", upath); */
	ip = lookup_host(host);
	printf("Host: %s (%s)\tPort: %d\tPath: %s\n", host, ip, port, path);

/*		hget = http_get(sources, rescount, host, port, upath,
		    request->body, postsz);
*/
	hget = http_get(ip, host, port, path);
	if (hget == NULL)
		errx(1, "http_get");
/*	free(ip); */
	printf("Server at %s returns:\n", host);
	for (i = 0; i < httphsz; i++)
		printf("[%s]=[%s]\n", httph[i].key, httph[i].val);
	printf("	  [Body]=[%zu bytes]\n", hget->bodypartsz);
	printf("Body: %s\n", hget->bodypart); 
	if (hget->bodypartsz <= 0)
			errx(1, "No body in reply from %s", host);
	if (hget->code != 200)
			errx(1, "http reply code %d from %s", hget->code, host);

}
