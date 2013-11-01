#ifndef _QUICKSORT_H__
#define _QUICKSORT_H__

#include <algorithm>    // std::iter_swap
#include <iterator>     // std::distance

namespace SortBench{

  const int cutoff = 32;

  template<typename _RanIt>
  inline _RanIt Med3(_RanIt __a, _RanIt __b, _RanIt __c)
  {
    if (*__a < *__b)
      if (*__b < *__c)
        return __b;
      else if (*__a < *__c)
        return __c;
      else
        return __a;
    else if (*__a < *__c)
      return __a;
    else if (*__b < *__c)
      return __c;
    else
      return __b;
  }
  //template<class _RanIt> inline
  //  _RanIt Med3(_RanIt _First, _RanIt _Mid, _RanIt _Last)
  //{ 
  //  return (*_First < *_Mid)?
  //    ((*_Mid < *_Last)? _Mid: (*_First < *_Last) ? _Last:_First):
  //    ((*_Mid > *_Last)? _Mid: (*_First < *_Last) ? _First:_Last);
  //}

  template<class _RanIt> inline
  _RanIt Median(_RanIt _First, _RanIt _Mid, _RanIt _Last)
  { 
    if (40 < _Last - _First){ 
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

    typedef iterator_traits<_RanIt>::value_type value_t;

    if(std::distance(_First,_Last) <= cutoff )
      return;// Insertion_sort(_First,_Last);

    _RanIt Mid = _First + (_Last - _First) / 2;  // sort median to _Mid
    _RanIt pm = Median(_First, Mid, _Last-1);
    std::iter_swap(_First,pm);

    value_t pivot = *_First;

    _RanIt forwardI = _First;
    _RanIt cut = _Last;
    for (;;){
      do ++forwardI; while (forwardI < _Last && *forwardI < pivot);
      do --cut; while (pivot < *cut );
      if (forwardI >= cut)
        break;
      std::iter_swap(forwardI,cut);
    }
    std::iter_swap(_First,cut);

    QuickSort(_First,cut);
    QuickSort(cut+1,_Last);
  }

  template<class _RanIt> inline
  void STLPORT__QuickSort(_RanIt _First, _RanIt _Last)
  {	// order [_First, _Last), using operator<

    typedef iterator_traits<_RanIt>::value_type value_t;

    if(std::distance(_First,_Last) <= cutoff )
      return;// Insertion_sort(_First,_Last);

    _RanIt Mid = _First + (_Last - _First) / 2;
    value_t pivot = *(Med3(_First, Mid, _Last-1));

    _RanIt cut = _First - 1;
    _RanIt backwardI = _Last;
    for (;;){
      do ++cut; while (*cut < pivot );
      do --backwardI; while (pivot < *backwardI );
      if (cut >= backwardI)
        break;
      std::iter_swap(cut,backwardI);
    }

    STLPORT__QuickSort(_First,cut);
    STLPORT__QuickSort(cut,_Last);
  }
}

namespace SortBench_STLPORT{

  enum { _S_threshold = 32 };

  template<typename _Tp>
  inline const _Tp&
  __median3(const _Tp& __a, const _Tp& __b, const _Tp& __c)
  {
    // concept requirements
    if (__a < __b)
      if (__b < __c)
        return __b;
      else if (__a < __c)
        return __c;
      else
        return __a;
    else if (__a < __c)
      return __a;
    else if (__b < __c)
      return __c;
    else
      return __b;
  }
  
  /// This is a helper function...
  template<typename _RandomAccessIterator, typename _Tp>
  _RandomAccessIterator 
  __unguarded_partition(_RandomAccessIterator __first,
                        _RandomAccessIterator __last, 
                        _Tp __pivot)
  {
    while (true) {
      while (*__first < __pivot)
        ++__first;
      --__last;
      while (__pivot < *__last)
        --__last;
      if (!(__first < __last))
        return __first;
      std::iter_swap(__first, __last);
      ++__first;
    }
  }
  
  template<typename _RandomAccessIterator, typename _Size>
  void __introsort_loop(_RandomAccessIterator __first,
                        _RandomAccessIterator __last,
                        _Size __depth_limit)
  {
    typedef typename iterator_traits<_RandomAccessIterator>::value_type
      _ValueType;
    
    using SortBench::Median;

    while (__last - __first > int(_S_threshold)){
      if (__depth_limit == 0){
        std::partial_sort(__first, __last, __last);
        return;
      }
      --__depth_limit;

      //_RandomAccessIterator __cut =
      //          __unguarded_partition(__first, __last,
      //                        _ValueType(*Median(__first,
      //                                            (__first+ (__last- __first)/ 2),
      //                                            (__last- 1))));
      _RandomAccessIterator __cut =        
        __unguarded_partition(__first, __last,
                              _ValueType(__median3(*__first,
                                                  *(__first+ (__last- __first)/ 2),
                                                  *(__last- 1))));
      __introsort_loop(__cut, __last, __depth_limit);
      __last = __cut;
    }
  }

  /// This is a helper function for the sort routines.  Precondition: __n > 0.
  template<typename _Size>
  inline _Size
  __lg(_Size __n)
  {
    _Size __k;
    for (__k = 0; __n != 0; __n >>= 1)
      ++__k;
    return __k - 1;
  }

  /// This is a helper function for the sort routine.
  template<typename _RandomAccessIterator, typename _Tp>
  void __unguarded_linear_insert(_RandomAccessIterator __last, _Tp __val)
  {
    _RandomAccessIterator __next = __last;
    --__next;
    while (__val < *__next)
    {
      *__last = *__next;
      __last = __next;
      --__next;
    }
    *__last = __val;
  }

  template<typename _RandomAccessIterator>
  void
  __insertion_sort(_RandomAccessIterator __first,
                   _RandomAccessIterator __last)
  {
    if (__first == __last)
      return;

    for (_RandomAccessIterator __i = __first + 1; __i != __last; ++__i)
    {
      typename iterator_traits<_RandomAccessIterator>::value_type __val = *__i;

      if (__val < *__first)
      {
        std::copy_backward(__first, __i, __i + 1);
        *__first = __val;
      }
      else
        __unguarded_linear_insert(__i, __val);
    }
  }
  
  /// This is a helper function for the sort routine.
  template<typename _RandomAccessIterator>
  inline void
  __unguarded_insertion_sort(_RandomAccessIterator __first,
                             _RandomAccessIterator __last)
  {
    typedef typename iterator_traits<_RandomAccessIterator>::value_type
      _ValueType;

    for (_RandomAccessIterator __i = __first; __i != __last; ++__i)
          __unguarded_linear_insert(__i, _ValueType(*__i));
  }

  /// This is a helper function for the sort routine.
  template<typename _RandomAccessIterator>
  void
  __final_insertion_sort(_RandomAccessIterator __first,
                         _RandomAccessIterator __last)
  {
    if (__last - __first > int(_S_threshold))
    {
      __insertion_sort(__first, __first + int(_S_threshold));
      __unguarded_insertion_sort(__first + int(_S_threshold), __last);
    }
    else
      __insertion_sort(__first, __last);
  }

  template<typename _RandomAccessIterator>
  inline void
  sort(_RandomAccessIterator __first, _RandomAccessIterator __last)
  {
    typedef typename iterator_traits<_RandomAccessIterator>::value_type
      _ValueType;

    if (__first != __last)
    {
      __introsort_loop(__first, __last,__lg(__last - __first) * 2);
      __final_insertion_sort(__first, __last);
    }
  }

  //////////

  /// This is a helper function for the sort routine.
  template<typename _RandomAccessIterator, typename _Tp, typename _Compare>
  void  __unguarded_linear_insert(_RandomAccessIterator __last, 
                                  _Tp __val,
                                  _Compare __comp)
  {
    _RandomAccessIterator __next = __last;
    --__next;
    while (__comp(__val, *__next))
    {
      *__last = *__next;
      __last = __next;
      --__next;
    }
    *__last = __val;
  }

  /// This is a helper function for the sort routine.
  template<typename _RandomAccessIterator, typename _Compare>
  void __insertion_sort(_RandomAccessIterator __first,
                        _RandomAccessIterator __last, 
                        _Compare __comp)
  {
    if (__first == __last) return;

    for (_RandomAccessIterator __i = __first + 1; __i != __last; ++__i)
    {
      typename iterator_traits<_RandomAccessIterator>::value_type __val = *__i;
      
      if (__comp(__val, *__first))
      {
        std::copy_backward(__first, __i, __i + 1);
        *__first = __val;
      }
      else
        __unguarded_linear_insert(__i, __val, __comp);
    }
  }


  /// This is a helper function for the sort routine.
  template<typename _RandomAccessIterator, typename _Compare>
  inline void __unguarded_insertion_sort(_RandomAccessIterator __first,
                                         _RandomAccessIterator __last, 
                                         _Compare __comp)
  {
    typedef typename iterator_traits<_RandomAccessIterator>::value_type
      _ValueType;

    for (_RandomAccessIterator __i = __first; __i != __last; ++__i)
      __unguarded_linear_insert(__i, _ValueType(*__i), __comp);
  }

  /// This is a helper function for the sort routine.
  template<typename _RandomAccessIterator, typename _Compare>
  void
  __final_insertion_sort(_RandomAccessIterator __first,
                        _RandomAccessIterator __last, 
                        _Compare __comp)
  {
    if (__last - __first > int(_S_threshold))
    {
      __insertion_sort(__first, __first + int(_S_threshold), __comp);
      __unguarded_insertion_sort(__first + int(_S_threshold), __last,__comp);
    }
    else
      __insertion_sort(__first, __last, __comp);
  }

  template <class _Tp, class _Compare>
  const _Tp& __median3(const _Tp& __a, 
                      const _Tp& __b, 
                      const _Tp& __c, 
                      _Compare __comp) 
  {
      if (__comp(__a, __b)) {
          if (__comp(__b, __c)) {
              return __b;
          }
          else if (__comp(__a, __c)) {
              return __c;
          }
          else
            return __a;
      }
      else if (__comp(__a, __c)) {
          return __a;
      }
      else if (__comp(__b, __c)) {
          return __c;
      }
      else
        return __b;
  }

  template <class _RandomAccessIter, class _Tp, class _Compare>
  _RandomAccessIter
  __unguarded_partition(_RandomAccessIter __first,
                        _RandomAccessIter __last, 
                        _Tp __pivot, 
                        _Compare __comp) 
  {
    for (;;) {
      while (__comp(*__first, __pivot)) {++__first;}
        
      --__last;
      while (__comp(__pivot, *__last)) {--__last;}
      if (!(__first < __last))
        return __first;
      iter_swap(__first, __last);
      ++__first;
    }
  }

  template <class _RandomAccessIter, class _Size, class _Compare>
  void __introsort_loop(_RandomAccessIter __first,
                        _RandomAccessIter __last, 
                        _Size __depth_limit, 
                        _Compare __comp) 
  {
    typedef typename iterator_traits<_RandomAccessIter>::value_type
      _ValueType;
    while (__last - __first > int(_S_threshold)) {
      if (__depth_limit == 0) {
        std::partial_sort(__first, __last, __last, __comp);
        return;
      }
      --__depth_limit;
      _RandomAccessIter __cut =
        __unguarded_partition(__first, __last, 
                              _ValueType(__median3(*__first,
                                                *(__first + (__last - __first)/2),
                                                *(__last - 1), __comp)),
                              __comp);
      __introsort_loop(__cut, __last, __depth_limit, __comp);
      __last = __cut;
    }
  }

  template <class _RandomAccessIter, class _Compare>
  void sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp) 
  {
    if (__first != __last) {
      __introsort_loop(__first, __last, __lg(__last - __first) * 2, __comp);
      __final_insertion_sort(__first, __last, __comp);
    }
  }
}
#endif // _QUICKSORT_H__