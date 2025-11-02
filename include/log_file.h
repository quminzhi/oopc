#ifndef LOG_FILE_H
#define LOG_FILE_H

#include "log.h"

/* specific to file log */

int filelog_create(logger *impl, char *filename);
void filelog_destory(logger *impl);

#endif  // LOG_FILE_H
