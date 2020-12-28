#ifndef __BUFFER_H___
#define __BUFFER_H___

#include <Particle.h>

template <typename T>
struct Entry
{
  const T value;
  const unsigned long time;

  Entry(T value, unsigned long time) : value(value), time(time) {}
};

template <typename T, unsigned int S>
class Buffer
{
public:
  const unsigned int size = S;

  void push(T value, unsigned long time = millis());
  const Entry<T> &entry(unsigned int offset);
  T value(unsigned int offset);
  unsigned long time(unsigned int offset);

private:
  Entry<T> data[S];
  unsigned int head = 0, tail = 0;
};

#endif
