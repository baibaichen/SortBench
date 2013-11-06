#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <cstdint>
#include <string>
#include <memory>
#include "random.h"
#include "slice.h"

using namespace std;

template <typename DType>
inline void Bentleyswap(DType*x,int i, int j)
{  
  DType t = x[i];
  x[i] = x[j];
  x[j] = t;
}

template <typename _RanIt>
void generate_test_datas(_RanIt _First, _RanIt _Last, int)
{
  typedef iterator_traits<_RanIt>::value_type value_t;
  value_t i = 0;
  while (_First != _Last)
    *_First++ = i++;
}

void generate_test_datas(string* first,string*last,int len)
{
  random rand(16807);
  while(first < last){
    random_string(rand,len,*first++);
  }
}

template<typename T,int N> 
void generate_test_datas(slice<T,N>* first,slice<T,N>*last,int len)
{
  random rand(16807);
  while(first < last){
    random_string(rand,len,*first++);
  }
}
template <typename _RanIt>
void test_iter_swap(_RanIt _First, _RanIt _Last)
{ // [_Frist,_Last)
  while(_First < _Last)
    std::iter_swap(_First++,--_Last);
}


template <typename _RanIt>
int test_compare(_RanIt _First, _RanIt _Last)
{ // [_Frist,_Last)
  register int temp = 0;
  while(_First < _Last){
    if(*_First++ <  *--_Last)
      temp++;
  } 
  return temp;
}

template <typename DType>
void test_Bentleyswap(DType*x,int i, int u)
{ // [l,u]
  while (i < u)
    Bentleyswap(x, i++, u--);
}
const int maxIter = 100;

enum method_t
{
  itertor_swap  = 0,
  array_swap,
  value_compare,

  max_method_count,
};

const char* methodName[max_method_count] = 
{ 
  " iter_swap",
  "array_swap",
  "  compare "
};

double timesPerRun[max_method_count][maxIter];

template <typename VType>
static void bench(int iter,int size,int VLen = 0)
{
  VType * data = (VType *) malloc(sizeof(VType) * size);  
  std::uninitialized_fill(data,  data+size,   VType());
  generate_test_datas(data,data+size,VLen);


  const int allRuns = iter * max_method_count;
  int curentRun = 0;
  int xxx;
  for (int iter_i = 0 ; iter_i < iter; iter_i++)
    for(int i = 0; i < max_method_count;i++){

      cerr << "run ["  << ++curentRun << '/' << allRuns << "]";
      clock_t t1 = clock();
      switch (i)
      {
      case itertor_swap:
        test_iter_swap(data,data+size);
        break;
      case array_swap:
        test_Bentleyswap(data,0,size-1);
        break;
      case value_compare:
        xxx = test_compare(data,data+size);
        break;
      }
      clock_t t2 = clock();
      if(i == value_compare)
        cerr <<" = "<< xxx ;
      cerr <<'\n';
      timesPerRun[i][iter_i]= (double)(t2-t1)/CLOCKS_PER_SEC;
    }
  free(data);
}

#define  delim  ','
int main(int argc, char *argv[])
{
  int N = 50000000;
  int Iter = 1;
  if (argc < 3) 
    fprintf(stderr, "Usage: %s [%d] [%d]\n", argv[0], N,Iter);
  if (argc > 1)    N = atoi(argv[1]);
  if (argc > 2) Iter = atoi(argv[2]);
  Iter = min(Iter,maxIter);

  cout<< " type "<<delim 
    << "method"  <<delim
    << " time\n";
    
  bench<int32_t>(Iter,N);
  for (int j = 0; j < Iter;j++)
    for(int i = 0;i < max_method_count;i++){
      cout<<"int32_t"<<delim
        <<methodName[i]<<delim
        <<timesPerRun[i][j]
      <<'\n';
    }
    
  bench<int64_t>(Iter,N);
  for (int j = 0; j < Iter;j++)
    for(int i = 0;i < max_method_count;i++){
      cout<<"int64_t"<<delim
        <<methodName[i]<<delim
        <<timesPerRun[i][j]
      <<'\n';
    }

  bench<double>(Iter,N);
  for (int j = 0; j < Iter;j++)
    for(int i = 0;i < max_method_count;i++){
      cout<<"double"<<delim
        <<methodName[i]<<delim
        <<timesPerRun[i][j]
      <<'\n';
    }

  //bench<string>(Iter,N,12);
  //for (int j = 0; j < Iter;j++)
  //  for(int i = 0;i < max_method_count;i++){
  //    cout<<"string 12"   <<delim
  //      <<methodName[i]<<delim
  //      <<timesPerRun[i][j]
  //    <<'\n';
  //  }
  //bench<string>(Iter,N,16);
  //for (int j = 0; j < Iter;j++)
  //  for(int i = 0;i < max_method_count;i++){
  //    cout<<"string 16"   <<delim
  //      <<methodName[i]<<delim
  //      <<timesPerRun[i][j]
  //    <<'\n';
  //  }
  //bench<string>(Iter,N,32);
  //for (int j = 0; j < Iter;j++)
  //  for(int i = 0;i < max_method_count;i++){
  //    cout<<"string 32"   <<delim
  //      <<methodName[i]<<delim
  //      <<timesPerRun[i][j]
  //    <<'\n';
  //  }
  //bench<string>(Iter,N,64);
  //for (int j = 0; j < Iter;j++)
  //  for(int i = 0;i < max_method_count;i++){
  //    cout<<"string 64"   <<delim
  //      <<methodName[i]<<delim
  //      <<timesPerRun[i][j]
  //    <<'\n';
  //  }
  //bench<string>(Iter,N,128);
  //for (int j = 0; j < Iter;j++)
  //  for(int i = 0;i < max_method_count;i++){
  //    cout<<"string 128"   <<delim
  //      <<methodName[i]<<delim
  //      <<timesPerRun[i][j]
  //    <<'\n';
  //  }
  //bench<string>(Iter,N,256);
  //for (int j = 0; j < Iter;j++)
  //  for(int i = 0;i < max_method_count;i++){
  //    cout<<"string 256"   <<delim
  //      <<methodName[i]<<delim
  //      <<timesPerRun[i][j]
  //    <<'\n';
  //  }

  typedef slice<char, 12> sliceChar_12;
  typedef slice<char, 32> sliceChar_32;
  typedef slice<char, 64> sliceChar_64;
  typedef slice<char, 128> sliceChar_128;
  typedef slice<char, 256> sliceChar_256;

  bench<sliceChar_12>(Iter,N,12);
  for (int j = 0; j < Iter;j++)
    for(int i = 0;i < max_method_count;i++){
      cout<<"12"   <<delim
        <<methodName[i]<<delim
        <<timesPerRun[i][j]
      <<'\n';
    }

  bench<sliceChar_16>(Iter,N,16);
  for (int j = 0; j < Iter;j++)
    for(int i = 0;i < max_method_count;i++){
      cout<<"16"   <<delim
        <<methodName[i]<<delim
        <<timesPerRun[i][j]
      <<'\n';
    }

  bench<sliceChar_32>(Iter,N,32);
  for (int j = 0; j < Iter;j++)
    for(int i = 0;i < max_method_count;i++){
      cout<<"32"   <<delim
        <<methodName[i]<<delim
        <<timesPerRun[i][j]
      <<'\n';
    }

  bench<sliceChar_64>(Iter,N,64);
  for (int j = 0; j < Iter;j++)
    for(int i = 0;i < max_method_count;i++){
      cout<<"64"   <<delim
        <<methodName[i]<<delim
        <<timesPerRun[i][j]
      <<'\n';
    }
  bench<sliceChar_128>(Iter,N,128);
  for (int j = 0; j < Iter;j++)
    for(int i = 0;i < max_method_count;i++){
      cout<<"128"   <<delim
        <<methodName[i]<<delim
        <<timesPerRun[i][j]
      <<'\n';
    }

  bench<sliceChar_256>(Iter,N,256);
  for (int j = 0; j < Iter;j++)
    for(int i = 0;i < max_method_count;i++){
      cout<<"256"   <<delim
        <<methodName[i]<<delim
        <<timesPerRun[i][j]
      <<'\n';
    }

}