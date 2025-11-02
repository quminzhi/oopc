#include "log.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int file_init(logger *self, void *data);
void file_info(logger *self, const char *str);
void file_error(logger *self, const char *str);
void file_close(logger *self);

log_interface file_interface = {.init  = file_init,
                                .info  = file_info,
                                .error = file_error,
                                .close = file_close};

typedef struct filelog_data_ {
  const char *filename;
  FILE *fd;
} filelog_data;

int file_init(logger *self, void *data) {
  self->interface   = &file_interface;
  self->data        = data;

  filelog_data *fld = data;
  fld->fd           = fopen(fld->filename, "w");
  printf("File Logger Init: %s - %p\n", fld->filename, (void *)fld->fd);
  if (!fld->fd) {
    return errno;
  }
  return 0;
}

void file_info(logger *self, const char *str) {
  filelog_data *fld = self->data;
  fprintf(fld->fd, "INFO :> %s\n", str);
}

void file_error(logger *self, const char *str) {
  filelog_data *fld = self->data;
  fprintf(fld->fd, "ERROR:> %s\n", str);
}

void file_close(logger *self) {
  filelog_data *fld = self->data;
  fclose(fld->fd);
}

// in addition to init log, filelog_create allocate and
// filelog_destory free resources pointed by data
int filelog_create(logger *impl, const char *filename) {
  filelog_data *data = malloc(sizeof(filelog_data));
  if (!data) {
    return errno;
  }
  data->filename = filename;

  return file_init(impl, data);
}

void filelog_destroy(logger *impl) {
  // close fd first then free fd resource
  file_close(impl);
  filelog_data *data = impl->data;
  if (data) {
    free(data);
  }
}
