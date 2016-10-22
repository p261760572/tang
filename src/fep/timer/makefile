CFLAGS=-I $(ICS_SRCPATH)/fep/include -I $(ICS_SRCPATH)/folder/inc
LIB   =$(ICS_SRCPATH)/lib/libtimer.a
LIBS=$(LIB) -L$(ICS_SRCPATH)/lib -lfold -ldcsutil -ltimer
BIN   =$(ICS_HOME)/bin

OBJ =  tm_init.o  tm_insert.o tm_lock.o tm_queue.o tm_view.o tm_key.o

EXE = timersvr

.c.o:
	$(CC)  -c $(CFLAGS) $*.c
	$(AR) $(ARFLAGS) $(LIB) $*.o

obj:    $(OBJ) cleanup
exe:    $(EXE)
all:    obj exe

cleanup:
	rm -f *.o

timersvr: timersvr.c
	$(CC)  -o  $(BIN)/timersvr $(CFLAGS) timersvr.c $(LIBS)
