#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <cstdint>
#include "quicksort.h"

struct random
{
  uint32_t seed;

  uint32_t next(){
    seed = seed * 1103515245L + 12345;
    return seed;
  }

  random (uint32_t s)
  {
    seed = s;
  }
};

uint64_t  g_comp_with_count;
template<class _Ty>
struct less_with_count
  : public std::binary_function<_Ty, _Ty, bool>
{	// functor for operator<
  bool operator()(const _Ty& _Left, const _Ty& _Right) const
  {	
    g_comp_with_count++;
    return (_Left < _Right);
  }
};

uint64_t  g_swap_with_count;
template<class _FwdIt1,class _FwdIt2> 
inline void count_iter_swap(_FwdIt1 _Left, _FwdIt2 _Right)
{	
  g_swap_with_count++;
  std::swap(*_Left, *_Right);
}

template <typename _RanIt>
void generate_random_sequnce(_RanIt _First, _RanIt _Last, uint32_t seed)
{
  random r(seed);
  _RanIt first_tmp = _First;
  while (first_tmp != _Last)
    *first_tmp++ = r.next();
}

const int cutoff  = 32;
//template<class _RanIt,typename _Compare>
//inline _RanIt Med3(_RanIt _First, _RanIt _Mid, _RanIt _Last, _Compare __comp)
//{ 
//  return (__comp(*_First , *_Mid))?
//    (__comp(*_Mid , *_Last)? _Mid: __comp(*_First , *_Last) ? _Last:_First):
//    (__comp(*_Last, *_Mid )? _Mid: __comp(*_First , *_Last) ? _First:_Last);
//}

template<typename _RanIt ,typename _Compare>
inline _RanIt Med3(_RanIt __a, _RanIt __b, _RanIt __c, _Compare __comp)
{
  if (__comp(*__a ,*__b))
    if (__comp(*__b , *__c))
      return __b;
    else if (__comp(*__a , *__c))
      return __c;
    else
      return __a;
  else if (__comp(*__a , *__c))
    return __a;
  else if (__comp(*__b , *__c))
    return __c;
  else
    return __b;
}

template<class _RanIt,typename _Compare> 
inline _RanIt Median9(_RanIt _First, _RanIt _Mid, _RanIt _Last, _Compare __comp)
{ // sort median element to middle
  if (40 < _Last - _First){ // median of nine
    size_t _Step = (_Last - _First + 1) / 8;
    _RanIt p1 = Med3(_First, _First + _Step, _First + 2 * _Step,__comp);
    _RanIt p2 = Med3(_Mid - _Step, _Mid, _Mid + _Step,__comp);
    _RanIt p3 = Med3(_Last - 2 * _Step, _Last - _Step, _Last,__comp);
    return Med3(p1,p2,p3,__comp);
  }else
    return Med3(_First, _Mid, _Last,__comp);
}


template<class _RanIt,typename _Compare>
inline void STLPORT_QuickSort(_RanIt _First, _RanIt _Last, _Compare __comp)
{	// order [_First, _Last), using __comp

  typedef iterator_traits<_RanIt>::value_type value_t;

  if(std::distance(_First,_Last) <= cutoff )
    return ;

  _RanIt Mid = _First + (_Last - _First) / 2;
  //value_t pivot = *(Median9(_First, Mid, _Last-1,__comp));
  value_t pivot = SortBench_STLPORT::__median3(*_First,*Mid,*(_Last-1),__comp);

  _RanIt cut = _First - 1;
  _RanIt backwardI = _Last;
  for (;;){
    do ++cut; while (__comp(*cut, pivot));
    do --backwardI; while (__comp(pivot , *backwardI));
    if (cut >= backwardI)
      break;
    count_iter_swap(cut,backwardI);
  }

  STLPORT_QuickSort(_First,cut,__comp);
  STLPORT_QuickSort(cut,_Last,__comp);
}

template<class _RanIt,typename _Compare>
inline void QuickSort(_RanIt _First, _RanIt _Last, _Compare __comp)
{	// order [_First, _Last), using __comp

  typedef iterator_traits<_RanIt>::value_type value_t;

  if(std::distance(_First,_Last) <= cutoff )
    return;

  less_with_count<_RanIt> lt;
  _RanIt Mid = _First + (_Last - _First) / 2;  
  _RanIt pm = Median9(_First, Mid, _Last-1,__comp);
  count_iter_swap(_First,pm);

  value_t pivot = *_First;

  _RanIt forwardI = _First;
  _RanIt cut = _Last;
  for (;;){
    do ++forwardI; while (lt(forwardI , _Last) && __comp(*forwardI , pivot));
    do --cut; while (__comp(pivot,*cut));
    if (forwardI >= cut)
      break;
    count_iter_swap(forwardI,cut);
  }
  count_iter_swap(_First,cut);

  QuickSort(_First,cut,__comp);
  QuickSort(cut+1,_Last,__comp);
}

inline static void resetCounterAndSequence(uint32_t * copied, int size, uint32_t * data )
{
  std::copy(copied,copied+size,data);
  g_comp_with_count = 0;
  g_swap_with_count = 0;
}


using namespace std;

template<class _RanIt>
inline void verify(_RanIt _First, _RanIt _Last)
{
  bool IsOK = true;
  for (_RanIt x = _First; x < _Last-1 ; x++)
  {
    if(*x > *(x+1)){
      IsOK = false;
      break;
    }
  }
  if(IsOK)
    cout<<"[P],";
  else
    cout<<"[F],";
}

static void bench(int iter,int size)
{
  uint32_t * data = (uint32_t *) malloc(sizeof(uint32_t) * size);
  uint32_t * copied = (uint32_t *) malloc(sizeof(uint32_t) * size);

  for (int i = 0; i <iter;i++)
  {
    cerr << "run "<< i << '/' << iter << '\n';
    generate_random_sequnce(copied,copied+size,i);

    //resetCounterAndSequence(copied, size, data);
    //SortBench_STLPORT::sort(data,data+size,less_with_count<uint32_t>());
    //cout << i<<",STLPORTSORT,";
    //verify(data,data+size);
    //cout << g_comp_with_count
    //     <<','
    //     << g_swap_with_count
    //     <<'\n';

    resetCounterAndSequence(copied, size, data);
    {
      clock_t t1 = clock();
      STLPORT_QuickSort(data,data+size,less_with_count<uint32_t>());
      SortBench_STLPORT::__final_insertion_sort(data,data+size,less_with_count<uint32_t>());
      clock_t t2 = clock();

      cout << i<<",STLPORTSORT,";
      verify(data,data+size);
      cout << g_comp_with_count
        <<','
        << g_swap_with_count
        <<','
        <<((double)(t2-t1)/CLOCKS_PER_SEC)
        <<'\n';
    }

    resetCounterAndSequence(copied, size, data);
    {
      clock_t t1 = clock();
      QuickSort(data,data+size,less_with_count<uint32_t>());
      SortBench_STLPORT::__final_insertion_sort(data,data+size,less_with_count<uint32_t>());
      clock_t t2 = clock();

      cout << i<<",Median of 3 median,";
      verify(data,data+size);
      cout << g_comp_with_count
        <<','
        << g_swap_with_count
        <<','
        <<((double)(t2-t1)/CLOCKS_PER_SEC)
        <<'\n';
    }
  }
  free(data);
  free(copied);
}


int main(int argc, char *argv[])
{
  bench(6,1024*1024*100);
  return 0;
}