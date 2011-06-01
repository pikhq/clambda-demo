#include <stdio.h>

#include "lambda.h"
#include "pair.h"
#include "bool.h"
#include "caller.h"

#include "list.h"

// nil = pair churchTrue churchTrue
static void __attribute__((constructor)) init_nil()
{
  nil = caller(pair, 2, churchTrue, churchTrue);
}

static void __attribute__((constructor)) init_isnil()
{
  isnil = fst;
}

// lambda h.lambda t.pair churchFalse (pair h t)
global_closure(, cons, NULL, (void*_, closure h), {
    return lambda(h, (closure h, closure t), {
	return caller(pair, 2, churchFalse, caller(pair, 2, h, t));
      });
  });

// lambda z.fst (snd z)
global_closure(, car, NULL, (void*_, closure z), {
    return call(fst, call(snd, z));
  });

// lambda z.snd (snd z)
global_closure(, cdr, NULL, (void*_, closure z), {
    return call(snd, call(snd, z));
  });

global_closure(, mapP, NULL, (void*_, closure f), {
    return lambda(f, (closure f, closure l), {
	if(*(int*)call(fromChurchBool,call(isnil,l)))
	  return NULL;
	call(f, call(car,l));
	return caller(mapP, 2, f, call(cdr, l));
      });
  });

global_closure(, map, NULL, (void*_, closure f), {
    return lambda(f, (closure f, closure l), {
	if(*(int*)call(fromChurchBool,call(isnil,l)))
	  return nil;
	void *f_ret = call(f, call(car, l));
	closure map_ret = caller(map, 2, f, call(cdr, l));
	return caller(cons, 2, f_ret, map_ret);
      });
  });

