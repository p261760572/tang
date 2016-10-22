include ../../make/makefile.defines

OBJ= mbuf_create.o  mbuf_alloc.o mbuf_access.o mbuf_lock.o mbuf_view.o

.c.o:
	$(CC)   -c $(CFLAGS) $*.c
	$(AR) $(ARFLAGS) $(FLDLIB) $*.o

all:    $(OBJ)  cleanup
obj:    $(OBJ)  cleanup

cleanup:
	rm  -f  *.o
