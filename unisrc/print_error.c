#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

#include "print_error.h"

void print_error(int line_num, const char* func_name, enum error_type type, int err_code)
{
  fprintf (stderr, "Line %d, function %s: ", line_num, func_name);
  switch (type) {
    
  case GEN_ERR:
    fprintf(stderr, "%s\n", strerror(err_code));
    break;
  case MEM_ALLOC_ERR:
    fprintf(stderr, "Failed to allocate more memory for plugins\n");
    break;
  case DL_ERR:
    fprintf(stderr, "%s\n", dlerror());
  }
}
