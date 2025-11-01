#include "mp.h"
#include "log.h"
#include "log_file.h"
#include <stdio.h>
#include <stdlib.h>

void dogSound(void *self) {
  printf("%s wangwang\n", ((Animal *)self)->name);
}
void dogMove(void *self) {}
void dogEat(void *self) {}
static const VTable dogVTable = {
    .sound = dogSound, .move = dogMove, .eat = dogEat};

void catSound(void *self) {
  printf("%s mewmew\n", ((Animal *)self)->name);
}
void catMove(void *self) {}
void catEat(void *self) {}
static const VTable catVTable = {
    .sound = catSound, .move = catMove, .eat = catEat};

int mp_main(int argc, char **argv) {
  Animal dog = createAnimal("doggy", &dogVTable);
  Animal cat = createAnimal("catty", &catVTable);
  sound(&dog);
  sound(&cat);
  return 0;
}

void do_something(logger *log) {
  ILOG(log, "Part 1");
  ILOG(log, "Part 2");
  if (1) {
    ELOG(log, "oops");
  }
}

int log_main(int argc, char **argv) {
  //
  // console log sample
  //

  // long winded usage
  conlog.interface->info(&conlog, "Info message 1");
  conlog.interface->error(&conlog, "Error message 1");

  log_interface *clog = conlog.interface;
  // not really necessary for console log, but...
  clog->init(&conlog, 0);
  // shorter usage
  clog->info((void *)clog, "Info message 2");
  clog->error((void *)clog, "Error message 2");

  // macro usage
  ILOG(&conlog, "Info message 3");
  ELOG(&conlog, "Error message 3");

  //
  // file log samples
  //

  // first setup 2 separate log files.
  logger f1;
  logger f2;

  // create the loggers...
  filelog_create(&f1, "file1.log");
  filelog_create(&f2, "file2.log");

  // shorter usage
  log_interface *flog1 = f1.interface;
  log_interface *flog2 = f2.interface;

  flog1->info(&f1, "Info message 1");
  flog2->info(&f2, "Info message 1");
  flog1->error(&f1, "Error message 1");
  flog2->error(&f2, "Error message 1");

  // macro usage
  ILOG(&f1, "Info message 2");
  ILOG(&f2, "Info message 2");

  ELOG(&f1, "Error message 2");
  ELOG(&f2, "Error message 2");

  do_something(&f1);

  flog1->close(&f1);
  flog2->close(&f2);

  return 0;
}

int main(int argc, char **argv) {
  return log_main(argc, argv);
  return mp_main(argc, argv);
}
