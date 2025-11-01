#ifndef LOG_FILE_H
#define LOG_FILE_H

#include "log.h"

/* specific to file log */

// encapsulate init function
int filelog_create(logger *impl, char *filename);

#endif  // LOG_FILE_H
