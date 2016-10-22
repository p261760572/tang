LIBS=-L $(ICS_SRCPATH)/lib  -ldcsutil -ltimer -lfold  -lnsl


CFLAGS= -I $(ICS_SRCPATH)/fep/include -I$(ICS_SRCPATH)/folder/inc 

EXE =  poscmd possvr postcp

exe:    $(EXE) cleanup
all:    $(EXE) cleanup

possvr: dcmhstsv.c
	$(CC)   -g -o $@ $(CFLAGS)  dcmhstsv.c  $(LIBS)
	mv $@  $(ICS_HOME)/bin
postcp: dcmshttcp.c
	$(CC)   -g -o $@ $(CFLAGS)  dcmshttcp.c $(LIBS)
	mv $@  $(ICS_HOME)/bin
poscmd: dcmcmd.c
	$(CC)   -g -o $@ $(CFLAGS)  dcmcmd.c $(LIBS)
	mv $@  $(ICS_HOME)/bin
cleanup:
	rm  -f  *.o
