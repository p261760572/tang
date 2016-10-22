LIB =$(ICS_SRCPATH)/lib/libdcsutil.a
LIBS=$(LIB) -L$(ICS_SRCPATH)/lib  -ldcsutil -lfold
BIN =$(ICS_HOME)/bin

CFLAGS=-I $(ICS_SRCPATH)/fep/include -I $(ICS_SRCPATH)/folder/inc

OBJ = dcs_log.o   dcs_globals.o dcs_shminit.o dcs_sysconf.o  tmsibbcda.o 

EXE = dcscons  tsvrcmd

.c.o:
	$(CC) -c -g $(CFLAGS) $*.c
	$(AR) $(ARFLAGS) $(LIB) $*.o 

obj:    $(OBJ) cleanup
exe:    $(EXE)
all:    obj  exe


dcscons: dcscons.c 
	$(CC)  -o $(BIN)/dcscons $(CFLAGS) dcscons.c  $(LIBS)
tsvrcmd: dcscmd.c 
	$(CC)  -o $(BIN)/tsvrcmd $(CFLAGS) dcscmd.c  $(LIBS)

cleanup:
	rm -f *.o
