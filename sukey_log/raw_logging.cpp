#include "utilities.h"

#include <stdarg.h>
#include <stdio.h>
#include <errno.h>

#include <fcntl.h>              
#include <time.h>
#include "config.h"
#include "logging.h"
#include "raw_logging.h"
#include "commandlineflags.h"

_START_SUKEY_NAMESPACE_

static struct ::tm last_tm_time_for_raw_log;
static int last_usecs_for_raw_log;

void RawLog__SetLastTime(const struct ::tm& t, int usecs) {
  memcpy(&last_tm_time_for_raw_log, &t, sizeof(last_tm_time_for_raw_log));
  last_usecs_for_raw_log = usecs;
}

_END_SUKEY_NAMESPACE_