#include <stdio.h>
#include <stdlib.h>

#include "lambda.h"

#include "onerr.h"

// lambda s.fprintf stderr "%s" s; exit 1
global_closure(, onerr, NULL, (void*_, char *s), {
    fprintf(stderr, "%s", s);
    exit(1);
  });
