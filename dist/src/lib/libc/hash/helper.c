/**	$MirOS: src/lib/libc/hash/helper.c,v 1.6 2010/01/07 22:34:51 tg Exp $ */
/*	$OpenBSD: helper.c,v 1.8 2005/08/08 08:05:35 espie Exp $	*/

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <phk@login.dkuug.dk> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Poul-Henning Kamp
 * ----------------------------------------------------------------------------
 */

#include <sys/param.h>
#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <hashinc>

__RCSID("$MirOS: src/lib/libc/hash/helper.c,v 1.6 2010/01/07 22:34:51 tg Exp $ helper for HASH hash");

extern const uint8_t mbsd_digits_hex[17];
extern const uint8_t mbsd_digits_HEX[17];

/* ARGSUSED */
char *
HASHEnd(HASH_CTX *ctx, char *buf)
{
	int i;
	u_int8_t digest[HASH_DIGEST_LENGTH];
#ifdef HASH_DIGEST_UPPERCASE
#define hex mbsd_digits_HEX
#else
#define hex mbsd_digits_hex
#endif

	if (buf == NULL && (buf = malloc(HASH_DIGEST_STRING_LENGTH)) == NULL)
		return (NULL);

	HASHFinal(digest, ctx);
	for (i = 0; i < HASH_DIGEST_LENGTH; i++) {
		buf[i + i] = hex[digest[i] >> 4];
		buf[i + i + 1] = hex[digest[i] & 0x0f];
	}
	buf[i + i] = '\0';
	memset(digest, 0, sizeof(digest));
	return (buf);
}

char *
HASHFileChunk(const char *filename, char *buf, off_t off, off_t len)
{
	struct stat sb;
	u_char buffer[BUFSIZ];
	HASH_CTX ctx;
	int fd, save_errno;
	ssize_t nr;

	HASHInit(&ctx);

	if ((fd = open(filename, O_RDONLY)) < 0)
		return (NULL);
	if (len == 0) {
		if (fstat(fd, &sb) == -1) {
			close(fd);
			return (NULL);
		}
		len = sb.st_size;
	}
	if ((len < 0) || (off > 0 && lseek(fd, off, SEEK_SET) < 0))
		return (NULL);

	while ((nr = read(fd, buffer,
	    (size_t)(len ? MIN(BUFSIZ, len) : BUFSIZ))) > 0) {
		HASHUpdate(&ctx, buffer, (size_t)nr);
		if (len > 0 && (len -= nr) == 0)
			break;
	}

	save_errno = errno;
	close(fd);
	errno = save_errno;
	return (nr < 0 ? NULL : HASHEnd(&ctx, buf));
}

char *
HASHFile(const char *filename, char *buf)
{
	return (HASHFileChunk(filename, buf, (off_t)0, (off_t)0));
}

char *
HASHData(const u_char *data, size_t len, char *buf)
{
	HASH_CTX ctx;

	HASHInit(&ctx);
	HASHUpdate(&ctx, data, len);
	return (HASHEnd(&ctx, buf));
}
