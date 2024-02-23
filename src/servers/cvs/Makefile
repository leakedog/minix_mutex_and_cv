PROG=   cvs
SRCS=   main.c

DPADD+= ${LIBSYS}
DPADD+= ${LIBC}
LDADD+= -lsys -lc

MAN=

BINDIR?= /usr/sbin
FILES=cvs.conf
FILESNAME=cvs
FILESDIR= /etc/system.conf.d

.include <minix.service.mk>
