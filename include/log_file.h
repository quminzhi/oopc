#ifndef LOG_FILE_H
#define LOG_FILE_H

#include "log.h"

/* specific to file log */

int filelog_create(logger *impl, const char *filename);
void filelog_destroy(logger *impl);

#endif  // LOG_FILE_H
