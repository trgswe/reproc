#include "handle.h"

#include "error.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

const int HANDLE_INVALID = -1;

int handle_from(FILE *file, int *handle)
{
  assert(handle);

  int r = fileno(file);
  if (r < 0) {
    return error_unify(r);
  }

  r = fcntl(r, F_DUPFD_CLOEXEC, 0);
  if (r < 0) {
    return error_unify(r);
  }

  *handle = r;

  return 0;
}

int handle_destroy(int handle)
{
  if (handle == HANDLE_INVALID) {
    return HANDLE_INVALID;
  }

  int r = 0;

  PROTECT_SYSTEM_ERROR;

  r = close(handle);
  assert_unused(r == 0);

  UNPROTECT_SYSTEM_ERROR;

  return HANDLE_INVALID;
}
