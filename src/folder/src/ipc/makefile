include ../../make/makefile.defines
OBJ= ipc_makekey.o  ipc_msg.o  ipc_sem.o ipc_shm.o ipc_fmap.o 

.c.o:
	$(CC)   -c $(CFLAGS) $*.c
	$(AR) $(ss) $(ARFLAGS) $(FLDLIB) $*.o

obj:    $(OBJ)  cleanup
all:    obj

cleanup:
	rm  -f  *.o
