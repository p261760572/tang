include  ../../make/makefile.defines

OBJ= util_conf.o  util_misc.o util_signal.o util_command.o

.c.o:
	$(CC)   -c $(CFLAGS) $*.c
	$(AR) $(ARFLAGS) $(FLDLIB) $*.o

obj:    $(OBJ)  cleanup
all:    obj

cleanup:
	rm  -f  *.o
