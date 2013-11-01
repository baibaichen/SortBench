#ifndef _DUALPIVOTQUICK_H__
#define _DUALPIVOTQUICK_H__

#include <algorithm>

namespace SortBenchDual{

  const int cutoff = 32;

  template <typename _RanIt>
  void dp_qsort(_RanIt first, _RanIt last,int div){

    typedef iterator_traits<_RanIt>::value_type value_t;

    intptr_t len = last - first;
    if( len < cutoff )
      return; 

    intptr_t third = len / div;

    //medians
    _RanIt m1 = first + third;
    _RanIt m2 = last  - third;

    if (*m1 < *m2){
      std::iter_swap(m1,first);
      std::iter_swap(m2,last-1);
    }else{
      std::iter_swap(m1,last-1);
      std::iter_swap(m2,first);
    }
    value_t pivot1 = *first;
    value_t pivot2 = *(last-1);

    _RanIt lessI  = first + 1;
    _RanIt greatI = last  - 2;

    //sorting
    for (_RanIt k = lessI; k <= greatI; ++k){
      if (*k < pivot1)
        std::iter_swap(k,lessI++);
      else if (pivot2 < *k){
        while ( k < greatI && pivot2 < *greatI)
          --greatI;
        std::iter_swap(k,greatI--);
        if (*k < pivot1)
          std::iter_swap(k,lessI++);
      }
    }
    std::iter_swap(lessI-1, first);
    std::iter_swap(greatI+1,last-1);

    if(greatI - lessI < 13)
      div++;

    // subarrays
    dp_qsort(first, lessI - 1, div);
    dp_qsort(greatI + 2, last, div);
   
    if(pivot1 < pivot2)
      dp_qsort(lessI,greatI+1,div);
  }

}
#endif // _DUALPIVOTQUICK_H__
