SRCS=		string.c shttpd.c log.c auth.c cgi.c io_ssi.c \
		io_file.c io_socket.c io_ssl.c io_emb.c io_dir.c io_cgi.c
HDRS=		defs.h llist.h shttpd.h std_includes.h io.h ssl.h \
		compat_unix.h compat_win32.h compat_rtems.h config.h
OBJS=		$(SRCS:%.c=%.o) compat_unix.c
PROG=		shttpd
LIBS=		-lpthread -lnsl -ldl -lssl
HOMEPATH = $(HOME)/tang/
APPLIBPATH = $(HOMEPATH)src/lib/
DCSINC = -I$(HOME)/tang/src/fep/shttpd/inc

CL_FLAGS=	/MD /TC /nologo /DNDEBUG /Os 	# MSVC compiler flags


.c.o:
	$(CC) -g -c $(CFLAGS) ${DCSINC} $< -o $@

lib$(PROG).a: $(OBJS) compat_unix.o
	$(AR) -r $(APPLIBPATH)lib$(PROG).a $(OBJS) compat_unix.o 

shttpd:lib$(PROG).a 
	$(CC) -g $(CFLAGS) ${DCSINC} standalone.c -o $(PROG) $(LIBS) $(APPLIBPATH)lib$(PROG).a

clean:
	rm -rf *.o 

all: libshttpd.a shttpd clean
