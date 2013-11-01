#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <cstdint>

using namespace std;

template <typename DType>
inline void Bentleyswap(DType*x,int i, int j)
{  
  DType t = x[i];
  x[i] = x[j];
  x[j] = t;
}

template <typename _RanIt>
void generate_test_datas(_RanIt _First, _RanIt _Last)
{
  typedef iterator_traits<_RanIt>::value_type value_t;
  value_t i = 0;
  while (_First != _Last)
    *_First++ = i++;
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
static void bench(int iter,int size)
{
  VType * data = (VType *) malloc(sizeof(VType) * size);
  generate_test_datas(data,data+size);


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
int main(int argc, char *argv[])
{
    int N = 50000000;
    int Iter = 1;
    if (argc < 3) 
      fprintf(stderr, "Usage: %s [%d] [%d]\n", argv[0], N,Iter);
    if (argc > 1)    N = atoi(argv[1]);
    if (argc > 2) Iter = atoi(argv[2]);
    Iter = min(Iter,maxIter);

    cout<< " type \t" 
      << "method\t" 
      << " time\n";
    
    bench<int32_t>(Iter,N);
    for (int j = 0; j < Iter;j++)
      for(int i = 0;i < max_method_count;i++){
        cout<<"int32_t"<<"\t"
          <<methodName[i]<<"\t"
          <<timesPerRun[i][j]
        <<'\n';
      }
    
    bench<int64_t>(Iter,N);
    for (int j = 0; j < Iter;j++)
      for(int i = 0;i < max_method_count;i++){
        cout<<"int64_t"<<"\t"
          <<methodName[i]<<"\t"
          <<timesPerRun[i][j]
        <<'\n';
      }

    bench<double>(Iter,N);
    for (int j = 0; j < Iter;j++)
      for(int i = 0;i < max_method_count;i++){
        cout<<"double"<<"\t"
          <<methodName[i]<<"\t"
          <<timesPerRun[i][j]
        <<'\n';
      }
}