/*	$OpenBSD: error.c,v 1.12 2004/04/07 13:11:35 espie Exp $ */

/*
 * Copyright © 2013
 *	Thorsten “mirabilos” Glaser <tg@mirbsd.org>
 * Copyright (c) 2001 Marc Espie.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE OPENBSD PROJECT AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OPENBSD
 * PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "config.h"
#include "defines.h"
#include "error.h"
#include "job.h"
#include "targ.h"

#include "lowparse.h"

__RCSID("$MirOS: src/usr.bin/make/error.c,v 1.4 2013/10/31 20:07:06 tg Exp $");

int	    fatal_errors = 0;
static void ParseVErrorInternal(const char *, unsigned long, int,
    const char *, va_list) __attribute__((__format__(__printf__, 4, 0)));
/*-
 * Error --
 *	Print an error message given its format.
 */
/* VARARGS */
void
Error(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	(void)vfprintf(stderr, fmt, ap);
	va_end(ap);
	(void)fprintf(stderr, "\n");
}

/*-
 * Fatal --
 *	Produce a Fatal error message. If jobs are running, waits for them
 *	to finish.
 *
 * Side Effects:
 *	The program exits
 */
/* VARARGS */
void
Fatal(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	Job_Wait();

	(void)vfprintf(stderr, fmt, ap);
	va_end(ap);
	(void)fprintf(stderr, "\n");

	if (DEBUG(GRAPH2))
		Targ_PrintGraph(2);
	exit(2);		/* Not 1 so -q can distinguish error */
}

/*
 * Punt --
 *	Major exception once jobs are being created. Kills all jobs, prints
 *	a message and exits.
 *
 * Side Effects:
 *	All children are killed indiscriminately and the program Lib_Exits
 */
/* VARARGS */
void
Punt(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	(void)fprintf(stderr, "make: ");
	(void)vfprintf(stderr, fmt, ap);
	va_end(ap);
	(void)fprintf(stderr, "\n");

	DieHorribly();
}

/*-
 * DieHorribly --
 *	Exit without giving a message.
 *
 * Side Effects:
 *	A big one...
 */
void
DieHorribly(void)
{
	Job_AbortAll();
	if (DEBUG(GRAPH2))
		Targ_PrintGraph(2);
	exit(2);		/* Not 1, so -q can distinguish error */
}

/*
 * Finish --
 *	Called when aborting due to errors in child shell to signal
 *	abnormal exit.
 *
 * Side Effects:
 *	The program exits
 */
void
Finish(int errors) /* number of errors encountered in Make_Make */
{
	Fatal("%d error%s", errors, errors == 1 ? "" : "s");
}


/*-
 * ParseVErrorInternal	--
 *	Error message abort function for parsing. Prints out the context
 *	of the error (line number and file) as well as the message with
 *	two optional arguments.
 *
 * Side Effects:
 *	"fatals" is incremented if the level is PARSE_FATAL.
 */
/* VARARGS */
static void
ParseVErrorInternal(const char *cfname, unsigned long clineno, int type,
	const char *fmt, va_list ap)
{
	if (cfname)
	    (void)fprintf(stderr, "\"%s\", line %lu: ", cfname, clineno);
	if (type == PARSE_WARNING)
		(void)fprintf(stderr, "warning: ");
	(void)vfprintf(stderr, fmt, ap);
	va_end(ap);
	(void)fprintf(stderr, "\n");
	if (type == PARSE_FATAL)
		fatal_errors ++;
}

/*-
 * Parse_Error	--
 *	External interface to ParseVErrorInternal; uses the default filename
 *	Line number.
 */
/* VARARGS */
void
Parse_Error(int type, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	ParseVErrorInternal(Parse_Getfilename(), Parse_Getlineno(), type,
	    fmt, ap);
	va_end(ap);
}
