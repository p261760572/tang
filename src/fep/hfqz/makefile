HOMEPATH = $(HOME)/tang/
FLDINC = $(HOME)/tang/src/folder/inc
DCSINC = $(HOME)/tang/src/fep/hst/inc
ALLINC=  -I$(FLDINC) -I$(DCSINC) -I$(HOME)/tang/src/fep/shttpd/inc -I$(HOME)/tang/src/fep/include -I$(ORACLE_HOME)/precomp/public/
ORALIB = $(ORACLE_HOME)/lib/libsqlplus.a $(ORACLE_HOME)/lib/libclntsh.so
LIB= -L$(HOME)/tang/src/lib -ldcsutil -ltimer -lsecu -liso8583 -lfold -lnsl  -lcurl -lssl -ldl \
	$(HOME)/tang/src/lib/glbdb.a \
	$(HOME)/tang/src/lib/libdb.a \
	$(HOME)/tang/src/lib/libshttpd.a \
	$(HOME)/tang/src/lib/libnpc.a
CFLAGS=-O $(ALLINC)

OBJS =hfqzmain.o hfqzfunc.o base64.o appfunc.o 

.c.o:
	$(CC)  -c $(CFLAGS) $*.c

all:  hfqzapp hfqzshttp cleanup

app:  hfqzapp cleanup

shttp: hfqzshttp cleanup

hfqzapp: $(OBJS) 
	$(CC) -o hfqzapp $(CFLAGS) $(OBJS) $(ALLINC) $(ORALIB) $(LIB) 
	mv hfqzapp  $(ICS_HOME)/bin 
	
hfqzshttp: hfqzshttp.o appfunc.o
	$(CC) -o hfqzshttp $(CFLAGS) hfqzshttp.o appfunc.o $(ALLINC) $(ORALIB) $(LIB) 
	mv hfqzshttp  $(ICS_HOME)/bin 	

cleanup:
	rm -f *.o

