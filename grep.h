#define FNSIZE 128
#define LBSIZE 4096
#define ESIZE 256
#define GBSIZE 256
#define NBRA 5
#define CBRA 1
#define CCHR 2
#define CDOT 4
#define CCL 6
#define NCCL 8
#define CDOL 10
#define CEOF 11
#define CKET 12
#define CBACK 14
#define CCIRC 15
#define STAR 01
#define READ 0
#define WRITE 1
#define MAXFILE 100

int  peekc, lastc, ninbuf, io, oflag, nbra;
unsigned nlall = 128;  unsigned int *zero;

char linebuf[LBSIZE], expbuf[ESIZE+4], genbuf[LBSIZE];
char *nextip, line[70], *linp  = line, grepbuf[GBSIZE];

int mflag = 0, match = 0;
char* files[MAXFILE], *fname, *regex;

int advance(char *lp, char *ep); int cclass(char *set, int c, int af);
void commands(void); void compile(char* s); int execute(void); int getfile(void);
void exfile(char* filename); void putchr_(int ac); void puts_(char *sp);
void putsf(char *sp); void search(void); void grep(char *s, char* filename);
