#ifndef CLOSURE_H
#define CLOSURE_H

struct closure {
  void *(*func)();
  void *close;
};

typedef struct closure *closure;

#endif
