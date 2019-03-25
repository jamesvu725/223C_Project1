#define BLKSIZE 4096
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
#define WRITE 1  /* #define EOF = -1; */
#define MAXFILE 100 // max number of files for files array

int  peekc, lastc, given, ninbuf, io, oflag, tfile  = -1, tline;
int  names[26], nbra;
unsigned nlall = 128;  unsigned int  *addr1, *addr2, *dot, *dol, *zero;

char  savedfile[FNSIZE], file[FNSIZE], linebuf[LBSIZE], expbuf[ESIZE+4];
char  genbuf[LBSIZE], *nextip, *globp, *tfname, obuff[BLKSIZE];
char  line[70];  char  *linp  = line, grepbuf[GBSIZE];

int nfiles = 0; // number of files
char* files[MAXFILE]; // array to hold files
char* fname; // file name
int mfiles = 0; // multiple file boolean 0 for no 1 for yes
char* regex; // regex pattern

void commands(void); unsigned int *address(void);  int advance(char *lp, char *ep);
int append(int (*f)(void), unsigned int *a); int cclass(char *set, int c, int af);
void compile(char* s); int execute(); void filename(int comm);
char *getblock(unsigned int atl, int iof); int getchr(void); int getfile(void);
char *getline_blk(unsigned int tl); void global(int k); void init(char* filen);
void print(void);  void putchr_(int ac); int putline(void); void puts_(char *sp);
void setwide(void); void greperror(char);  void grepline(void); void search();
