#ifndef ISO8583
#define ISO8583


#define  LEN_FIXED          0
#define  LEN_LLVAR          1
#define  LEN_LLLVAR         2

#define  FMT_MMDDHHMMSS     8
#define  FMT_YYMMDDHHMMSS   1
#define  FMT_YYMM           2
#define  FMT_YYYYMMDD       3
#define  FMT_YYMMDD         4
#define  FMT_MMDD           5
#define  FMT_HHMMSS         6
#define  FMT_MONEY          7
#define  FMT_B              0
#define  FMT_N              9
#define  FMT_A              10
#define  FMT_AN             11
#define  FMT_ANS            12
#define  FMT_Z              13


#define CHK_LENGTH     1
#define CHK_FORMAT     2
#define CHK_DATETIME   3

struct  ISO_8583 {              /* ISO 8583 Message Structure Definitions */
        int    len;             /* data element max length */
        unsigned char    type;  /* bit 0--x, bit 1--n×ó¶ÔÆë, bit 2--z bit 3--b*/
        unsigned char    flag;  /* length field length: 1--LLVARÐÍ 2--LLLVARÐÍ*/
        unsigned char    fmt;
};


struct len_str {
        short  len;
        char   *str;
};

struct data_element_flag {
        short bitf;
        short len;
        int   dbuf_addr;
};

typedef struct  {
        struct  data_element_flag f[128];
        struct  ISO_8583 *iso8583;
        short   off;
        char    dbuf[4096];
        char    message_id[10];
        char    head_data[50];
        char    pkg_type[5];
        short   headlen;
        char    head_flag;
        char    bitmap_flag;
        char    var_flag;
        char    msgid_flag;
} ISO_data;

typedef  struct _tagIso8583Bit
{
  int    bit_index;     /*bit order , 0..127 */
  int    bit_fixedvar;  /*flag for fixed/variable length */
                        /*be one of (LEN_FIXED,LEN_LLVAR,LEN_LLLVAR) */
  int    bit_length;    /*lenght of fixed-length bit or max length of */
                        /*variable-length bit */
  int    bit_format;    /*format of this bit,such as FMT_ANS,FMT_YYMMDD */
  int    bit_compress;  /*  */
  char   bit_name[40];      /*name of this bit */
}ISO8583BIT;

struct element_struc {
/*        short bitno; */          /* element no */
/*     short type; */           /* 0--default value, 1,2--process function */
        short flag;  
        short len;             /* when type=0, len=length of defaule value */
        int  pointer;        /* pointer of value or process function */
};

struct  trans_bitmap_struc {
/*        short trans_type;  */              /* transaction type */
/*        char  message_id[10]; */
        short   off;
        char buf[2048];            /* number of elememts */
        struct element_struc element[129];   /* transaction element ally */

};

int strtoiso(unsigned char *, ISO_data * ,int);
int isotostr(unsigned char *, ISO_data *  );
int getbit(ISO_data *, int, unsigned char *);
int setbit_t(ISO_data *, int ,unsigned char *, int );
void setbitoff(ISO_data *iso ,int n);
int SetIsoElement( int id , int len ,  unsigned char type , unsigned char flag, unsigned char fmt);
int SetIsoHeardFlag( ISO_data *,int  type );
int SetIsoFieldLengthFlag (ISO_data *, int type );
void clearbit(ISO_data *);
void asc_to_bcd(unsigned char *,unsigned  char *, int, unsigned char);
void bcd_to_asc(unsigned char *,unsigned  char *, int, unsigned char);
#endif
