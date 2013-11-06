#ifndef _RANDOM_H__
#define _RANDOM_H__

#include <cstdint>

struct random
{
  uint32_t seed_;

  uint32_t next(){
    seed_ = seed_ * 1103515245L + 12345;
    return seed_;
  }

  int32_t nextInt(){
    return next();
  }

  random (uint32_t s)
  {
    seed_ = s;
  }

  // Returns a uniformly distributed value in the range [0..n-1]
  // REQUIRES: n > 0
  uint32_t uniform(int n) { return next() % n; }
};

template<typename STRING_T>
inline void random_string(random& rand, int len, STRING_T& dst)
{
  dst.resize(len);
  for (int i = 0; i < len; i++)
    dst[i] = static_cast<char>(' ' + rand.uniform(95));   // ' ' .. '~'  
}

#endif // _random_H__
