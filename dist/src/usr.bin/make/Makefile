# $MirOS: src/usr.bin/make/Makefile,v 1.17 2007/06/21 14:17:06 tg Exp $
# $OpenBSD: Makefile,v 1.40 2007/03/18 15:37:06 mickey Exp $

.include <bsd.own.mk>

PROG=		make
CDIAGFLAGS+=	-Wall -W -Wno-char-subscripts -Wstrict-prototypes -pedantic \
		-Wmissing-prototypes -Werror -Wno-cast-qual
#CPPFLAGS+=	-DHAS_STATS	# Throw out statistics after running
CPPFLAGS+=	-DMACHINE=\"${MACHINE}\" -DMACHINE_ARCH=\"${MACHINE_ARCH}\"
CPPFLAGS+=	-I. -I${.CURDIR}

SRCS=	arch.c buf.c cmd_exec.c compat.c cond.c dir.c error.c for.c \
	init.c job.c lowparse.c main.c make.c memory.c parse.c \
	parsevar.c str.c stats.c suff.c targ.c timestamp.c \
	var.c varmodifiers.c varname.c
SRCS+=	lstAddNew.c lstAppend.c lstConcat.c lstConcatDestroy.c \
	lstDeQueue.c lstDestroy.c lstDupl.c lstFindFrom.c lstForEachFrom.c \
	lstInsert.c lstMember.c lstRemove.c lstReplace.c lstSucc.c
.PATH:	${.CURDIR}/lst.lib

.ifdef MAKE_BOOTSTRAP
MKFEATURES?=	-D_PATH_DEFSYSPATH=\"/usr/share/mk\"
CPPFLAGS+=	-DMACHINE_OS=\"${MACHINE_OS}\" -DMAKE_BOOTSTRAP
.else
MKFEATURES?=	-DUSE_TIMESPEC
.endif
CPPFLAGS+=	${MKFEATURES} -D_PATH_MIRBSDKSH=\"${MKSH}\" -DIN_MIRMAKE

.if !exists(${MKSH})
.BEGIN:
	@echo 'You know, $${MKSH} (${MKSH}) should exist!'
	@exit 1
.endif

make.1: ${.CURDIR}/make.1
	sed -e 's#@@MKSH@@#${MKSH}#g' <$> >$@

CLEANFILES+=	${LIBOBJS} check condhashconsts.h generate.o generate \
		hashconsts.h libohash.a regress.o varhashconsts.h
.if ${.OBJDIR} != ${.CURDIR}
CLEANFILES+=	make.1
.endif

beforedepend: varhashconsts.h condhashconsts.h
# may need tweaking if you add variable synonyms or change the hash function
MAGICVARSLOTS=	82
MAGICCONDSLOTS=	65

varhashconsts.h: generate
	${.OBJDIR}/generate 1 ${MAGICVARSLOTS} >$@

condhashconsts.h: generate
	${.OBJDIR}/generate 2 ${MAGICCONDSLOTS} >$@

generate: generate.c stats.c memory.c ${OHASH_SRCS}
	${HOSTCC} ${LDSTATIC} -o $@ ${HOSTCFLAGS} ${CPPFLAGS} $> ${LDADD}

check: regress.o str.o memory.o buf.o
	${CC} -o $@ ${CFLAGS} $> ${LDADD}

regress: check
	${.OBJDIR}/check

.if make(install)
SUBDIR+= PSD.doc
.endif

.PHONY: regress

.include <bsd.prog.mk>
