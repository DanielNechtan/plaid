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

#include "../3rd/tidy-html5/include/tidy.h"
#include "../3rd/tidy-html5/include/tidybuffio.h"

// char *HTMLSrc;
char *nhtmlsrc;
TidyBuffer output = {0};
TidyBuffer errbuf = {0};
int rc = -1;
Bool ok;


struct html_text {
        char    *htsrc;
        char    *hthead;
        int     htalloc;
        int     htsz;
};

/* html_text* HTMLBody = 0; */

/* static TidyDoc tdoc; */

extern int
tidyhtml (char *HTMLsrc, char *tsfn)
{

TidyDoc tdoc = tidyCreate();
ok = tidyOptSetBool(tdoc, TidyXhtmlOut, yes);
if (ok)
	rc = tidySetErrorBuffer(tdoc, &errbuf);
if (rc >= 0)
	rc = tidyParseString(tdoc, HTMLsrc);
if ( rc >= 0 )
	rc = tidyCleanAndRepair(tdoc);
if ( rc >= 0 )
	rc = tidyRunDiagnostics(tdoc);
if ( rc > 1 )
	rc = ( tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1 );
if (rc >= 0)
        rc = tidyOptSetBool(tdoc, TidyWriteBack, yes);
if ( rc >= 0 )
	rc = tidyOptSetValue(tdoc, TidyOutFile, tsfn);
if ( rc >= 0 )
	rc = tidySaveBuffer( tdoc, &output );
if ( rc >= 0 )
        rc = tidySaveFile( tdoc, tsfn);
  if ( rc >= 0 )
  {
    if ( rc > 0 )
	printf("\n[DONE]\ttidyhtml()\n");
/*      printf( "\nDiagnostics:\n\n%s", errbuf.bp ); */
/*    printf( "\nresult:\n\n%s", output.bp ); */
  }
  else
    printf( "A severe error (%d) occurred.\n", rc );
  tidyBufFree( &output ); 
  tidyBufFree( &errbuf );
  tidyRelease( tdoc );
  return rc;
}
