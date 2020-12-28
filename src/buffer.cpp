#define SYSLOG_DEBUG
#include <psyslog.h>

#include "buffer.h"

template <typename T, unsigned int S>
void Buffer<T, S>::push(T value, unsigned long time)
{
  head = (head + 1) % size;
  data[head].value = value;
  data[head].time = time;
  if (head == tail)
  {
    tail = (tail + 1) % size;
  }
}

template <typename T, unsigned int S>
const Entry<T> &Buffer<T, S>::entry(unsigned int offset)
{
  return data[(head - offset) % size];
}

template <typename T, unsigned int S>
T Buffer<T, S>::value(unsigned int offset)
{
  return entry(offset).value;
}

template <typename T, unsigned int S>
unsigned long Buffer<T, S>::time(unsigned int offset)
{
  return entry(offset).time;
}
