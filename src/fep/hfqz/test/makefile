NPCDIR=${HOME}/src/npc
DESTDIR=${NPCDIR}/test
LIBDIR=${NPCDIR}/lib
INCDIR=${NPCDIR}/incl
#objects := $(patsubst %.c,%.o,$(wildcard *.c)) 
CC=cc
AR=ar
RM=rm -f

INCLUDES= -I${INCDIR} -I.

CCFLAGS= -c ${INCLUDES}

NPCOBJ= npctest.o 

all:	${DESTDIR}/npc

${DESTDIR}/npc:	${NPCOBJ} ${LIBDIR}/libnpc.a
	#$(CC) -o  ${DESTDIR}/npc ${NPCOBJ} -fPIC ${LIBDIR}/libnpc.a
	$(CC) -o  ${DESTDIR}/npc ${NPCOBJ} -L${LIBDIR} -lnpc

npctest.o:	npctest.c
	$(CC) $(CCFLAGS) npctest.c

clean:
	rm ${DESTDIR}/npc 
	rm *.o
