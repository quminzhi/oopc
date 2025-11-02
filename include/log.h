#ifndef LOG_H
#define LOG_H

/* log abstraction, interface */

#define ILOG(log, msg) (log)->interface->info(log, msg)
#define ELOG(log, msg) (log)->interface->error(log, msg)

typedef struct logger_ logger;

// separate struct for function interface
typedef struct log_interface_ {
  int (*init)(logger *self, void *data);
  void (*info)(logger *self, const char *str);
  void (*error)(logger *self, const char *str);
  void (*close)(logger *self);
} log_interface;

// logger struct with data object
struct logger_ {
  log_interface *interface;
  void *data;  // resources implementor may want to allocate or free
};

// only really one implementation of console so extern the impl
extern logger conlog;

#endif  // LOG_H
