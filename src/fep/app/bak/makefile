HOMEPATH = $(HOME)/tang/
FLDINC = $(HOME)/tang/src/folder/inc
DCSINC = $(HOME)/tang/src/fep/hst/inc
ALLINC=  -I$(FLDINC) -I$(DCSINC)  -I$(HOME)/tang/src/fep/include -I$(ORACLE_HOME)/precomp/public/
ORALIB = $(ORACLE_HOME)/lib/libsqlplus.a $(ORACLE_HOME)/lib/libclntsh.so
LIB= -L$(HOME)/tang/src/lib -ldcsutil -liso8583 -ltimer -lsecu -lfold -lnsl  \
	$(HOME)/tang/src/lib/libsecu.a \
	$(HOME)/tang/src/lib/glbdb.a \
	$(HOME)/tang/src/lib/libdb.a 
CFLAGS=-O $(ALLINC)
OBJS = appmain.o appfunc.o BusT00.o BusZ00.o BusZ22.o CMDI.o BusGZ_Z22.o IVRFunc.o BusHF_Z22.o BusSMS_Z22.o cups.o posp.o 

.c.o:
	$(CC)  -c -gstabs+ $(CFLAGS) $*.c

EXE = appsvr notiscanner timescanner AJSignIn GDUnionPSignIn JAVASignIn GetKey gdyltest


all:  $(EXE) cleanup

appsvr:	$(OBJS)
		$(CC)   -o appsvr  $(CFLAGS) $(OBJS) $(ALLINC)  $(ORALIB) $(LIB) 

		mv appsvr  $(ICS_HOME)/bin 

GetKey:GetKey.o appfunc.o
	$(CC) -o $(ICS_HOME)/bin/GetKey $(CFLAGS) GetKey.o appfunc.o $(ALLINC) $(ORALIB) $(LIB) 

AJSignIn: AJSignIn.o appfunc.o
	$(CC) -o $(ICS_HOME)/bin/AJSignIn $(CFLAGS) AJSignIn.o appfunc.o $(ALLINC) $(ORALIB)  $(LIB) 

GDUnionPSignIn: GDUnionPSignIn.o appfunc.o
	$(CC) -o $(ICS_HOME)/bin/GDUnionPSignIn $(CFLAGS) GDUnionPSignIn.o appfunc.o $(ALLINC) $(ORALIB) $(LIB) 

JAVASignIn: JAVASignIn.o appfunc.o
	$(CC) -o $(ICS_HOME)/bin/JAVASignIn $(CFLAGS) JAVASignIn.o appfunc.o $(ALLINC) $(ORALIB) $(LIB) 

gdyltest:gdyltest.o appfunc.o 
	$(CC) -o $(ICS_HOME)/bin/gdyltest $(CFLAGS) gdyltest.o appfunc.o $(ALLINC) $(ORALIB) $(LIB) 

timescanner: 
	$(CC) -o $(ICS_HOME)/bin/timescanner $(CFLAGS) timescanner.c $(ORALIB) $(LIB)

notiscanner: 
	$(CC) -o $(ICS_HOME)/bin/notiscanner $(CFLAGS) notiscanner.c $(ORALIB) $(LIB)

cleanup:
		rm -f *.o
