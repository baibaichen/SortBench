#include <iostream>
#include <algorithm>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <strings.h>
//#include <sys/time.h>

typedef struct list {
  struct list *next;
  unsigned char *data;
} list;
namespace first_simple{
  list *rsort(list *a, int b, list *sequel)
  {
  #define ended(a, b) b>0 && a->data[b-1]==0
  #define append(s, a) tmp=a; while(tmp->next) tmp=tmp->next; tmp->next=s
  #define deal(a, p) tmp = a->next, a->next = p, p = a, a = tmp
    list *pile[256], *tmp;
    int i;
    if(a == 0)
      return sequel;
    if(ended(a, b)) { /* pile finished */
      append(sequel, a);
      return a;
    }
    for(i = 256; --i >= 0; ) /* split */
      pile[i] = 0;
    while(a)
      deal(a, pile[a->data[b]]);
    for(i = 256; --i >= 0; ) /* recur on each pile */
      sequel = rsort(pile[i], b+1, sequel);
    return sequel;
  }
}
namespace improved{
  list* rsort(list* a)
  {
#define SIZE        256
#define singleton(a) a->next == 0
#define push(a, b) sp->sa = a, (sp++)->sb = b
#define pop(a, b) a = (--sp)->sa, b = sp->sb
#define stackempty() (sp <= stack)
    struct { list *sa; int sb; } stack[SIZE], *sp = stack;
    static list *pile[256];
    list *tmp, *sequel = 0;
    int i, b;
    if(a)
      push(a, 0);
    while(!stackempty()) {
      pop(a, b);
      if(singleton(a) || ended(a, b)) { /* pile finished */
        append(sequel, a);
        sequel = a;
        continue;
      }
      while(a) /* split */
        deal(a, pile[a->data[b]]);
      for(i = 0; i<256; i++) /* stack the pieces */
        if(pile[i]) {
          push(pile[i], b+1);
          pile[i] = 0;
        }
    }
    return sequel;
  }
}
namespace twoArray{
  typedef unsigned char *cString;
  void rsort(cString *a, int n)
  {
#define SIZE        256
#define push2(a, n, b) sp->sa = a, sp->sn = n, (sp++)->sb = b
#define pop2(a, n, b) a = (--sp)->sa, n = sp->sn, b = sp->sb
#define stackempty() (sp <= stack)
#define splittable(c) c > 0 && count[c] > 1
    struct { cString *sa; int sn, sb; } stack[SIZE], *sp = stack;
    cString *pile[256], *ak, *ta;
    static int count[256];
    int i, b;
    ta = (cString*)malloc(n*sizeof(cString));
    push2(a, n, 0);
    while (!stackempty()) {
      pop2(a, n, b);
      for(i = n; --i >= 0; ) /* tally */
        count[a[i][b]]++;
      for(ak = a, i = 0; i < 256; i++) { /* find places */
        if(splittable(i))
          push2(ak, count[i], b+1);
        pile[i] = ak += count[i];
        count[i] = 0;
      }
      for(i = n; --i >= 0; ) /* move to temp */
        ta[i] = a[i];
      for(i = n; --i >= 0; ) /* move home */
        *--pile[ta[i][b]] = ta[i];
    }
    free(ta);
  }
}
namespace american_flag{

#ifdef SIZE
#undef  SIZE
#endif // SIZE
#ifdef push
#undef push
#endif // push
#ifdef pop
#undef pop
#endif // pop


  enum { SIZE = 510, THRESHOLD = 16 };
  typedef unsigned char *cString;
  typedef struct { cString *sa; int sn, si; } stack_t;
  void simplesort(cString*, int, int);
  static void rsorta(cString *a, int n ,int b)
  {
#define push(a, n, i) sp->sa = a, sp->sn = n, (sp++)->si = i
#define pop(a, n, i) a = (--sp)->sa, n = sp->sn, i = sp->si
#define stackempty() (sp <= stack)
#define swap(p, q, r) r = p, p = q, q = r
    stack_t stack[SIZE], *sp = stack, stmp, *oldsp, *bigsp;
    cString *pile[256], *ak, *an, r, t;
    static int count[256], cmin, nc;
    int *cp, c, cmax/*, b = 0*/;
    push(a, n, b);
    while(!stackempty()) {
      pop(a, n, b);
      if(n < THRESHOLD) { /* divert */
        simplesort(a, n, b);
        continue;
      }
      an = a + n;
      if(nc == 0) { /* untallied? */
        cmin = 255; /* tally */
        for(ak = a; ak < an; ) {
          c = (*ak++)[b];
          if(++count[c] == 1 && c > 0) {
            if(c < cmin) cmin = c;
            nc++;
          }
        }
        if(sp+nc > stack+SIZE) { /* stack overflow */
          rsorta(a, n, b);
          continue;
        }
      }
      oldsp = bigsp = sp, c = 2; /* logarithmic stack */
      pile[0] = ak = a+count[cmax=0]; /* find places */
      for(cp = count+cmin; nc > 0; cp++, nc--) {
        while(*cp == 0) cp++;
        if(*cp > 1) {
          if(*cp > c) c = *cp, bigsp = sp;
          push(ak, *cp, b+1);
        }
        pile[cmax = cp-count] = ak += *cp;
      }
      swap(*oldsp, *bigsp, stmp);
      an -= count[cmax]; /* permute home */
      count[cmax] = 0;
      for(ak = a; ak < an; ak += count[c], count[c] = 0) {
        r = *ak;
        while(--pile[c = r[b]] > ak)
          swap(*pile[c], r, t);
        *ak = r;
      } /* here nc = count[...] = 0 */
    }
  }
  void rsort(cString *a, int n) { rsorta(a, n, 0); }
}
void insertion_sort(int *array, int offset, int end) {
  int x, y, temp;
  for (x=offset; x<end; ++x) {
    for (y=x; y>offset && array[y-1]>array[y]; y--) {
      temp = array[y];
      array[y] = array[y-1];
      array[y-1] = temp;
    }
  }
}

void radix_sort(int *data, int offset, int end, int shift) {
  int x, y, value, temp;
  int last[256] = { 0 }, pointer[256];

  for (x=offset; x<end; ++x) {
    ++last[(data[x] >> shift) & 0xFF];
  }

  last[0] += offset;
  pointer[0] = offset;
  for (x=1; x<256; ++x) {
    pointer[x] = last[x-1];
    last[x] += last[x-1];
  }

  for (x=0; x<256; ++x) {
    while (pointer[x] != last[x]) {
      value = data[pointer[x]];
      y = (value >> shift) & 0xFF;
      while (x != y) {
        temp = data[pointer[y]];
        data[pointer[y]++] = value;
        value = temp;
        y = (value >> shift) & 0xFF;
      }
      data[pointer[x]++] = value;
    }
  }

  if (shift > 0) {
    shift -= 8;
    for (x=0; x<256; ++x) {
      temp = x > 0 ? pointer[x] - pointer[x-1] : pointer[0] - offset;
      if (temp > 64) {
        radix_sort(data, pointer[x] - temp, pointer[x], shift);
      } else if (temp > 1) {
        // std::sort(array + (pointer[x] - temp), array + pointer[x]);
        insertion_sort(data, pointer[x] - temp, pointer[x]);
      }
    }
  }
}

int intcmp(const void *aa, const void *bb)
{
  const int *a = (int *)aa, *b = (int *)bb;
  return (*a < *b) ? -1 : (*a > *b);
}

int main(int argc, char **argv) {
  if (sizeof(int) != 4) {
    std::cerr << "Ooops. sizeof(int) != 4\n";
    return 1111;
  }

  int N;
  if (argc != 2 || sscanf(argv[1], "%d", &N) == -1) {
    std::cerr << "n missing\n";
    return 111;
  }
  int *array = (int *)malloc(sizeof(int) * N);

  for (int i=2; i<3; ++i) {
    int n = N;
    srand(1);
    while (n --> 0) {
      array[n] = rand();
    }

    clock_t t1 = clock();
    switch(i) {
    case 0: std::sort(array, array+N); break;
    case 1: qsort(array, N, sizeof(int), intcmp); break;
    case 2: radix_sort(array, 0, N, 24); break;
    }
    clock_t t2 = clock();

    n = N - 1;
    while (n --> 0) {
      if (array[n] > array[n+1]) {
        std::cerr << "sorting failed\n";
        return -1;
      }
    }

    double mtime = (double)(t2-t1)/CLOCKS_PER_SEC;
    switch(i) {
    case 0: std::cout << "std::sort " << mtime << "\n"; break;
    case 1: std::cout << "qsort " << mtime << "\n"; break;
    case 2: std::cout << "radix_sort " << mtime << "\n"; break;
    }
  }

  return 0;
}