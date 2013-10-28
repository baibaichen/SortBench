#ifndef _QUICKSORT_H__
#define _QUICKSORT_H__

#include <algorithm>    // std::iter_swap
namespace SortBench{

  const int cutoff = 32;

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

  template<class _BidIt> inline
  void Insertion_sort(_BidIt _First, _BidIt _Last)
  {	// insertion sort [_First, _Last), using operator<
    typedef iterator_traits<_BidIt>::value_type value_t;

    for(_BidIt i = _First+1; i != _Last; ++i){      
      value_t tmp = *i;

      _BidIt j = i;
      for (; j != _First && (*(j-1)) > tmp; --j)
        *j = *(j-1);
      *j= tmp;

    }
  }

  template<class _BidIt,
  class _Ty> inline
    void _Insertion_sort1(_BidIt _First, _BidIt _Last, _Ty *)
  {	// insertion sort [_First, _Last), using operator<
    if (_First != _Last)
      for (_BidIt _Next = _First; ++_Next != _Last; )
      {	// order next element
        _BidIt _Next1 = _Next;
        _Ty _Val = std::move(*_Next);

        if (_Val < *_First)
        {	// found new earliest element, move to front
          std::move_backward(_First, _Next, ++_Next1);
          *_First = std::move(_Val);
        }
        else
        {	// look for insertion point after first
          for (_BidIt _First1 = _Next1;_Val <  *--_First1;_Next1 = _First1)
            *_Next1 = std::move(*_First1);	// move hole down
          *_Next1 = std::move(_Val);	// insert element in hole
        }
      }
  }

  template<class _Iter> inline
  typename std::iterator_traits<_Iter>::value_type *_Val_type(_Iter)
  {	// return value type from arbitrary argument
    return (0);
  }

  template<class _BidIt> inline
  void _Insertion_sort(_BidIt _First, _BidIt _Last)
  {	// insertion sort [_First, _Last), using operator<
    _Insertion_sort1(_First, _Last, _Val_type(_First));
  }


  template<class _RanIt> inline
  void QuickSort(_RanIt _First, _RanIt _Last)
  {	// order [_First, _Last), using operator<
    typedef iterator_traits<_RanIt>::reference reference_t;
    typedef iterator_traits<_RanIt>::value_type value_t;

    if( _Last - _First < cutoff )
      return Insertion_sort(_First,_Last);

    _RanIt Mid = _First + (_Last - _First) / 2;  // sort median to _Mid
    _RanIt pm = Median(_First, Mid, _Last-1);
    std::iter_swap(_First,pm);

    const value_t tmp = *_First;

    _RanIt i = _First;
    _RanIt j = _Last;
    for (;;){
      do ++i; while (i != _Last && *i < tmp);
      do --j; while (*j > tmp);
      if (i > j)
        break;
      std::iter_swap(i,j);
    }
    std::iter_swap(_First,j);

    QuickSort(_First,j);
    QuickSort(j+1,_Last);
  }
}
#endif // _QUICKSORT_H__
