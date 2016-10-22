HOMEPATH = $(HOME)/tang/
PATHOBJ = $(HOMEPATH)src/fep/db/

ALLINC = -I$(HOMEPATH)src/fep/db/inc/ -I$(ORACLE_HOME)/precomp/public/
ALLLIB = -L$(ORACLE_HOME)/lib/ -L$(ORACLE_HOME)/precomp/lib/  \
	-L$(PATHOBJ)inc \
	-L$(HOMEPATH)src/bin/ \
	$(HOMEPATH)src/lib/glbdb.a  \
	-lsqlplus -lclntsh -lpthread

APPNAME = libdb
PROC=proc

APPLIBPATH = $(HOMEPATH)src/lib/

DBOBJS = dbfunction.o \
	 ti_bc_trans_det.o \
	 ti_bc_trans_inf.o \
	 ti_bc_trans_chnl_inf.o \
	 ti_bc_term_inf.o  \
	 ti_bc_ins_inf.o   \
	 ti_bc_mchnt_det.o \
	 ti_bc_mchnt_inf.o \
	 ti_bc_qs_mchnt_inf.o \
	 tl_bc_trans_log.o \
	 ti_bc_card_bin.o  \
	 ti_bc_mchnt_term_charge_inf.o \
	 ti_bc_mt_flow_id.o \
	 tl_bc_bill_log.o  \
         ti_bc_app_fork_inf.o \
         ti_bc_route_gateway_inf.o \
         ti_bc_mid_day_limit_tims.o \
         ti_bc_white_list.o \
         ti_bc_black_list.o \
         ti_bc_route_card_bin.o \
         ti_bc_ivr.o \
         ti_bc_ivr_opr_cmd_inf.o \
         ti_bc_ivr_opr_impdata_inf.o \
         ti_bc_ivr_inf_key_inf.o \
         ti_bc_ivr_mobile_inf.o \
         ti_bc_ivr_list_inf.o \
         tl_bc_exp_log.o \
         tl_bc_cardno_list_inf.o \
         ti_bc_error_inf.o \
         msg_send.o \
         msg_rcv.o	 	 

static : $(DBOBJS) $(APPNAME).a 

dynamic : $(DBOBJS) $(APPNAME).so 

clean:
	@rm -f $(DBOBJS)
	@rm -f *.o t*.c

CC = cc
CCFLAGS = -g
CFLAGSADD = $(ALLINC) $(ALLLIB)

##############################################################################

.SUFFIXES: .pc .c .o .typ .h

$(APPNAME).so :
	$(CC) -o $(APPLIBPATH)$(APPNAME).so -G -KPIC $(DASSRC) $(ALLLIB)

$(APPNAME).a:$(DASOBJ)
	$(AR) -rv $(APPLIBPATH)$(APPNAME).a $(DBOBJS)

pc1:
	$(PROC) $(PROCFLAGS) iname=$(PCCSRC) $(INCLUDE)

.pc.c:
	$(PROC) $(PROCFLAGS) UNSAFE_NULL=YES MODE=ORACLE and DBMS=V8 iname=$*

.pc.o:
	$(PROC) $(PROCFLAGS) UNSAFE_NULL=YES MODE=ORACLE and DBMS=V8 iname=$*
	$(CC) $(CFLAGS) ${CFLAGSADD} -c $*.c

.c.o:
	$(CC) $(CFLAGS) ${CFLAGSADD} -c $*.c

.typ.h:
	$(OTT) intype=$*.typ hfile=$*.h outtype=$*o.typ $(OTTFLAGS) code=c user=scott/tiger

