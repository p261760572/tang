HOMEPATH = $(HOME)/tang/
FLDINC = $(HOME)/tang/src/folder/inc
DCSINC = $(HOME)/tang/src/fep/hst/inc
ALLINC=  -I$(FLDINC) -I$(DCSINC)  -I$(HOME)/tang/src/fep/include -I$(ORACLE_HOME)/precomp/public/
ORALIB = $(ORACLE_HOME)/lib/libsqlplus.a $(ORACLE_HOME)/lib/libclntsh.so
LIB= -L$(HOME)/tang/src/lib -ldcsutil -liso8583 -ltimer -lsecu -lfold -lnsl\
	$(HOME)/tang/src/lib/libsecu.a \
	$(HOME)/tang/src/lib/glbdb.a \
	$(HOME)/tang/src/lib/libdb.a 
CFLAGS=-O $(ALLINC)
OBJS = appmain.o appfunc.o genkey.o ivrSignIn.o

.c.o:
	$(CC)  -c $(CFLAGS) $*.c

EXE = appivrsvr ivrSignIn


all:  $(EXE) cleanup

appivrsvr:	appmain.o appfunc.o
		$(CC)   -o appivrsvr  $(CFLAGS) appmain.o appfunc.o $(ALLINC)  $(ORALIB) $(LIB) 
		mv appivrsvr  $(ICS_HOME)/bin 

ivrSignIn: ivrSignIn.o 
	$(CC) -o $(ICS_HOME)/bin/ivrSignIn $(CFLAGS) ivrSignIn.o $(ALLINC) $(ORALIB) $(LIB) 

genk: appfunc.o genkey.o
	$(CC) -o genk $(CFLAGS) genkey.o appfunc.o $(ALLINC) $(ORALIB) $(LIB) 

cleanup:
		rm -f *.o
