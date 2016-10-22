HOMEPATH = $(HOME)/tang/
FLDINC = $(HOME)/tang/src/folder/inc
DCSINC = $(HOME)/tang/src/fep/hst/inc
ALLINC=  -I$(FLDINC) -I$(DCSINC) -I$(HOME)/tang/src/fep/db/inc -I$(HOME)/tang/src/fep/shttpd/inc -I$(HOME)/tang/src/fep/include -I$(ORACLE_HOME)/precomp/public/
ORALIB = $(ORACLE_HOME)/lib/libsqlplus.a $(ORACLE_HOME)/lib/libclntsh.so
LIB= -L$(HOME)/tang/src/lib -ldcsutil -ltimer -lsecu -liso8583 -lfold -lnsl  -lcurl -lssl -ldl \
	$(HOME)/tang/src/lib/glbdb.a \
	$(HOME)/tang/src/lib/libdb.a \
	$(HOME)/tang/src/lib/libshttpd.a \
	$(HOME)/tang/src/lib/libnpc.a
CFLAGS=-O $(ALLINC)

OBJS =smsmain.o smsfunc.o base64.o appfunc.o 

SOBJS = smsfunc.o smsshttp.o base64.o appfunc.o

.c.o:
	$(CC)  -c $(CFLAGS) $*.c

all:  smsapp smsshttp cleanup

app:  smsapp cleanup

shttp: smsshttp cleanup

smsapp: $(OBJS) 
	$(CC) -o smsapp $(CFLAGS) $(OBJS) $(ALLINC) $(ORALIB) $(LIB) 
	mv smsapp  $(ICS_HOME)/bin 
	
smsshttp: $(SOBJS)
	$(CC) -o smsshttp $(CFLAGS) $(SOBJS) $(ALLINC) $(ORALIB) $(LIB) 
	mv smsshttp  $(ICS_HOME)/bin 	

cleanup:
	rm -f *.o

