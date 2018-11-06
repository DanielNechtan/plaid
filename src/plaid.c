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

#define MAX_SERVERS_DNS 8

struct addr {
	int	 family; /* 4 for PF_INET, 6 for PF_INET6 */
	char	 ip[INET6_ADDRSTRLEN];
};

static ssize_t
host_dns(const char *s, struct addr vec[MAX_SERVERS_DNS])
{
	struct addrinfo		 hints, *res0, *res;
	int			 error;
	ssize_t			 vecsz;
	struct sockaddr		*sa;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM; /* DUMMY */

	error = getaddrinfo(s, NULL, &hints, &res0);

	if (error == EAI_AGAIN ||
#ifdef EAI_NODATA
	    error == EAI_NODATA ||
#endif
	    error == EAI_NONAME)
		return 0;

	if (error) {
		warnx("%s: parse error: %s", s, gai_strerror(error));
		return -1;
	}

	for (vecsz = 0, res = res0;
	    res != NULL && vecsz < MAX_SERVERS_DNS;
	    res = res->ai_next) {
		if (res->ai_family != AF_INET &&
		    res->ai_family != AF_INET6)
			continue;

		sa = res->ai_addr;

		if (res->ai_family == AF_INET) {
			vec[vecsz].family = 4;
			inet_ntop(AF_INET,
			    &(((struct sockaddr_in *)sa)->sin_addr),
				vec[vecsz].ip, INET6_ADDRSTRLEN);
		} else {
			vec[vecsz].family = 6;
			inet_ntop(AF_INET6,
			    &(((struct sockaddr_in6 *)sa)->sin6_addr),
			    vec[vecsz].ip, INET6_ADDRSTRLEN);
		}

	/*	dspew("DNS returns %s for %s\n", vec[vecsz].ip, s); */
		printf("DNS returns %s for %s\n", vec[vecsz].ip, s);
		vecsz++;
		break;
	}

	freeaddrinfo(res0);
	return vecsz;
}

/*
 * Extract the domain and port from a URL.
 * The url must be formatted as schema://address[/stuff].
 * This returns NULL on failure.
 */
static char *
url2host(const char *host, short *port, char **path)
{
	char	*url, *ep;

	/* We only understand HTTP and HTTPS. */

	if (strncmp(host, "https://", 8) == 0) {
		*port = 443;
		if ((url = strdup(host + 8)) == NULL) {
			warn("strdup");
			return (NULL);
		}
	} else if (strncmp(host, "http://", 7) == 0) {
		*port = 80;
		if ((url = strdup(host + 7)) == NULL) {
			warn("strdup");
			return (NULL);
		}
	} else {
		warnx("%s: unknown schema", host);
		return (NULL);
	}

	/* Terminate path part. */

	if ((ep = strchr(url, '/')) != NULL) {
		*path = strdup(ep);
		*ep = '\0';
	} else
		*path = strdup("");

	if (*path == NULL) {
		warn("strdup");
		free(url);
		return (NULL);
	}

	return (url);
}
static void usage()
{
	fprintf(stderr, "usage: %s [-u url]\n", getprogname()); 
	exit(1);
}

int main(int argc, char *argv[])
{
char *url = "", *host = "", *cafile = get_path_ca(), *path = "/";
struct addr addrs[MAX_SERVERS_DNS] = {{0}};
struct source sources[MAX_SERVERS_DNS];
int i, ch, staplefd = -1, infd = -1, nonce = 1;
size_t rescount, httphsz = 0; 
struct httphead	*httph = NULL;
struct httpget *hget;
ssize_t written, w;
short port;
/*	if (pledge("stdio inet rpath dns", NULL) == -1)
		err(1, "pledge");
*/	if (unveil("/tmp", "w") == -1)
		err(1, "unveil");
	if (unveil(cafile, "r") == -1)
		err(1, "unveil");	
if(argc < 2){
	usage();
}
if(argv[2])
	url=argv[2];

printf("URL: %s\t", url);
printf("CA: %s\n", cafile);
if ((host = url2host(url, &port, &path)) == NULL)
	errx(1, "url2host failed");
if (*path == '\0')
		path = "/";
printf("Host: %s\tPort: %d\tPath: %s\n", host, port, path);
		rescount = host_dns(host, addrs);
		for (i = 0; i < rescount; i++) {
			sources[i].ip = addrs[i].ip;
			sources[i].family = addrs[i].family;
		}
}
