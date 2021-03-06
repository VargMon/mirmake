/* $MirOS: src/usr.bin/make/cond_int.h,v 1.3 2005/11/17 20:58:17 tg Exp $ */
/* $OpenBSD: cond_int.h,v 1.3 2002/06/11 21:12:11 espie Exp $ */

/* List of all keywords recognized by the make parser */
#define COND_IF		"if"
#define COND_IFDEF	"ifdef"
#define COND_IFNDEF	"ifndef"
#define COND_IFMAKE	"ifmake"
#define COND_IFNMAKE	"ifnmake"
#define COND_ELSE	"else"
#define COND_ELIF	"elif"
#define COND_ELIFDEF	"elifdef"
#define COND_ELIFNDEF	"elifndef"
#define COND_ELIFMAKE	"elifmake"
#define COND_ELIFNMAKE	"elifnmake"
#define COND_ENDIF	"endif"
#define COND_FOR	"for"
#define COND_ENDFOR	"endfor"
#define COND_INCLUDE	"include"
#define COND_UNDEF	"undef"
#define COND_UERR	"error"
#define COND_TRACE	"trace"
