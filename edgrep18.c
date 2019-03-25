#include <unistd.h> // write, read, lseek, close, unlink
#include <stdio.h> // EOF, snprintf
#include <stdlib.h> // malloc, realloc, exit
#include <fcntl.h> // open, creat,
#include <string.h> // memset
#include "edgrep.h" // edgrep function protocols
#include <dirent.h>

typedef struct dirent dirent;
DIR* dir;
dirent* in_dir;

int main(int argc, char *argv[]) {
  char *p1, *p2;
  if (argc < 3) { puts_("Not enough arguments"); return 0; }
  regex = argv[1]; fname = argv[2]; // no options
  // puts_(regex); puts_(fname); // test for right argument
  if ((dir = opendir(fname)) != NULL) { // if fname is a directory
    while((in_dir = readdir(dir)) != NULL) {
      if (in_dir->d_name[0] != '.'  && in_dir->d_name[strlen(in_dir->d_name)-1] != '~') {
        files[nfiles] = in_dir->d_name;
        puts_(files[nfiles]); // test files in dir
        nfiles++;
      }
    }
    if (nfiles > 0) { mflag = 1; }
    closedir(dir);
  } else if (argc > 3) { // if we have multiple files
    mflag = 1; nfiles = 1;
    for (size_t i = 3; i < argc; ++i) {
      files[nfiles] = argv[i];
      puts_(files[nfiles]);
      nfiles++;
    }
  }
  zero = (unsigned *)malloc(nlall * sizeof(unsigned));
  if (mflag == 1) { // loop through files if multiple files
    puts_("Multiple files");
    for (size_t i = 0; i < nfiles; ++i) {
      p1 = files[i];
      fname = p1;
      p2 = savedfile;
      while ((*p2++ = *p1++)) {
        if (p2 >= &savedfile[sizeof(savedfile)]) {
          p2--;
        }
      }
      compile(regex); init(p1); search();
    }
  } else {
    // puts_("1 file");
    p1 = fname; p2 = savedfile;
    compile(regex); init(p1); search();
    exit(0);
  }
  return 0;
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
int cclass(char *set, int c, int af) {
  int n; n = *set++;
  while (--n) { if (*set++ == c) { return(af); } } return(!af);
}
void compile(char* s) {
  int c, cclcnt; nbra = 0;
  char *ep = expbuf, *lastep, bracket[NBRA], *bracketp = bracket, *sp = s;
  if (*sp=='^') { ++sp;  *ep++ = CCIRC; }
  peekc = *sp;
  for (;;) {
    c = *sp++; if (c == '\0') { *ep++ = CEOF; return; } lastep = ep;
    switch (c) {
    case '\\':
      if ((c = *sp++) == '(') {
        *bracketp++ = nbra; *ep++ = CBRA; *ep++ = nbra++; continue;
      }
      if (c == ')') { *ep++ = CKET; *ep++ = *--bracketp; continue; }
      if (c >= '1' && c < '1' + NBRA) { *ep++ = CBACK; *ep++ = c - '1'; continue; }
      *ep++ = CCHR; *ep++ = c; continue;
    case '.': *ep++ = CDOT; continue;
    case '*':
      if (lastep == 0 || *lastep == CBRA || *lastep == CKET) {
        *ep++ = CCHR; *ep++ = c; continue;
      }
      *lastep |= STAR; continue;
    case '$':
      if ((peekc = *sp++) != '\0') { *ep++ = CCHR; *ep++ = c; continue; }
      --sp; *ep++ = CDOL; continue;
    case '[':
      *ep++ = CCL; *ep++ = 0; cclcnt = 1;
      if ((c = *sp++) == '^') { c = *sp++; ep[-2] = NCCL; }
      do {
        if (c == '-' && ep[-1] != 0) {
          if ((c = *sp++) == ']') { *ep++ = '-'; cclcnt++; break; }
          while (ep[-1] < c) { *ep = ep[-1] + 1; ep++; cclcnt++; }
        }
        *ep++ = c; cclcnt++;
      } while ((c = *sp++) != ']');
      lastep[1] = cclcnt; continue;
    default:
      *ep++ = CCHR; *ep++ = c;
    }
  }
}
int execute() {
  char *p1, *p2 = expbuf; p1 = linebuf;
  do { if (advance(p1, p2)) { return(1); } } while (*p1++);
  return(0);
}
int getfile(void) {  int c;  char *lp = linebuf, *fp = nextip;
  do { if (--ninbuf < 0) {
      if ((ninbuf = (int)read(io, genbuf, LBSIZE)-1) < 0) { return(EOF); }
      fp = genbuf; } c = *fp++; *lp++ = c;
  } while (c != '\n'); nextip = fp; return(0);
}
void init(char* filename) {
  if ((io = open(filename, 0)) < 0) { puts_("Cannot open file"); }
  getfile(); close(io); io = -1;
}
void putchr_(int ac) {
  char *lp = linp;  int c = ac; *lp++ = c;
  if (c == '\n' || lp >= &line[64]) {
    linp = line; write(oflag ? 2 : 1, line, lp - line); return;
  } linp = lp;
}
void puts_(char *sp) { while (*sp) { putchr_(*sp++); }  putchr_('\n');  }
void search() {
  char *gp = genbuf, *lp = linebuf;
  while (*gp != '\0') {
    if (*gp == '\n') {
      *lp++ = '\0'; if (execute()) { puts_(linebuf); } lp = linebuf; ++gp;
    } else { *lp++ = *gp++; }
  }
}
