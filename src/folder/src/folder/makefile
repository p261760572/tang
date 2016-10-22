include ../../make/makefile.defines

OBJ = fold_create.o  fold_globals.o  fold_locate.o  fold_read.o \
      fold_request.o fold_write.o fold_init.o fold_lock.o fold_view.o\
      fold_error.o fold_attr.o fold_log.o

EXE = foldSvr foldUtil

.c.o:
	$(CC)   -c $(CFLAGS) $*.c
	$(AR) $(ARFLAGS) $(FLDLIB) $*.o

obj:    $(OBJ) cleanup
exe:    $(EXE) 
all:    obj  exe

foldSvr: foldSvr.c
	$(CC)   -o $(FLDBIN)/foldSvr $(CFLAGS) foldSvr.c $(FLDLIB)              

foldUtil: foldUtil.c
	$(CC)   -o $(FLDBIN)/foldUtil $(CFLAGS) foldUtil.c $(FLDLIB)              

cleanup:
	rm  -f  *.o

