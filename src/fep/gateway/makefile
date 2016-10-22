.SUFFIXES: .c .o .h

EXE = gateway gateway_k nac_sw p_sw
all: $(EXE) cleanup
OBJ = gateway.c  log.c http_parse.c do_loop.c do_loop1.c do_loop2.c do_loop2-1.c iso_8583.c util_conf.c iso8583_conf.c iso_chkmsg.c desc.c
OBJ1 = gateway1.c  log.c do_loop3.c  util_conf.c  
#OBJ = gateway.o   log.o http_parse.o do_loop1.o do_loop.o iso_8583.o util_conf.o iso8583_conf.o iso_chkmsg.o
OBJ2 = nac_sw.c  log.c nac_do_loop.c  util_conf.c  
OBJ3 = ftp_app.c  log.c util_conf.c  
OBJ4 = p_sw.c  log.c p_do_loop.c iso_8583.c iso8583_conf.c util_conf.c  
gateway:	 
		$(CC)   -o $(ICS_HOME)/bin/gateway $(CFLAGS) $(OBJ) -lcrypto
gateway_k:	 
	$(CC)   -o $(ICS_HOME)/bin/gateway_k $(CFLAGS) $(OBJ1) 
nac_sw:
	$(CC)   -o $(ICS_HOME)/bin/nac_sw $(CFLAGS) $(OBJ2) 
p_sw:
	$(CC)   -o $(ICS_HOME)/bin/p_sw $(CFLAGS) $(OBJ4) 
ftp_app:
	$(CC)   -o $(ICS_HOME)/bin/ftp_app $(CFLAGS) $(OBJ3) 
cleanup:
		rm -f *.o
