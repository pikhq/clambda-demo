#include "gc.h"

#include "lambda.h"
#include "onerr.h"

#include "xgc.h"


global_closure(, xgc_malloc, NULL, (void*_, size_t size), {
    void *ret = GC_MALLOC(size);
    if(!ret)
      call(onerr, "Could not allocate memory.\n");
    return ret;
  });

global_closure(, xgc_realloc, NULL, (void*_, void *x), {
    return lambda(x, (void *x, size_t size), {
	void *ret = GC_REALLOC(x, size);
	if(!ret)
	  call(onerr, "Could not resize memory.\n");
	return ret;
      });
  });
