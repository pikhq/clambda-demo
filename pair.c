#include <stdio.h>

#include "xgc.h"
#include "lambda.h"
#include "caller.h"

#include "pair.h"

//lambda x.lambda y.lambda z.z x y
global_closure(, pair, NULL, (void*_, closure x), {
    return lambda(x, (closure x, closure y), {
	void **c = call(xgc_malloc,sizeof(void*)*2);
	c[0] = x;
	c[1] = y;
	return lambda(c, (void **c, closure z), {
	    closure x = c[0];
	    closure y = c[1];
	    return caller(z, 2, x, y);
	  });
      });
  });

// lambda p.p (lambda x.lambda y.x)
global_closure(, fst, NULL, (void*_, closure p), {
    closure arg = lambda(NULL, (void*_, closure x), {
	return lambda(x, (closure x,void*_), return x;);
      });
    return call(p,arg);
  });

// lambda p.p (lambda x.lambda y.y)
global_closure(, snd, NULL, (void*_, closure p), {
    closure arg = lambda(NULL, (void*_, void*__), {
	return lambda(NULL, (void*_, closure y), return y;);
      });
    return call(p,arg);
  });
