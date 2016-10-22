HOMEPATH = $(HOME)/tang/
FLDINC = $(HOME)/tang/src/folder/inc
DCSINC = $(HOME)/tang/src/fep/hst/inc
ALLINC=  -I$(FLDINC) -I$(DCSINC) -I$(HOME)/tang/src/fep/shttpd/inc -I$(HOME)/tang/src/fep/include -I$(ORACLE_HOME)/precomp/public/
ORALIB = $(ORACLE_HOME)/lib/libsqlplus.a $(ORACLE_HOME)/lib/libclntsh.so
LIB= -L$(HOME)/tang/src/lib -ldcsutil -ltimer -lsecu -liso8583 -lfold -lnsl  -lcurl -lssl -ldl \
	$(HOME)/tang/src/lib/glbdb.a \
	$(HOME)/tang/src/lib/libdb.a \
	$(HOME)/tang/src/lib/libshttpd.a 
CFLAGS=-O $(ALLINC)

OBJS = ylqzmain.o ylqzfunc.o base64.o appfunc.o 

.c.o:
	$(CC)  -c $(CFLAGS) $*.c

all:  ylqzapp ylqzshttp cleanup

app:  ylqzapp cleanup

shttp:  ylqzshttp cleanup

ylqzapp: $(OBJS) 
	$(CC) -o ylqzapp $(CFLAGS) $(OBJS) $(ALLINC) $(ORALIB) $(LIB) 
	mv ylqzapp  $(ICS_HOME)/bin 
	
ylqzshttp: ylqzshttp.o appfunc.o 
	$(CC) -o ylqzshttp $(CFLAGS) ylqzshttp.o appfunc.o $(ALLINC) $(ORALIB) $(LIB) 
	mv ylqzshttp  $(ICS_HOME)/bin 	

cleanup:
	rm -f *.o

