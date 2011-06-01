#include <stdio.h>

#include "lambda.h"
#include "xgc.h"
#include "bool.h"
#include "caller.h"

#include "church_numerals.h"


// lambda f.lambda x.x
global_closure(, church0, NULL, (void*_, closure f), {
    return lambda(NULL, (void*_, void *x), return x;);
  });

// lambda n.lambda f.lambda x.f (n f x)
global_closure(, churchSucc, NULL, (void*_, closure n), {
    return lambda(n, (closure n, closure f), {
	void **c = call(xgc_malloc,sizeof(void*)*2);
	c[0] = n;
	c[1] = f;
	return lambda(c, (void **c, void *x), {
	    closure n = c[0];
	    closure f = c[1];
	    return call(f,caller(n, 2, f, x));
	  });
      });
  });

// lambda n.lambda f.lambda x.n (lambda g.lambda h.h (g f)) (lambda u.x) (lambda u.u)
global_closure(, churchPred, NULL, (void*_, closure n), {
    return lambda(n, (closure n, closure f), {
	void **c = call(xgc_malloc,sizeof(void*)*2);
	c[0] = n;
	c[1] = f;
	return lambda(c, (void **c, closure x), {
	    closure n = c[0];
	    closure f = c[1];
	    closure arg1 = lambda(f, (closure f, closure g), {
		void **c = call(xgc_malloc,sizeof(void*)*2);
		c[0] = f;
		c[1] = g;
		return lambda(c, (void **c, closure h), {
		    closure f = c[0];
		    closure g = c[1];
		    return call(h,call(g,f));
		  });
	      });
	    closure arg2 = lambda(x, (closure x, void*_), return x;);
	    closure arg3 = lambda(NULL, (void*_, closure u), return u;);

	    return caller(n, 3, arg1, arg2, arg3);
	  });
      });
  });

// lambda m.lambda n.m churchSucc n
global_closure(, churchPlus, NULL, (void*_, closure m), {
    return lambda(m, (closure m, closure n), return caller(m, 2, churchSucc, n););
  });

//lambda m.lambda n.n churchPred m
global_closure(, churchSub, NULL, (void*_, closure m), {
    return lambda(m, (closure m, closure n), return caller(n, 2, churchPred, m););
  });

// lambda m.lambda n.m (churchPlus n) church0
global_closure(, churchMult, NULL, (void*_, closure m), {
    return lambda(m, (closure m, closure n), return caller(m, 2, call(churchPlus, n), church0););
  });

// lambda m.lambda n.n m
global_closure(, churchExp, NULL, (void*_, closure m), {
    return lambda(m, (closure m, closure n), return call(n,m););
  });

global_closure(, toChurch, NULL, (void*_, unsigned int m), {
    closure f(unsigned int m, closure n)
    {
      return m == 0 ? n : f(m-1, call(churchSucc, n));
    }
    return f(m, church0);
  });

global_closure(, fromChurch, NULL, (void*_, closure n), {
    unsigned int *x = call(xgc_malloc,sizeof(unsigned int));
    return caller(n, 2, lambda(NULL, (void*_, unsigned int *x), ++*x;return x;), x);
  });

global_closure(, printChurch, NULL, (void*_, closure n), {
    printf("%u\n", *(unsigned int*)call(fromChurch,n));
  });

global_closure(, is0, NULL, (void*_, closure n), {
    return caller(n, 2, lambda(NULL, (void*_,void*__), return churchFalse;), churchTrue);
  });
