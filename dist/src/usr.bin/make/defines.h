#ifndef DEFINES_H
#define DEFINES_H

/**	$MirOS: src/usr.bin/make/defines.h,v 1.6 2006/08/26 21:29:00 tg Exp $ */
/*	$OpenBSD: defines.h,v 1.2 2002/02/19 19:39:38 millert Exp $ */

/*
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

#include <stdbool.h>

/* define common types in an opaque way */
struct GNode_;
typedef struct GNode_ GNode;

struct List_;
typedef struct List_ *Lst;

struct SymTable_;
typedef struct SymTable_ SymTable;

struct ohash;
typedef struct ohash GSymT;

struct Buffer_;
typedef struct Buffer_ *Buffer;

struct Name;

struct ListNode_;
typedef struct ListNode_ *LstNode;

/* some useful defines for gcc */

#ifdef __GNUC__
# define UNUSED	__attribute__((__unused__))
# define HAS_INLINES
# define INLINE  __inline__
#else
# define UNUSED
#endif

#ifdef HAS_INLINES
# ifndef INLINE
#  define INLINE	inline
# endif
#endif

/*
 * debug control:
 *	There is one bit per module.  It is up to the module what debug
 *	information to print.
 */
extern int debug;
#define DEBUG_ARCH	0x0001
#define DEBUG_COND	0x0002
#define DEBUG_DIR	0x0004
#define DEBUG_GRAPH1	0x0008
#define DEBUG_GRAPH2	0x0010
#define DEBUG_JOB	0x0020
#define DEBUG_MAKE	0x0040
#define DEBUG_SUFF	0x0080
#define DEBUG_TARG	0x0100
#define DEBUG_VAR	0x0200
#define DEBUG_FOR	0x0400
#define DEBUG_LOUD	0x0800

#define CONCAT(a,b)	a##b

#define DEBUG(module)	(debug & CONCAT(DEBUG_,module))

#ifndef	__RCSID
#define	__RCSID(x)	static const char __rcsid[] = x
#endif

#endif
