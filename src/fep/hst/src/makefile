LIBU =$(ICS_SRCPATH)/lib/libdcsutil.a
FLDINC = $(ICS_SRCPATH)/folder/inc
DCSINC = $(ICS_SRCPATH)/fep/hst/inc

ISEBINEXE = $(ICS_HOME)/bin/exe
ISEBINOBJ = $(ICS_HOME)/bin/obj
ISEBINLIB = $(ICS_HOME)/bin/lib

ALLINC=  -I$(FLDINC) -I$(DCSINC)  
LIB= -L $(ICS_SRCPATH)/lib    -ldcsutil -ltimer -lfold -lnsl
CFLAGS=-O $(ALLINC)

#-D_PERFORMANCE_TEST_

EXE = dcxhstsv dcxhstcp  dcxcmd dcxmonisv


lib:
	$(CC)   -c $(CFLAGS) dcssock.c
	$(AR) $(ARFLAGS) $(LIBU) dcssock.o

all: lib $(EXE) cleanup

dcxhstsv:	dcmhstsv.c 
		$(CC)  -pthread -o dcxhstsv $(CFLAGS) dcmhstsv.c  $(LIB) 
		mv dcxhstsv $(ICS_HOME)/bin 

dcxhstcp:	dcmhstcp.c  dcssock.c  utility.c 
		$(CC)   -pthread -o dcxhstcp $(CFLAGS) dcmhstcp.c dcssock.c utility.c  $(LIB)
		mv dcxhstcp $(ICS_HOME)/bin

dcxcmd:	dcmcmd.c  
		$(CC)   -o dcxcmd $(CFLAGS) dcmcmd.c   $(LIB)
		mv dcxcmd $(ICS_HOME)/bin
dcxmonisv:	dcmmonisv.c dcssock.c 
		$(CC)   -pthread -o dcxmonisv $(CFLAGS) dcmmonisv.c dcssock.c $(LIB) 

		mv dcxmonisv  $(ICS_HOME)/bin 
cleanup:
		rm -f *.o
