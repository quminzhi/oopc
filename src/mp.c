#include "mp.h"
#include <string.h>

#define VCALL_SAFE(obj, method)                          \
  do {                                                   \
    if ((obj) && (obj)->vtable && (obj)->vtable->method) \
      (obj)->vtable->method((obj));                      \
  } while (0)

void sound(Animal *animal) {
  VCALL_SAFE(animal, sound);
}
void move(Animal *animal) {
  VCALL_SAFE(animal, move);
}
void eat(Animal *animal) {
  VCALL_SAFE(animal, eat);
}

Animal createAnimal(char name[], const VTable *vt) {
  Animal animal;
  strcpy(animal.name, name);
  animal.vtable = vt;
  return animal;
}
