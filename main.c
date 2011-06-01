#include <stdlib.h>
#include <stdio.h>

#include "lambda.h"

#include "onerr.h"
#include "church_numerals.h"
#include "bool.h"
#include "list.h"
#include "caller.h"

static closure print_list;
static closure flip;
static closure range;
static closure iter;
static closure gen_list;
static closure fib;
static closure fact;

global_closure(static, print_list, NULL, (void*_, closure l), {
    caller(mapP, 2, lambda(NULL, (void*_,closure x), printf("%u ", *(unsigned int*)call(fromChurch,x));), l);
    printf("\n");
  });

global_closure(static, flip, NULL, (void*_, closure f), {
    return lambda(f, (closure f, closure x), {
	void **c = call(xgc_malloc, sizeof(void*)*2);
	c[0] = f;
	c[1] = x;
	return lambda(c, (void **c, closure y), {
	    closure f = c[0];
	    closure x = c[1];
	    return caller(f, 2, y, x);
	  });
      });
  });

global_closure(static, range, NULL, (void*_, closure from), {
    return lambda(from, (closure from, closure to), {
	void **c = call(xgc_malloc,sizeof(void*)*2);
	c[0] = from;
	c[1] = to;
	return lambda(c, (void **c, closure f), {
	    closure from = c[0];
	    closure to = c[1];
	    call(f, from);
	    closure n = caller(churchSub, 2, to, from);
	    if(*(int*)call(fromChurchBool,call(is0,n)))
	      return NULL;
	    else
	      return caller(range, 3, call(churchSucc, from), to, f);
	  });
      });
  });


global_closure(static, iter, NULL, (void*_, closure from), {
    return lambda(from, (closure from, closure to), {
	void **c = call(xgc_malloc,sizeof(void*)*2);
	c[0] = from;
	c[1] = to;
	return lambda(c, (void **c, closure f), {
	    closure from = c[0];
	    closure to = c[1];
	    closure n = caller(churchSub, 2, to, from);
	    closure val = call(f, from);
	    if(*(int*)call(fromChurchBool,call(is0,n)))
	      return caller(cons, 2, val, nil);
	    else {
	      closure next = call(churchSucc,from);
	      
	      return caller(cons, 2, val, caller(iter, 3, next, to, f));
	    }
	  });
      });
  });

global_closure(static, gen_list, NULL, (void*_, closure from), {
    return lambda(from, (closure from, closure to), {
	closure l = nil;
	caller(range, 3, from, to, lambda(&l, (closure *l, closure x), {
	      *l = caller(cons, 2, x, *l);
	    }));
	return l;
      });
  });

global_closure(static, fib, NULL, (void*_, closure x), {
    if(*(int*)call(fromChurchBool,call(is0,x)))
      return church0;
    else if(*(int*)call(fromChurchBool,call(is0,call(churchPred,x))))
      return call(churchSucc,church0);
    else {
      closure pred = call(churchPred,x);
      closure a = call(fib,pred);
      closure b = call(fib,call(churchPred,pred));
      return caller(churchPlus, 2, a, b);
    }
  });

global_closure(static, fact, NULL, (void*_, closure x), {
    if(*(int*)call(fromChurchBool,call(is0,x)))
      return call(churchSucc,church0);
    else
      return caller(churchMult, 2, x, call(fact,call(churchPred,x)));
  });

int main(int argc, char **argv)
{
  onerr = lambda(argv[0], (char *c, char *s), {
      fprintf(stderr, "%s: %s", c, s);
      exit(1);
    });

  closure l = caller(gen_list, 2, church0, call(toChurch, argc > 1 ? atoi(argv[1]) : 10));
  closure fromTo = caller(flip, 2, map, l);
  call(print_list, l);
  call(print_list, call(fromTo, fib));
  if(argc > 2) {
    call(print_list, call(fromTo, fact));
  }
  l = NULL;
  fromTo = NULL;

  closure church10 = call(toChurch, 10);
  closure church100 = caller(churchMult, 2, church10, church10);
  closure church2 = call(toChurch, 2);
  closure church10000 = caller(churchExp, 2, church100, church2);
  church100 = NULL;
  church2 = NULL;
  closure church9990 = caller(churchSub, 2, church10000, church10);
  call(printChurch, church9990);
}
