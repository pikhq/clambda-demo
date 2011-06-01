#include <string.h>
#include <stdint.h>

#include "lambda.h"
#include "xgc.h"
#include "caller.h"

#include "bool.h"

//lambda a.lambda b.a
global_closure(, churchTrue, NULL, (void*_, closure a), {
    return lambda(a, (closure a,void*_), return a;);
  });

//lambda a.lambda b.b
global_closure(, churchFalse, NULL, (void*_,void*__), {
    return lambda(NULL, (void*_, closure b), return b;);
  });

//lambda m.lambda n.m n m
global_closure(, and, NULL, (void*_, closure m), {
    return lambda(m, (closure m, closure n), return caller(m, 2, n, m););
  });

//lambda m.lambda n.m m n
global_closure(, or, NULL, (void*_, closure m), {
    return lambda(m, (closure m, closure n), return caller(m, 2, m, n););
  });

//lambda m.lambda a.lambda b.m b a
global_closure(, not, NULL, (void *_, closure m), {
    return lambda(m, (closure m, closure a), {
	void **c = call(xgc_malloc,sizeof(void*)*2);
	c[0] = m;
	c[1] = a;
	return lambda(c, (void **c, closure b), {
	    closure m = c[0];
	    closure a = c[1];
	    return caller(m, 2, b, a);
	  });
      });
  });

//lambda m.lambda n.lambda a.lambda b. m (n b a) (n a b)
global_closure(, xor, NULL, (void *_, closure m), {
    return lambda(m, (closure m, closure n), {
	void **c = call(xgc_malloc,sizeof(void*)*2);
	c[0] = m;
	c[1] = n;
	return lambda(c, (void **c, closure a), {
	    void **c1 = call(xgc_malloc,sizeof(void*)*3);
	    memcpy(c1, c, sizeof(void*)*2);
	    c1[2] = a;
	    return lambda(c1, (void **c, closure b), {
		closure m = c[0];
		closure n = c[1];
		closure a = c[2];
		return caller(m, 2, caller(n, 2, b, a), caller(n, 2, a, b));
	      });
	  });
      });
  });

global_closure(, fromChurchBool, NULL, (void*_, closure f), {
    int *a = call(xgc_malloc,sizeof(int));
    int *b = call(xgc_malloc,sizeof(int));
    *a = 0;
    *b = 1;
    return caller(f, 2, b, a);
  });

global_closure(, toChurchBool, NULL, (void*_, int p), return p ? churchTrue : churchFalse;);
