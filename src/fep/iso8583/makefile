CFLAGS=-I $(ICS_SRCPATH)/fep/include  -I $(ICS_SRCPATH)/folder/inc
LIB   =$(ICS_SRCPATH)/lib/libiso8583.a
BIN   =$(ICS_HOME)/bin

OBJ   = iso_8583.o iso_chkmsg.o iso8583_conf.o

.c.o:
	$(CC) -c  $(CFLAGS) $*.c
	$(AR) $(ARFLAGS) $(LIB) $*.o

obj:    $(OBJ) cleanup
all:    obj

cleanup:
	rm -f *.o
