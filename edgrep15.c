#include <unistd.h> // write, read, lseek, close, unlink
#include <stdio.h> // EOF, snprintf
#include <stdlib.h> // malloc, realloc, exit
#include <fcntl.h> // open, creat,
#include <string.h> // memset
#include "edgrep.h" // edgrep function protocols

int main(int argc, char *argv[]) {
  zero = (unsigned *)malloc(nlall * sizeof(unsigned)); commands(); return 0;
}
void commands(void) {  unsigned int *a1;  int c;
  for (;;) {
    c = '\n';
    for (addr1 = 0;;) {
      a1 = address();  c = getchr(); if (c != ',' && c != ';') { break; }
    }
    if ((addr2 = a1)==0) { given = 0;  addr2 = dot;  } else { given = 1; }
    if (addr1==0) { addr1 = addr2; }
    switch(c) {
    case EOF:  return; //need
    case 'e':  filename(c);  init(); goto caseread; // need
    case 'g':  global(1);  continue; // need
    case 'p':  case 'P':  getchr();  print();  continue; //need to print out message
    caseread:
        io = open((const char*)file, 0); setwide(); append(getfile, addr2);
        continue;
    case 'z':  grepline();  continue;
    default:  // fallthrough
    greperror(c);  continue;
    }
  }
}
unsigned int* address(void) {
  unsigned int *a; int c; a = 0; c = getchr(); peekc = c; return (a);
  /*NOTREACHED*/  return 0;
}
int advance(char *lp, char *ep) {
  for (;;) {
    switch (*ep++) {
      case CCHR:  if (*ep++ == *lp++) { continue; } return(0);
      case CDOT:  if (*lp++) { continue; }    return(0);
      case CDOL:  if (*lp==0) { continue; }  return(0);
      case CEOF:  return(1);
      case CCL:   if (cclass(ep, *lp++, 1)) {  ep += *ep;  continue; }  return(0);
    }
  }
}
int append(int (*f)(void), unsigned int *a) {
  unsigned int *a1, *rdot;  int nline, tl;  nline = 0;  dot = a;
  while ((*f)() == 0) { tl = putline(); a1 = ++dol; rdot = ++dot; *rdot = tl; }
  return(nline);
}
int cclass(char *set, int c, int af) {  int n;
  // if (c == 0) { return(0); }
  n = *set++; while (--n) { if (*set++ == c) { return(af); } } return(!af);
}
void compile(int eof) {  int c, cclcnt;  char *ep = expbuf, *lastep, bracket[NBRA], *bracketp = bracket;
  if ((c = getchr()) == '\n') { peekc = c;  c = eof; }
  if (c == eof) {  if (*ep==0) {}  return; } nbra = 0;
  if (c=='^') { c = getchr();  *ep++ = CCIRC; } peekc = c;
  lastep = 0;
  for (;;) {
    if (ep >= &expbuf[ESIZE]) { goto cerror; }
    c = getchr(); if (c == '\n') { peekc = c;  c = eof; }
    if (c==eof) { if (bracketp != bracket) { goto cerror; }  *ep++ = CEOF;  return;  }
    if (c!='*') { lastep = ep; }
    switch (c) {
      case '\\':
        if ((c = getchr())=='(') {
          if (nbra >= NBRA) { goto cerror; }
          *bracketp++ = nbra;
          *ep++ = CBRA;
          *ep++ = nbra++;
          continue;
        }
        if (c == ')') {  if (bracketp <= bracket) { goto cerror; }  *ep++ = CKET;  *ep++ = *--bracketp;  continue; }
        if (c>='1' && c<'1'+NBRA) { *ep++ = CBACK;  *ep++ = c-'1';  continue; }
        *ep++ = CCHR;
        if (c=='\n') { goto cerror; }
        *ep++ = c;
        continue;
      case '.': *ep++ = CDOT;  continue;
      case '\n':  goto cerror;
      case '*':  if (lastep==0 || *lastep==CBRA || *lastep==CKET) { goto defchar; }  *lastep |= STAR; continue;
      case '$':  if ((peekc=getchr()) != eof && peekc!='\n') { goto defchar; }  *ep++ = CDOL;  continue;
      case '[':  *ep++ = CCL;  *ep++ = 0;  cclcnt = 1;  if ((c=getchr()) == '^') {  c = getchr();  ep[-2] = NCCL; }
        do {
          if (c=='\n') { goto cerror; }  if (c=='-' && ep[-1]!=0) {
            if ((c=getchr())==']') { *ep++ = '-';  cclcnt++;  break; }
            while (ep[-1] < c) {  *ep = ep[-1] + 1;  ep++;  cclcnt++;  if (ep >= &expbuf[ESIZE]) { goto cerror; } }
          }
          *ep++ = c;  cclcnt++;  if (ep >= &expbuf[ESIZE]) { goto cerror; }
        } while ((c = getchr()) != ']');
        lastep[1] = cclcnt;  continue;
      defchar:  default:  *ep++ = CCHR;  *ep++ = c;
    }
  }  cerror:  expbuf[0] = 0;  nbra = 0;
}
int execute(unsigned int *addr) {
  char *p1, *p2 = expbuf; p1 = getline_blk(*addr);
  do { if (advance(p1, p2)) { return(1); } } while (*p1++);
  return(0);
}
void filename(int comm) {
  char *p1;  int c; c = getchr();
  while ((c = getchr()) == ' ') { } p1 = file;
  do { *p1++ = c; } while ((c = getchr()) != '\n');
}
char * getblock(unsigned int atl, int iof) {
  int off = (atl<<1) & (BLKSIZE-1) & ~03;
  return(obuff+off);
}
char inputbuf[GBSIZE];
int getchr(void) {  char c;
  if ((lastc=peekc)) {  peekc = 0;  return(lastc); }
  if (globp) {  if ((lastc = *globp++) != 0) { return(lastc); }  globp = 0;  return(EOF);  }
  if (read(0, &c, 1) <= 0) { return(lastc = EOF); }
  lastc = c&0177; return(lastc);
} //need
int getfile(void) {  int c;  char *lp = linebuf, *fp = nextip;
  do { if (--ninbuf < 0) {
      if ((ninbuf = (int)read(io, genbuf, LBSIZE)-1) < 0) { return(EOF); }
      fp = genbuf; } c = *fp++; *lp++ = c;
  } while (c != '\n'); nextip = fp; return(0);
}
char* getline_blk(unsigned int tl) {
  char *bp = getblock(tl, READ), *lp = linebuf;
  while ((*lp++ = *bp++)) {} return(linebuf);
}
void global(int k) {
  char *gp;  int c;  unsigned int *a1;  char globuf[GBSIZE];
  setwide(); c = getchr(); compile(c); gp = globuf; c = getchr();
  if (gp == globuf) { *gp++ = 'p'; } *gp++ = '\n';
  for (a1 = zero; a1 <= dol; a1++) {
    if (a1>=addr1 && a1<=addr2 && execute(a1)==k) { *a1 |= 01; }
  }
  for (a1 = zero; a1 <= dol; a1++) {
    if (*a1 & 01) { *a1 &= ~01; dot = a1; globp = globuf; commands(); //removing this line makes it only execute the first test
    }
  }
}
void greperror(char c) {  getchr();  /* throw away '\n' */
  snprintf(grepbuf, sizeof(grepbuf), "\'%c\' is a non-grep command", c);  puts_(grepbuf);  }
void grepline(void) {
  getchr(); for (int i = 0; i < 50; ++i) { putchr_('-'); } putchr_('\n');
}
void init(void) {
  int *markp;
  close(tfile);
  tline = 2;
  for (markp = names; markp < &names[26]; )  {  *markp++ = 0;  }
  subnewa = 0;  anymarks = 0;  iblock = -1;  oblock = -1;  ichanged = 0;
  close(creat(tfname, 0600));  tfile = open(tfname, 2);  dot = dol = zero;  memset(inputbuf, 0, sizeof(inputbuf));
}
void print(void) {  unsigned int *a1 = addr1;
  do { puts_(getline_blk(*a1++)); } while (a1 <= addr2);
}
void putchr_(int ac) {
  char *lp = linp;  int c = ac; *lp++ = c;
  if (c == '\n' || lp >= &line[64]) { linp = line;
    write(oflag ? 2 : 1, line, lp - line); return; } linp = lp;
}
int putline(void) { char *bp, *lp;  int nl;  unsigned int tl; lp = linebuf;
  tl = tline; bp = getblock(tl, WRITE);
  while ((*bp = *lp++)) { if (*bp++ == '\n') { *--bp = 0; break; } }
  nl = tline; tline += (((lp - linebuf) + 03) >> 1) & 077776; return(nl);
}
void puts_(char *sp) {  col = 0;  while (*sp) { putchr_(*sp++); }  putchr_('\n');  }
void setwide(void) {
  if (!given) {
    addr1 = zero + (dol>zero);
    addr2 = dol;
  }
}
