LIBU = $(ICS_SRCPATH)/lib/libdcsutil.a
FLDINC = $(ICS_SRCPATH)/folder/inc
DCSINC = $(ICS_SRCPATH)/fep/hst/inc

ALLINC=  -I$(FLDINC) -I$(DCSINC)  
LIB= -L $(ICS_SRCPATH)/lib    -ldcsutil -ltimer -lfold -lnsl
LIBSO= -L $(ICS_SRCPATH)/lib    -ldcsutil  -lfold
LIBS= -L $(ICS_SRCPATH)/lib -lsecu -liso8583
CFLAGS=-O $(ALLINC)
LIBOBJ=  secuLib.o
#-D_PERFORMANCE_TEST_
.c.o:
	$(CC)  -c -g $(CFLAGS) $*.c

EXE = secuSrv secuCmd softjmj testjmj setkey test_queue effi_sec_srv
#EXE = secuSrv secuCmd 


all:  lib $(EXE) cleanup

secuSrv:	secuSrv.c dcssock.c 
		$(CC)  -pthread -o secuSrv $(CFLAGS) secuSrv.c dcssock.c $(LIB) 

		mv secuSrv  $(ICS_HOME)/bin 
secuCmd:	secuCmd.c dcssock.c
		$(CC)  -pthread -o secuCmd $(CFLAGS) secuCmd.c dcssock.c $(LIB) 
		mv secuCmd  $(ICS_HOME)/bin 
softjmj:	sj06e_soft.c dcssock.c desc.c sms4.c ascbcd.c util.c
		$(CC)  -o softjmj $(CFLAGS) sj06e_soft.c sms4.c dcssock.c desc.c ascbcd.c util.c $(LIBSO) 
softjmj_r:	sj06e_soft.c dcssock.c desc.c ascbcd.c util_r.c sms4.c
		$(CC)  -o softjmj_r $(CFLAGS) sj06e_soft.c dcssock.c sms4.c desc.c ascbcd.c util_r.c $(LIBSO) 
softjmj_wst:	sj06e_soft_wst.c dcssock.c desc.c sms4.c ascbcd.c util_wst.c
		$(CC)  -o softjmj_wst $(CFLAGS) sj06e_soft_wst.c sms4.c dcssock.c desc.c ascbcd.c util_wst.c $(LIBSO) 

setkey:	setIndexKey.c desc.c ascbcd.c
		$(CC) -o setkey $(CFLAGS) setIndexKey.c desc.c ascbcd.c
testjmj:	testjmj.c
		$(CC) -o testjmj $(CFLAGS) testjmj.c $(LIBS) $(LIB)
		mv testjmj  $(ICS_HOME)/bin 
effi_sec_srv:	effi_sec_srv.c effi_queue.c 
		$(CC)  -pthread -o effi_sec_srv $(CFLAGS) effi_sec_srv.c effi_queue.c $(LIB) 
		mv effi_sec_srv  $(ICS_HOME)/bin 
test_queue:	test_queue.c effi_queue.c effi_sec.h
		$(CC) -o test_queue  test_queue.c effi_queue.c 
test_sec:	test_sec.c 
		$(CC) -o test_sec $(CFLAGS) test_sec.c $(LIB)
		
lib:   secuLib.o secuLib_wst.o
		$(AR) $(ARFLAGS) $(ICS_SRCPATH)/lib/libsecu.a  secuLib.o 
		$(AR) $(ARFLAGS) $(ICS_SRCPATH)/lib/libwst.a  secuLib_wst.o 
cleanup:
		rm -f *.o
