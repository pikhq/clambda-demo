#ifndef LAMBDA_H
#define LAMBDA_H

#include "xgc.h"
#include "closure.h"

#define global_closure(linkage, name, close, params, ...)		\
  static void *__LAMBDA__##name params					\
  {									\
    __VA_ARGS__;							\
  }									\
  static struct closure __LAMBDA__##name##_ = { __LAMBDA__##name, close }; \
  linkage closure name = &__LAMBDA__##name##_;

#define lambda(close, params, ...)					\
  ({									\
    void* __LAMBDA__ params						\
    {									\
      __VA_ARGS__;							\
    };									\
    closure _x = call(xgc_malloc,sizeof(struct closure));		\
    *_x = (struct closure){ __LAMBDA__, close };			\
    _x;									\
  })

#define call(x, ...) ({				\
      closure _x = x;				\
      _x->func(_x->close, __VA_ARGS__);		\
    })

#endif
