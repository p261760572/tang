include  ../../make/makefile.defines

EXE = batchtest inteacttest iseps ipcname eachoFold cleanup

exe:    $(EXE) 

inteacttest: inteacttest.c 
	$(CC)   -o $(FLDBIN)/inteacttest $(CFLAGS) inteacttest.c $(FLDLIB)              
batchtest: batchtest.c 
	$(CC)   -o $(FLDBIN)/batchtest $(CFLAGS) batchtest.c $(FLDLIB)              
eachoFold: eachoFold.c 
	$(CC)   -o $(FLDBIN)/eachoFold $(CFLAGS) eachoFold.c $(FLDLIB)

ipcname: ipcname.c 
	$(CC)   -o $(FLDBIN)/ipcname $(CFLAGS) ipcname.c $(FLDLIB)

iseps: iseps.c 
	$(CC)   -o $(FLDBIN)/iseps $(CFLAGS) iseps.c

cleanup:
	rm  -f  *.o

