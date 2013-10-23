#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <functional>
#include <iostream>

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


enum state_t
{
  sorted = 0, 
  randomized, 
  reversed, 
  partially_sorted_0, 
  partially_sorted_1,  
  unique_key_100000,
  unique_key_1000,
  STATE_NUMBER,
};


//enum dist_t{
//  sawtooth,
//  rand,
//  stagger,
//  plateau,
//  shuffle
//};
template <typename _RanIt>
void generate_test_datas(_RanIt _First, _RanIt _Last, state_t state)
{
  int i = 0;
  
  typedef iterator_traits<_RanIt>::value_type value_t;

  std::less<value_t> lt;
  std::greater<value_t> gt;

  int m = _Last-_First;
  if (state ==  unique_key_100000)
    m = 100000;
  else if(state ==  unique_key_1000)
    m = 1000;
  
  _RanIt first_tmp = _First;
  while (first_tmp != _Last)
    *first_tmp++ = (i++)%m;
  
  switch(state)
  {
  case randomized:
  case unique_key_100000:
  case unique_key_1000:
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


  //const int size = _Last - _First;
  //const int m = size *2;
  //int i,j = 0;
  //while (_First != _Last){
  //  switch (state)
  //  {
  //  case sawtooth:
  //    *_First = i % m; 
  //    break;
  //  case rand:
  //    *_First = i;
  //    break;
  //  case stagger:
  //    *_First = (i*m + i) % size;
  //    break;
  //  case plateau:

  //    break;
  //  case shuffle:
  //    break;
  //  default:
  //    break;
  //  }
  //}
}
//template <typename value_t>
//static void bench(int size, state_t state)
//{
//  value_t * data = (value_t *) malloc(sizeof(value_t) * size);
//  generate_test_datas(data,data+size,state);
//
//  free(data);
//}

enum sortmethod_t
{
  c_qsort,
  stl_sort,
  stl_stable_sort,
  stl_heap_sort,
  paul_qsort,
  paul_mergesort,
  paul_heapSort,
  java_dual_pivot,
  java_timsort,
};
struct BenchEntry
{
  sortmethod_t method;
  const char *name;
  double sumOfTimes[STATE_NUMBER];
  double sumOfSquareTimes[STATE_NUMBER];
};

BenchEntry IntBenchEntries[] =
{
  //{c_qsort,         "       c qsort"},
  {stl_sort,        "      c++ sort"},
  {stl_stable_sort, "c++ stablesort"},
  //{stl_heap_sort,   "  c++ heapsort"},
  {paul_qsort,      "    paul qsort"},
  {paul_mergesort,   "paul mergesort"},
  //{paul_heapSort,   " paul heapsort"},
  {java_dual_pivot, "    dual pivot"},
  {java_timsort,    "       timsort"},
};

int intcomp(int *x, int *y)
{	return *x - *y;
}

static void bench(int size, state_t state)
{
    int * data = (int *) malloc(sizeof(int) * size);

    for(int i = 0; 
        i < sizeof(IntBenchEntries)/sizeof(IntBenchEntries[0]); 
        i++){
      generate_test_datas(data,data+size,state);
      IntBenchEntries[i].sumOfTimes[state]=0;
      cerr<<"run " << IntBenchEntries[i].name;

      clock_t t1 = clock();
      switch (IntBenchEntries[i].method)
      {
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
        if(state != reversed)
          quickSort(data,size);
        break;
      case paul_mergesort:
        mergeSort(data,size);
        break;
      case paul_heapSort:
        heapSort(data,size);
        break;
      case java_dual_pivot:
        doSort(data, 0,size-1);
        break;
      case java_timsort:
        gfx::timsort(data,data+size);
        break;
      }
      clock_t t2 = clock();

      IntBenchEntries[i].sumOfTimes[state] = (double)(t2-t1)/CLOCKS_PER_SEC;

      cerr <<", time = " << IntBenchEntries[i].sumOfTimes[state] <<" s \n";
    }

    free(data);
}
int main(int argc, char *argv[])
{
    int N = 50000000;
    //int *array, *temp;
    //clock_t t1, t2;
    if (argc == 1) fprintf(stderr, "Usage: %s [%d]\n", argv[0], N);
    if (argc > 1) N = atoi(argv[1]);

    bench(N,sorted);
    bench(N,randomized);
    bench(N,reversed);
    bench(N,partially_sorted_0);
    bench(N,partially_sorted_1); 
    bench(N,unique_key_100000); 
    bench(N,unique_key_1000); 
    cout<< "  size    \t" 
        << " method   \t" 
        << " sorted   \t" 
        << "randomized\t" 
        << "reversed  \t"
        << "partially sorted 0\t"
        << "partially sorted 1\t"
        << "unique key 100000 0\t"
        << "unique key 1000 1\t"
        <<"\n";

    for(int i = 0; 
      i < sizeof(IntBenchEntries)/sizeof(IntBenchEntries[0]); 
      i++){
      cout<<N<<'\t'
        <<IntBenchEntries[i].name<<'\t'
        <<IntBenchEntries[i].sumOfTimes[sorted]<<'\t'
        <<IntBenchEntries[i].sumOfTimes[randomized]<<'\t'
        <<IntBenchEntries[i].sumOfTimes[reversed]<<'\t'
        <<IntBenchEntries[i].sumOfTimes[partially_sorted_0]<<'\t'
        <<IntBenchEntries[i].sumOfTimes[partially_sorted_1]<<'\t'
        <<IntBenchEntries[i].sumOfTimes[unique_key_100000]<<'\t'
        <<IntBenchEntries[i].sumOfTimes[unique_key_1000]<<'\t'
        <<'\n';
    }

    //temp = (int*)malloc(sizeof(int) * N);
    //array = (int*)malloc(sizeof(int) * N);

    //srand48(11);
    //for (i = 0; i < N; ++i) array[i] = (int)lrand48();
    //t1 = clock();
    //sort(array, array+N);
    //t2 = clock();
    //fprintf(stderr, "STL introsort: %.3lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);
    //t1 = clock();
    //sort(array, array+N);
    //t2 = clock();
    //fprintf(stderr, "STL introsort (sorted): %.3lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);

    //srand48(11);
    //for (i = 0; i < N; ++i) array[i] = (int)lrand48();
    //t1 = clock();
    //stable_sort(array, array+N);
    //t2 = clock();
    //fprintf(stderr, "STL stablesort: %.3lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);
    //t1 = clock();
    //stable_sort(array, array+N);
    //t2 = clock();
    //fprintf(stderr, "STL stablesort (sorted): %.3lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);

    //srand48(11);
    //for (i = 0; i < N; ++i) array[i] = (int)lrand48();
    //t1 = clock();
    //algo_combsort(N, array);
    //t2 = clock();
    //fprintf(stderr, "combsort: %.3lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);
    //for (i = 0; i < N-1; ++i) {
    //    if (array[i] > array[i+1]) {
    //        fprintf(stderr, "Bug in combsort!\n");
    //        exit(1);
    //    }
    //}

    //srand48(11);
    //for (i = 0; i < N; ++i) array[i] = (int)lrand48();
    //t1 = clock();
    //qsort(array, N, sizeof(int), compare);
    //t2 = clock();
    //fprintf(stderr, "libc qsort: %.3lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);

    //srand48(11);
    //for (i = 0; i < N; ++i) array[i] = (int)lrand48();
    //t1 = clock();
    //algo_sort(N, array);
    //t2 = clock();
    //fprintf(stderr, "my introsort: %.3lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);
    //for (i = 0; i < N-1; ++i) {
    //    if (array[i] > array[i+1]) {
    //        fprintf(stderr, "Bug in intro_sort!\n");
    //        exit(1);
    //    }
    //}
    //t1 = clock();
    //algo_sort(N, array);
    //t2 = clock();
    //fprintf(stderr, "introsort (sorted): %.3lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);

    //srand48(11);
    //for (i = 0; i < N; ++i) array[i] = (int)lrand48();
    //t1 = clock();
    //algo_mergesort(N, array);
    //t2 = clock();
    //fprintf(stderr, "iterative mergesort: %.3lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);
    //for (i = 0; i < N-1; ++i) {
    //    if (array[i] > array[i+1]) {
    //        fprintf(stderr, "Bug in merge_sort!\n");
    //        exit(1);
    //    }
    //}
    //t1 = clock();
    //algo_mergesort(N, array);
    //t2 = clock();
    //fprintf(stderr, "iterative mergesort (sorted): %.3lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);

    //srand48(11);
    //for (i = 0; i < N; ++i) array[i] = (int)lrand48();
    //t1 = clock();
    //recur_msort(N, array, temp);
    //t2 = clock();
    //fprintf(stderr, "recursive mergesort: %.3lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);
    //for (i = 0; i < N-1; ++i) {
    //    if (array[i] > array[i+1]) {
    //        fprintf(stderr, "Bug in recur_sort!\n");
    //        exit(1);
    //    }
    //}

    //srand48(11);
    //for (i = 0; i < N; ++i) array[i] = (int)lrand48();
    //t1 = clock();
    //algo_heap_make(array, N);
    //algo_heap_sort(array, N);
    //t2 = clock();
    //fprintf(stderr, "my heapsort: %.3lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);
    //for (i = 0; i < N-1; ++i) {
    //    if (array[i] > array[i+1]) {
    //        fprintf(stderr, "Bug in heap_sort!\n");
    //        exit(1);
    //    }
    //}
    //t1 = clock();
    //algo_heap_make(array, N);
    //algo_heap_sort(array, N);
    //t2 = clock();
    //fprintf(stderr, "heapsort (sorted): %.3lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);

    //srand48(11);
    //for (i = 0; i < N; ++i) array[i] = (int)lrand48();
    //t1 = clock();
    //// icc seems to be able to optimize "algo_sort(N, array,
    //// compare_int)", but not the following form. Unfortunately, g++-3.3
    //// only accepts the following form.
    //algo_sort(N, array, &compare_int);
    //t2 = clock();
    //fprintf(stderr, "my isort (func call): %.3lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);
    //for (i = 0; i < N-1; ++i) {
    //    if (array[i] > array[i+1]) {
    //        fprintf(stderr, "Bug in introsort (with func call)!\n");
    //        exit(1);
    //    }
    //}

    //srand48(11);
    //for (i = 0; i < N; ++i) array[i] = (int)lrand48();
    //t1 = clock();
    //algo_sort(N, array, intcmp_t());
    //t2 = clock();
    //fprintf(stderr, "my isort (template func): %.3lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);
    //for (i = 0; i < N-1; ++i) {
    //    if (array[i] > array[i+1]) {
    //        fprintf(stderr, "Bug in introsort (with template func)!\n");
    //        exit(1);
    //    }
    //}

    //srand48(11);
    //for (i = 0; i < N; ++i) array[i] = (int)lrand48();
    //t1 = clock();
    //heapSort(array, N);
    //t2 = clock();
    //fprintf(stderr, "Paul's heapsort: %.3lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);
    //for (i = 0; i < N-1; ++i) {
    //    if (array[i] > array[i+1]) {
    //        fprintf(stderr, "Bug in intro_sort!\n");
    //        exit(1);
    //    }
    //}

    //srand48(11);
    //for (i = 0; i < N; ++i) array[i] = (int)lrand48();
    //t1 = clock();
    //quickSort(array, N);
    //t2 = clock();
    //fprintf(stderr, "Paul's quicksort: %.3lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);
    //for (i = 0; i < N-1; ++i) {
    //    if (array[i] > array[i+1]) {
    //        fprintf(stderr, "Bug in intro_sort!\n");
    //        exit(1);
    //    }
    //}

    //srand48(11);
    //for (i = 0; i < N; ++i) array[i] = (int)lrand48();
    //t1 = clock();
    //mergeSort(array, N);
    //t2 = clock();
    //fprintf(stderr, "Paul's mergesort: %.3lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);
    //for (i = 0; i < N-1; ++i) {
    //    if (array[i] > array[i+1]) {
    //        fprintf(stderr, "Bug in intro_sort!\n");
    //        exit(1);
    //    }
    //}

    //srand48(11);
    //for (i = 0; i < N; ++i) array[i] = (int)lrand48();
    //t1 = clock();
    //doSort(array, 0,N-1);
    //t2 = clock();
    //fprintf(stderr, "java's dual pivot sort: %.3lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);
    //for (i = 0; i < N-1; ++i) {
    //  if (array[i] > array[i+1]) {
    //    fprintf(stderr, "Bug in intro_sort!\n");
    //    exit(1);
    //  }
    //}

    //t1 = clock();
    //doSort(array, 0,N-1);
    //t2 = clock();
    //fprintf(stderr, "java's dual pivot sort (sorted): %.3lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);


    //free(array); free(temp);
    //return 0;
}