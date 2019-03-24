#define BLKSIZE 4096
#define NBLK 2047
#define FNSIZE 128
#define LBSIZE 4096
#define ESIZE 256
#define GBSIZE 256
#define NBRA 5
#define KSIZE 9
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
#define WRITE 1  /* #define EOF = -1; */


int  peekc, lastc, given, ninbuf, io, pflag;
int  vflag  = 1, oflag, listf, listn, col, tfile  = -1, tline, iblock  = -1, oblock  = -1, ichanged, nleft;
int  names[26], anymarks, nbra, subnewa, subolda, fchange, wrapp, bpagesize = 20;
unsigned nlall = 128;  unsigned int  *addr1, *addr2, *dot, *dol, *zero;

long  count;
char  Q[] = "", T[] = "TMP", savedfile[FNSIZE], file[FNSIZE], linebuf[LBSIZE], rhsbuf[LBSIZE/2], expbuf[ESIZE+4];
char  genbuf[LBSIZE], *nextip, *linebp, *globp, *mktemp(char *), tmpXXXXX[50] = "/tmp/eXXXXX";
char  *tfname, *loc1, *loc2, ibuff[BLKSIZE], obuff[BLKSIZE], WRERR[]  = "WRITE ERROR", *braslist[NBRA], *braelist[NBRA];
char  line[70];  char  *linp  = line;
char grepbuf[GBSIZE];

#define MAXFILE 100 // max number of files for files array
int nfiles = 0; // number of files
char* files[MAXFILE]; // array to hold files
char* fname; // file name
int mfiles = 0; // multiple file boolean 0 for no 1 for yes
char* regex; // regex pattern


void commands(void); void add(int i);  unsigned int *address(void);  int advance(char *lp, char *ep);
int append(int (*f)(void), unsigned int *a);  int backref(int i, char *lp);
void blkio(int b, char *buf, long (*iofcn)(int, void*, unsigned long));  void callunix(void);
int cclass(char *set, int c, int af);  void compile(int eof);
int compsub(void);  void dosub(void);  void error(char *s);  int execute(unsigned int *addr);  void exfile(void);
void filename(int comm);  void gdelete(void);  char *getblock(unsigned int atl, int iof); int getchr(void);
int getcopy(void);  int getfile(void);  char *getline_blk(unsigned int tl);  int getnum(void);  int getsub(void);
int gettty(void);  int gety(void);  void global(int k);  void init(void);
void join(void);  void move_(int cflag);  void newline(void);  void nonzero(void);  void onhup(int n);
void onintr(int n);  char *place(char *sp, char *l1, char *l2);  void print(void);  void putchr_(int ac);
void putd(void);  void putfile(void);  int putline(void);  void puts_(char *sp); void quit(int n);
void rdelete(unsigned int *ad1, unsigned int *ad2);  void reverse(unsigned int *a1, unsigned int *a2);
void setwide(void);  void setnoaddr(void);  void squeeze(int);  void substitute(int inglob);
void greperror(char);  void grepline(void);
