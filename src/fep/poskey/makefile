#LIB =$(HOME)/tang/src/lib/libdcs.a
LIBS=$(LIB) -L$(HOME)/tang/src/lib  -ldcsutil -ltimer -lsecu -liso8583 -lfold
BIN =$(HOME)/run/bin
CFLAGS=-I $(HOME)/tang/src/fep/include -I $(HOME)/tang/src/folder/inc -I $(HOME)/tang/src/fep/hst/inc  -I.
ORACLE_LIB= -L$(ORACLE_HOME)/lib -lsqlplus -lclntsh
OBJ =appCons.o  publicfn.o dssappfn.o dssdclfn.o \
			pc_proc.o \
			pos_proc.o \
			desc.o
EXE = poskey 

.c.o:
	$(CC) -c $(CFLAGS) $*.c

obj:    $(OBJ) cleanup
exe:    $(EXE) cleanup
all:    $(OBJ) $(EXE) cleanup

dssdclfn.c:dssdclfn.pc
	proc dssdclfn.pc dssdclfn.c  unsafe_null=yes   mode=oracle dbms=v8 include=$(HOME)/tang/src/fep/include  include=$(HOME)/src/folder/inc

dssappfn.c:dssappfn.pc
	proc dssappfn.pc dssappfn.c  unsafe_null=yes   mode=oracle dbms=v8 include=$(HOME)/tang/src/fep/include  include=$(HOME)/src/folder/inc

poskey: $(OBJ)
	$(CC) -o poskey   $(CFLAGS) $(OBJ) $(LIBS) $(ORACLE_LIB)
	strip poskey 2>/dev/null
#	mv poskey $(BIN)/poskey
cleanup:
	rm -f *.o dssdclfn.c dssappfn.c 


