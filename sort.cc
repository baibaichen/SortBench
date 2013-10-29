#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <functional>
#include <iostream>

#include "quicksort.h"
#include "sort.h"
#include "timsort.h"
#include "drand48.h"

using namespace std;

/**********************************
 * BEGIN OF PAUL'S IMPLEMENTATION *
 **********************************/

/* Attractive Chaos: I have added inline where necessary. */

/*
Copyright (c) 2004 Paul Hsieh
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    Neither the name of sorttest nor the names of its contributors may be
    used to endorse or promote products derived from this software without
    specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

/*

Recommended flags:
------------------

Intel C/C++:
icl /O2 /G6 /Qaxi /Qxi /Qip sorttest.c

WATCOM C/C++:
wcl386 /otexan /6r sorttest.c

GCC:
gcc -O3 -mcpu=athlon-xp -march=athlon-xp sorttest.c

MSVC:
cl /O2 /Ot /Og /G6 sorttest.c

*/

static inline void sort2 (int * numbers) {
int tmp;

    if (numbers[0] <= numbers[1]) return;
    tmp = numbers[0];
    numbers[0] = numbers[1];
    numbers[1] = tmp;
}

static inline void sort3 (int * numbers) {
int tmp;

    if (numbers[0] <= numbers[1]) {
        if (numbers[1] <= numbers[2]) return;
        if (numbers[2] <= numbers[0]) {
            tmp = numbers[0];
            numbers[0] = numbers[2];
            numbers[2] = numbers[1];
            numbers[1] = tmp;
            return;
        }
        tmp = numbers[1];
    } else {
        tmp = numbers[0];
        if (numbers[0] <= numbers[2]) {
            numbers[0] = numbers[1];
            numbers[1] = tmp;
            return;
        }
        if (numbers[2] <= numbers[1]) {
            numbers[0] = numbers[2];
            numbers[2] = tmp;
            return;
        }
        numbers[0] = numbers[1];
    }
    numbers[1] = numbers[2];
    numbers[2] = tmp;
}

static inline void sort4 (int * num) {
int tmp;
  if (num[0] < num[1]) {
    if (num[1] < num[2]) {
      if (num[1] < num[3]) {
        if (num[2] >= num[3]) {
          tmp = num[2];
          num[2] = num[3];
          num[3] = tmp;
        }
      } else {
        tmp = num[1];
        if (num[0] < num[3]) {
          num[1] = num[3];
        } else {
          num[1] = num[0];
          num[0] = num[3];
        }
        num[3] = num[2];
        num[2] = tmp;
      }
    } else {
      if (num[0] < num[2]) {
        if (num[2] < num[3]) {
          if (num[1] < num[3]) {
            tmp = num[1];
          } else {
            tmp = num[3];
            num[3] = num[1];
          }
          num[1] = num[2];
          num[2] = tmp;
        } else {
          if (num[0] < num[3]) {
            tmp = num[3];
          } else {
            tmp = num[0];
            num[0] = num[3];
          }
          num[3] = num[1];
          num[1] = tmp;
        }
      } else {
        if (num[0] < num[3]) {
          tmp = num[0];
          num[0] = num[2];
          if (num[1] < num[3]) {
            num[2] = num[1];
          } else {
            num[2] = num[3];
            num[3] = num[1];
          }
          num[1] = tmp;
        } else {
          if (num[2] < num[3]) {
            tmp = num[0];
            num[0] = num[2];
            num[2] = tmp;
            tmp = num[1];
            num[1] = num[3];
          } else {
            tmp = num[1];
            num[1] = num[2];
            num[2] = num[0];
            num[0] = num[3];
          }
          num[3] = tmp;
        }
      }
    }
  } else {
    tmp = num[0];
    if (tmp < num[2]) {
      if (tmp < num[3]) {
        num[0] = num[1];
        num[1] = tmp;
        if (num[2] >= num[3]) {
          tmp = num[2];
          num[2] = num[3];
          num[3] = tmp;
        }
      } else {
        if (num[1] < num[3]) {
          num[0] = num[1];
          num[1] = num[3];
        } else {
          num[0] = num[3];
        }
        num[3] = num[2];
        num[2] = tmp;
      }
    } else {
      if (num[1] < num[2]) {
        if (num[2] < num[3]) {
          num[0] = num[1];
          num[1] = num[2];
          if (tmp < num[3]) {
            num[2] = tmp;
          } else {
            num[2] = num[3];
            num[3] = tmp;
          }
        } else {
          if (num[1] < num[3]) {
            num[0] = num[1];
            num[1] = num[3];
          } else {
            num[0] = num[3];
          }
          num[3] = tmp;
        }
      } else {
        if (num[1] < num[3]) {
          num[0] = num[2];
          if (tmp < num[3]) {
            num[2] = tmp;
          } else {
            num[2] = num[3];
            num[3] = tmp;
          }
        } else {
          if (num[2] < num[3]) {
            num[0] = num[2];
            num[2] = num[1];
            num[1] = num[3];
            num[3] = tmp;
          } else {
            num[0] = num[3];
            num[3] = tmp;
            tmp = num[1];
            num[1] = num[2];
            num[2] = tmp;
          }
        }
      }
    }
  }
}

static inline void sortAlt2 (int * numbers, int * altNumbers) {
    if (numbers[0] <= numbers[1]) {
        altNumbers[0] = numbers[0];
        altNumbers[1] = numbers[1];
    } else {
        altNumbers[0] = numbers[1];
        altNumbers[1] = numbers[0];
    }
}

static inline void sortAlt3 (int * numbers, int * altNumbers) {
    if (numbers[0] <= numbers[1]) {
        if (numbers[1] <= numbers[2]) {
            altNumbers[0] = numbers[0];
            altNumbers[1] = numbers[1];
            altNumbers[2] = numbers[2];
        } else if (numbers[2] <= numbers[0]) {
            altNumbers[0] = numbers[2];
            altNumbers[1] = numbers[0];
            altNumbers[2] = numbers[1];
        } else {
            altNumbers[0] = numbers[0];
            altNumbers[1] = numbers[2];
            altNumbers[2] = numbers[1];
        }
    } else {
        if (numbers[0] <= numbers[2]) {
            altNumbers[0] = numbers[1];
            altNumbers[1] = numbers[0];
            altNumbers[2] = numbers[2];
        } else if (numbers[2] <= numbers[1]) {
            altNumbers[0] = numbers[2];
            altNumbers[1] = numbers[1];
            altNumbers[2] = numbers[0];
        } else {
            altNumbers[0] = numbers[1];
            altNumbers[1] = numbers[2];
            altNumbers[2] = numbers[0];
        }
    }
}

/*
 *  Insert Sort
 */

inline void insertSort (int numbers[], int qty) {
int i, j, idx, q4;
int tmp;

    if (qty <= 4) {
        if (qty == 4) sort4 (numbers);
        else if (qty == 3) sort3 (numbers);
        else if (qty == 2) sort2 (numbers);
        return;
    }

    q4 = qty - 4;

    for (i=0; i < q4; i++) {
        idx = i;
        for (j=i+1; j < qty; j++) {
            if (numbers[j] < numbers[idx]) idx = j;
        }
        if (idx != i) {
            tmp = numbers[idx];
            numbers[idx] = numbers[i];
            numbers[i] = tmp;
        }
    }

    sort4 (numbers + q4);
}

/*
 *  Heap Sort
 */

/* Assure the heap property for entries from top to last */
static void siftDown (int numbers[], int top, int last) {
int tmp = numbers[top];
int maxIdx = top;

    while (last >= (maxIdx += maxIdx)) {

        /* This is where the comparison occurrs and where a sufficiently
           good compiler can use a computed conditional result rather
           than using control logic. */
        if (maxIdx != last && numbers[maxIdx] < numbers[maxIdx + 1]) maxIdx++;

        if (tmp >= numbers[maxIdx]) break;
        numbers[top] = numbers[maxIdx];
        top = maxIdx;
    }
    numbers[top] = tmp;
}

/* Peel off the top siftDown operation since its parameters are trivial to
   fill in directly (and this saves us some moves.) */
static void siftDown0 (int numbers[], int last) {
int tmp;

    if (numbers[0] < numbers[1]) {
        tmp = numbers[1];
        numbers[1] = numbers[0];
        siftDown (numbers, 1, last);
    } else {
        tmp = numbers[0];
    }
    numbers[0] = numbers[last];
    numbers[last] = tmp;
}

void heapSort (int numbers[], int qty) {
int i;

    if (qty <= 4) {
        if (qty == 4) sort4 (numbers);
        else if (qty == 3) sort3 (numbers);
        else if (qty == 2) sort2 (numbers);
        return;
    }

    i = qty / 2;
    /* Enforce the heap property for each position in the tree */
    for (  qty--; i >  0; i--) siftDown  (numbers, i, qty);
    for (i = qty; i >= 4; i--) siftDown0 (numbers, i);
    sort4 (numbers);
}

/*
 *  Quick Sort
 */

static int medianOf3 (int * numbers, int i, int j) {
int tmp;

    if (numbers[0] <= numbers[i]) {
        if (numbers[j] <= numbers[0]) return numbers[0]; /* j 0 i */
        if (numbers[i] <= numbers[j]) j = i;             /* 0 i j */
                                                         /* 0 j i */
    } else {
        if (numbers[0] <= numbers[j]) return numbers[0]; /* i 0 j */
        if (numbers[j] <= numbers[i]) j = i;             /* j i 0 */
                                                         /* i j 0 */
    }
    tmp = numbers[j];
    numbers[j] = numbers[0];
    numbers[0] = tmp;
    return tmp;
}

static void quickSortRecurse (int * numbers, int left, int right) {
int pivot, lTmp, rTmp;

    qsrStart:;

#if defined(__GNUC__)
    if (right <= left + 8) {
        insertSort (numbers + left, right - left + 1);
        return;
    }
#else
    if (right <= left + 3) {
        if (right == left + 1) {
            sort2 (numbers + left);
        } else if (right == left + 2) {
            sort3 (numbers + left);
        } else if (right == left + 3) {
            sort4 (numbers + left);
        }
        return;
    }
#endif

    lTmp = left;
    rTmp = right;

    pivot = medianOf3 (numbers + left, (right-left) >> 1, right-1-left);

    goto QStart;
    while (1) {
        do {
            right--;
            if (left >= right) goto QEnd;
            QStart:;
        } while (numbers[right] > pivot);
        numbers[left] = numbers[right];
        do { 
            left++;
            if (left >= right) {
                left = right;
                goto QEnd;
            }
        } while (numbers[ left] < pivot);
        numbers[right] = numbers[left];
    }
    QEnd:;
    numbers[left] = pivot;

    /* Only recurse the smaller partition */

    if (left-1 - lTmp <= rTmp - left - 1) {
        if (lTmp < left) quickSortRecurse (numbers,   lTmp, left-1);

        /* Set up for larger partition */
        left++;
        right = rTmp;
    } else {
        if (rTmp > left) quickSortRecurse (numbers, left+1,   rTmp);

        /* Set up for larger partition */
        right = left - 1;
        left = lTmp;
    }

    /* Rerun with larger partition (recursion not required.) */
    goto qsrStart;
}

void quickSort (int numbers[], int qty) {
    if (qty < 2) return;
    quickSortRecurse (numbers, 0, qty - 1);
}

/*
 *  Merge Sort
 */

static void mergesortInPlace (int * numbers, int * altNumbers, int qty);

/* Perform mergesort, but store results in altNumbers */

static void mergesortExchange (int * numbers, int * altNumbers, int qty) {
int half, i0, i1, i;

    if (qty == 2) {
        sortAlt2 (numbers, altNumbers);
        return;
    }
    if (qty == 3) {
        sortAlt3 (numbers, altNumbers);
        return;
    }

    half = (qty + 1)/2;

    mergesortInPlace (numbers, altNumbers, half);
    mergesortInPlace (numbers + half, altNumbers, qty - half);

    i0 = 0; i1 = half;

    for (i=0; i < qty; i++) {
        if (i1 >= qty || (i0 < half && numbers[i0] < numbers[i1])) {
            altNumbers[i] = numbers[i0];
            i0++;
        } else {
            altNumbers[i] = numbers[i1];
            i1++;
        }
    }
}

/* Perform mergesort and store results in numbers */

static void mergesortInPlace (int * numbers, int * altNumbers, int qty) {
int half, i0, i1, i;

#if 0
    if (qty == 2) {
        sort2 (numbers);
        return;
    }
    if (qty == 3) {
        sort3 (numbers);
        return;
    }
    if (qty == 4) {
        sort4 (numbers);
        return;
    }
#else
    if (qty <= 12) {
        insertSort (numbers, qty);
        return;
    }
#endif

    half = (qty + 1)/2;

    mergesortExchange (numbers, altNumbers, half);
    mergesortExchange (numbers + half, altNumbers + half, qty - half);

    i0 = 0; i1 = half;

    for (i=0; i < qty; i++) {
        if (i1 >= qty || (i0 < half && altNumbers[i0] < altNumbers[i1])) {
            numbers[i] = altNumbers[i0];
            i0++;
        } else {
            numbers[i] = altNumbers[i1];
            i1++;
        }
    }
}

#include <stdlib.h>

void mergeSort (int numbers[], int qty) {
int * tmpArray;

    if (qty <= 12) {
        insertSort (numbers, qty);
        return;
    }

    tmpArray = (int *) malloc (qty * sizeof (int));
    mergesortInPlace (numbers, tmpArray, qty);
    free (tmpArray);
}

/********************************
 * END OF PAUL'S IMPLEMENTATION *
 ********************************/

/*****************************************
 * BEGIN OF Jon Bentley'S IMPLEMENTATION *
 *****************************************/
typedef int DType;

int randint(int l, int u)
{  
  return l + (RAND_MAX*rand() + rand()) % (u-l+1);
}

inline void Bentleyswap(DType*x,int i, int j)
{  
  DType t = x[i];
  x[i] = x[j];
  x[j] = t;
}
void isort3(DType*data,int n)
{  
  int i, j;
  DType t;
  for (i = 1; i < n; i++) {
  t = data[i];
  for (j = i; j > 0 && data[j-1] > t; j--)
    data[j] = data[j-1];
  data[j] = t;
  }
}

/* qsort3 + randomization + isort small subarrays + swap inline */
int cutoff = 32;
void qsort4(DType*data, int l, int u)
{  
  int i, j;
  DType t;
  if (u - l < cutoff)
    return;
  Bentleyswap(data,l, randint(l, u));
  t = data[l];
  i = l;
  j = u+1;
  for (;;) {
    do i++; while (i <= u && data[i] < t);
    do j--; while (data[j] > t);
    if (i > j)
      break;
    //temp = data[i]; data[i] = data[j]; data[j] = temp;
    Bentleyswap(data,i, j);
  }
  Bentleyswap(data,l, j);
  qsort4(data,l, j-1);
  qsort4(data,j+1, u);
}

void qsort5(DType*data, int l, int u)
{  
  int i, j;
  DType t;
  if (u - l < cutoff){
    isort3(data+l,u-l+1);
    return;
  }
  Bentleyswap(data,l, randint(l, u));
  t = data[l];
  i = l;
  j = u+1;
  for (;;) {
    do i++; while (i <= u && data[i] < t);
    do j--; while (data[j] > t);
    if (i > j)
      break;
    //temp = data[i]; data[i] = data[j]; data[j] = temp;
    Bentleyswap(data,i, j);
  }
  Bentleyswap(data,l, j);
  qsort5(data,l, j-1);
  qsort5(data,j+1, u);
}

//// copy from stl
//template<class _RanIt> inline
//void Med3(_RanIt _First, _RanIt _Mid, _RanIt _Last)
//{ // sort median of three elements to middle
//  if (*_Mid < *_First)
//    std::iter_swap(_Mid, _First);
//
//  if (*_Last < *_Mid){ // swap middle and last, then test first again
//    std::iter_swap(_Last, _Mid);
//    if (*_Mid < * _First)
//      std::iter_swap(_Mid, _First);    
//  }
//}
//
//template<class _RanIt> inline
//void Median(_RanIt _First, _RanIt _Mid, _RanIt _Last)
//{ // sort median element to middle
//  if (40 < _Last - _First){ // median of nine
//    size_t _Step = (_Last - _First + 1) / 8;
//    Med3(_First, _First + _Step, _First + 2 * _Step);
//    Med3(_Mid - _Step, _Mid, _Mid + _Step);
//    Med3(_Last - 2 * _Step, _Last - _Step, _Last);
//    Med3(_First + _Step, _Mid, _Last - _Step);
//  }else
//    Med3(_First, _Mid, _Last);
//}

template<class _RanIt> inline
_RanIt Med3(_RanIt _First, _RanIt _Mid, _RanIt _Last)
{ 
  return (*_First < *_Mid)?
            ((*_Mid < *_Last)? _Mid: (*_First < *_Last) ? _Last:_First):
            ((*_Mid > *_Last)? _Mid: (*_First < *_Last) ? _First:_Last);
}

template<class _RanIt> inline
_RanIt Median(_RanIt _First, _RanIt _Mid, _RanIt _Last)
{ // sort median element to middle
  if (40 < _Last - _First){ // median of nine
    size_t _Step = (_Last - _First + 1) / 8;
    _RanIt p1 = Med3(_First, _First + _Step, _First + 2 * _Step);
    _RanIt p2 = Med3(_Mid - _Step, _Mid, _Mid + _Step);
    _RanIt p3 = Med3(_Last - 2 * _Step, _Last - _Step, _Last);
    return Med3(p1,p2,p3);
  }else
    return Med3(_First, _Mid, _Last);
}
void qsort6(DType*data, int l, int u)
{  
  int i, j;
  DType t;
  if (u - l < cutoff){
    isort3(data+l,u-l+1);
    return;
  }
  int  Mid = l + (u + 1 - l) / 2;  // sort median to _Mid
  DType *pm = Med3(data+l, data+Mid, data+u);
  t = *pm; *pm=data[l]; data[l]=t; //swap

  i = l;
  j = u+1;
  for (;;) {
    do i++; while (i <= u && data[i] < t);
    do j--; while (data[j] > t);
    if (i > j)
      break;
    Bentleyswap(data,i, j);
  }
  Bentleyswap(data,l, j);
  qsort6(data,l, j-1);
  qsort6(data,j+1, u);
}

void qsort7(DType*data, int l, int u)
{  
  int i, j;
  DType t;
  
  if (u - l < cutoff){
    isort3(data+l,u-l+1);
    return;
  }

  //if (u <= l){return;}
  int  Mid = l + (u + 1 - l) / 2;  // sort median to _Mid
  DType *pm = Median(data+l, data+Mid, data+u);
  t = *pm; *pm=data[l]; data[l]=t; //swap

  i = l;
  j = u+1;
  for (;;) {
    do i++; while (i <= u && data[i] < t);
    do j--; while (data[j] > t);
    if (i > j)
      break;
    Bentleyswap(data,i, j);
  }
  Bentleyswap(data,l, j);
  qsort7(data,l, j-1);
  qsort7(data,j+1, u);
}


void qsort8(DType*data, int l, int r)
{  
  DType t;
  if (r - l < cutoff){
    isort3(data+l,r-l+1);
    return;
  }
  //if (r <= l){return;}
  int  Mid = l + (r + 1 - l) / 2;  // sort median to _Mid
  DType *pm = Median(data+l, data+Mid, data+r);
  t = *pm; *pm=data[r]; data[r]=t; //swap

  int i = l-1, j = r, p = l-1, q = r;

  for (;;){
    
    while (data[++i] < t);
    while (t < data[--j]) if (j == l) break;

    if(i>=j) break;
    Bentleyswap(data,i, j);

    if (data[i] == t) { p++; Bentleyswap(data, p, i); }
    if (t == data[j]) { q--; Bentleyswap(data,j, q); }
  }
  Bentleyswap(data,i, r);   j = i-1; i = i+1;

  for (int k = l;   k < p; k++, j--) Bentleyswap(data,k, j);
  for (int k = r-1; k > q; k--, i++) Bentleyswap(data,i, k);

  qsort8(data,l, j);
  qsort8(data,i, r);
}

/***************************************
 * END OF Jon Bentley'S IMPLEMENTATION *
 ***************************************/
struct intcmp_t {
    inline int operator() (int a, int b) const {
        return a < b? -1 : a > b? 1 : 0;
    }
};

int compare_int(int a, int b)
{
    return a < b? -1 : a > b? 1 : 0;
}
int compare(const void *a, const void *b)
{
    return *((int*)a) - *((int*)b);
}

void recur_msort(int n, int *b, int *buf)
{ 
  int *b0 = buf, *b1, *b2 ;
  int  n1, n2 ;

  n1 = n / 2 ; n2 = n - n1 ;
  b1 = b ; b2 = b + n1 ;

  if (n1 > 1) recur_msort(n1, b1, buf);
  if (n2 > 1) recur_msort(n2, b2, buf);

  if (*(b2-1) <= *b2) return;

  while (n1 > 0 && n2 > 0)
    if (*b1 < *b2)
      { n1-- ; *b0++  = *b1++ ; }
    else
      { n2-- ; *b0++  = *b2++ ; }

  if (n1 > 0) memcpy(b0, b1, n1 * sizeof (int)) ;
  memcpy(b, buf, (n - n2) * sizeof (int)) ;
}

// Comb sort, adapted from D codes on this page:
// http://www.digitalmars.com/d/archives/digitalmars/D/announce/Sorting_algorithms_benchmark_7661.html
template<class TYPE>
void algo_combsort(size_t n, TYPE a[])
{
    const double shrink_factor = 1.2473309501039786540366528676643474234433714124826;
    int do_swap;
    size_t i, gap = n;
    TYPE tmp;

    do {
        if (gap > 1) {
            gap = (size_t)(gap / shrink_factor);
            if (gap == 9 || gap == 10) gap = 11;
        }
        do_swap = 0;
        for (i = 0; i < n - gap; ++i) {
            size_t j = i + gap;
            if (a[j] < a[i]) {
                tmp = a[i]; a[i] = a[j]; a[j] = tmp;
                do_swap = 1;
            }
        }
    } while (do_swap || gap > 1);
}

void doSort(int* a, int left, int right);


enum dist_t
{
  sorted = 0, 
  randomized, 
  reversed, 
  partially_sorted_0, 
  partially_sorted_1,  
  unique_key_100000,
  unique_key_100,
  STATE_NUMBER,
};
struct DistEntry
{
  dist_t dist;
  const char *name;
};
DistEntry AllDistEntries[] =
{
  {sorted,             "     sorted"},
  {randomized,         "     random"},
  {reversed,           "   R sorted"},
  {partially_sorted_0, "   sort[10]"},
  {partially_sorted_1, " sort[1000]"},
  {unique_key_100000,  "10^5 U key"},
  {unique_key_100,     "10^2 U key"},
};

template <typename _RanIt>
void generate_test_datas(_RanIt _First, _RanIt _Last, dist_t dist)
{
  int i = 0;
  
  typedef iterator_traits<_RanIt>::value_type value_t;

  std::less<value_t> lt;
  std::greater<value_t> gt;

  int m = _Last-_First;
  if (dist ==  unique_key_100000)
    m = 100000;
  else if(dist ==  unique_key_100)
    m = 100;
  
  _RanIt first_tmp = _First;
  while (first_tmp != _Last)
    *first_tmp++ = (i++)%m;
  
  switch(dist)
  {
  case randomized:
  case unique_key_100000:
  case unique_key_100:
    std::random_shuffle(_First,_Last);
    break;
  case reversed:
    std::reverse(_First,_Last);
    break;
  case partially_sorted_0:
    {
      std::random_shuffle(_First,_Last);
      bool toggle = true;
      for(_RanIt i=_First; i < _Last; i+=10){
        if(toggle)
          std::stable_sort(i, i+10, lt);
        else
          std::stable_sort(i, i+10, gt);
        toggle = !toggle;
      }
    }
    break;
  case partially_sorted_1:
    {
      std::random_shuffle(_First,_Last);
      bool toggle = true;
      for(_RanIt i=_First; i < _Last; i+=1000){
        if(toggle)
          std::stable_sort(i, i+1000, lt);
        else
          std::stable_sort(i, i+1000, gt);
        toggle = !toggle;
      }
    }
    break;
  case sorted:
    break;
  }
}

enum sortalgo_t
{
  c_qsort,
  stl_sort,
  stl_stable_sort,
  stl_heap_sort,
  paul_qsort,
  paul_mergesort,
  paul_heapSort,
  Bentley_qsort,
  Bentley_qsort5,
  Bentley_qsort6,
  Bentley_qsort7,
  Bentley_qsort8,
  java_dual_pivot,
  java_timsort,

  Template_QSort,
  STLPort_Sort,
};

const int maxIter = 100;
struct BenchEntry
{
  sortalgo_t algo;
  const char *name;
  double timesPerRun[STATE_NUMBER][maxIter];
};

BenchEntry IntBenchEntries[] =
{
  //{c_qsort,         "       c qsort"},
  //{stl_sort,        "      c++ sort"},
  //{stl_stable_sort, "c++ stablesort"},
  //{stl_heap_sort,   "  c++ heapsort"},
  //{paul_qsort,       "    paul qsort"},
  //{paul_mergesort,   "paul mergesort"},
  //{paul_heapSort,   " paul heapsort"},
  //{Bentley_qsort,    " single i sort"},
  //{Bentley_qsort5,   " random pivot "},
  {Bentley_qsort6,   "  median of 3 "},
  {Bentley_qsort7,   "adaptive pivot"},
  {Bentley_qsort8,   "3way partition"},
  //{java_dual_pivot,  "    dual pivot"},
  {Template_QSort,   " template sort"},
  {STLPort_Sort,    " stl port sort"},
  //{java_timsort,    "       timsort"},
};

#ifndef SIZEOF_ARRAY
#define SIZEOF_ARRAY(x) ((sizeof(x))/(sizeof(x[0])))
#endif // SIZEOF_ARRAY


int intcomp(int *x, int *y)
{  
  return *x - *y;
}

inline static double run_sort(sortalgo_t runalgo, int * data, int size )
{
  clock_t t1 = clock();
  switch (runalgo){
  case c_qsort:
    qsort(data, size, sizeof(int), 
      (int (__cdecl *)(const void *,const void *)) intcomp);
    break;
  case stl_sort:
    std::sort(data,data+size);
    break;
  case stl_stable_sort:
    std::stable_sort(data,data+size);
    break;
  case stl_heap_sort:
    make_heap(data,data+size);
    sort_heap(data,data+size);
    break;
  case paul_qsort:
     quickSort(data,size);
    break;
  case paul_mergesort:
    mergeSort(data,size);
    break;
  case paul_heapSort:
    heapSort(data,size);
    break;
  case Bentley_qsort:
    qsort4(data,0,size-1);
    isort3(data,size);
    break;
  case Bentley_qsort5:
    qsort5(data,0,size-1);
    break;
  case Bentley_qsort6:
    qsort6(data,0,size-1);
    break;
  case Bentley_qsort7:
    qsort7(data,0,size-1);
    break;
  case Bentley_qsort8:
    qsort8(data,0,size-1);
    break;
  case java_dual_pivot:
    doSort(data, 0,size-1);
    break;
  case java_timsort:
    gfx::timsort(data,data+size);
    break;
  case Template_QSort:
    SortBench::QuickSort(data,data+size);
    break;
  case STLPort_Sort:
    SortBench_STLPORT::sort(data,data+size);
    break;
  }
  clock_t t2 = clock();
  return (double)(t2-t1)/CLOCKS_PER_SEC;
}

static void bench(int iter,int size)
{
  int * data = (int *) malloc(sizeof(int) * size);
  int * copied = (int *) malloc(sizeof(int) * size);
  int iter_tmp = iter;

  const int allRuns = iter * 
                      SIZEOF_ARRAY(IntBenchEntries) * 
                      SIZEOF_ARRAY(AllDistEntries);
  int curentRun = 0;
  for (int dist_i=0; dist_i < SIZEOF_ARRAY(AllDistEntries);dist_i++){

    dist_t dist = AllDistEntries[dist_i].dist;
    generate_test_datas(copied,copied+size,dist);

    for (int iter_i = 0 ; iter_i < iter; iter_i++)
      for(int i = 0; i < SIZEOF_ARRAY(IntBenchEntries);i++){
        cerr << "run " << AllDistEntries[dist_i].name
             << '/'    << IntBenchEntries[i].name 
             << " : "  << ++curentRun << '/' << allRuns 
             << " [";
        sortalgo_t runalgo = IntBenchEntries[i].algo;
        if ( (runalgo == paul_qsort ||runalgo ==  Bentley_qsort6 )&&
              dist == reversed   &&
               size >= 1000000){
          //paul's quick sort is quadric for reverse sorted data sequence.
          // median of 3 looks has issue on reversed sorted data sequence
         cerr<<"-]";
        }else{
          std::copy(copied,copied+size,data);
          IntBenchEntries[i].timesPerRun[dist][iter_i] = 
            run_sort(runalgo, data, size);

          bool IsOK = true;
          int i;
          for (i = 0; i < size-1; i++){
            if (data[i] > data[i+1]){
              IsOK = false;
              break;
            }
          }
          if(IsOK)
            cerr<<"OK]";
          else
            cerr<<"FAIL at "<<i<<"]";
        }
        cerr << '\n';
    }

  }
  free(data);
  free(copied);
}
int main(int argc, char *argv[])
{
    int N = 50000000;
    int Iter = 10;
    if (argc < 3) 
      fprintf(stderr, "Usage: %s [%d] [%d]\n", argv[0], N,Iter);
    if (argc > 1)    N = atoi(argv[1]);
    if (argc > 2) Iter = atoi(argv[2]);

    Iter = min(Iter,maxIter);

    bench(Iter,N);

    if (N == 1000000){
      cout<< "    size    \t" 
          << "   method   \t" ;
      for (int k =0; k < SIZEOF_ARRAY(AllDistEntries);k++)
          cout<<AllDistEntries[k].name << '\t';
      cout <<"\n";
    }
  
    for (int j = 0; j < Iter;j++)
      for(int i = 0;i < SIZEOF_ARRAY(IntBenchEntries);i++){
        cout<<N<<"    \t"<<IntBenchEntries[i].name<<'\t';
        for (int k =0; k < SIZEOF_ARRAY(AllDistEntries);k++) {
          dist_t dist =  AllDistEntries[k].dist;
          cout<<"    "<<IntBenchEntries[i].timesPerRun[dist][j] << '\t';
        }
        cout <<"\n";
    }
}