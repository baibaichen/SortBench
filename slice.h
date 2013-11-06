#ifndef _SLICE_H__
#define _SLICE_H__

#include <string>

template<typename T,int N> 
struct slice {
  typedef std::char_traits<T>  _Traits;
  typedef size_t size_type;
  
  typedef T& reference;
  typedef const T & const_reference;

  T data_[N];
public:
  void resize(size_type _Newsize){};
  const_reference operator[](size_type n) const {return data_[n];}
  reference operator[](size_type n) { return data_[n];}
};


template<typename T, int N> inline
  bool operator<(
  const slice<T,N>& _Left, const slice<T,N>& _Right)
{	// test if string < string  
  return slice<T,N>::_Traits::compare(_Left.data_,_Right.data_,N) < 0;
}

template<typename T, int N> inline
  bool operator>(
  const slice<T,N>& _Left, const slice<T,N>& _Right)
{	// test if string < string  
  return (_Right < _Left);;
}

typedef slice<char, 16> sliceChar_16;

#endif // _SLICE_H__
