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

char *
url2host(const char *host, short *port, char **path)
{
        char    *url, *ep;
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
/*
	char *upath;
	memcpy(upath, path, strlen(path+1));
*/
        return (url);
}
