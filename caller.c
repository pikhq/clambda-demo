#include <stdarg.h>

#include "lambda.h"

void *caller(closure f, unsigned int n, ...)
{
  va_list ap;
  va_start(ap, n);
  void *ret = f;
  while(n --> 0) {
    void *x = va_arg(ap, void*);
    ret = call(ret, x);
  }
  va_end(ap);
  return ret;
}
