#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <functional>
#include <iostream>

using namespace std;

typedef int DType;

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

void test_Bentleyswap(DType*x,int i, int u)
{ // [l,u]
  while (i < u)
    Bentleyswap(x, i++, u--);
}
const int maxIter = 100;

static void bench(int iter,int size)
{
  int * data = (int *) malloc(sizeof(int) * size);
  generate_test_datas(data,data+size);

  const char* methodName[2] = 
  { " iter_swap ",
    "Bentleyswap"};
  
  double timesPerRun[2][maxIter];
  const int allRuns = iter * 2;
  int curentRun = 0;

  for (int iter_i = 0 ; iter_i < iter; iter_i++)
    for(int i = 0; i < 2;i++){
      cerr << "run ["  << ++curentRun << '/' << allRuns << "]\n";
      clock_t t1 = clock();
      if(i)
        test_Bentleyswap(data,0,size-1);
      else
        test_iter_swap(data,data+size);
      clock_t t2 = clock();
      timesPerRun[i][iter_i]= (double)(t2-t1)/CLOCKS_PER_SEC;
    }

  cout<< " size \t" 
      << "method\t" 
      << " time\n";
  for (int j = 0; j < iter;j++)
    for(int i = 0;i < 2;i++){
      cout<<size<<"\t"
          <<methodName[i]<<"\t"
          <<timesPerRun[i][j]
          <<'\n';
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
    bench(Iter,N);
}