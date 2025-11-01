#include "mp.h"
#include <stdio.h>
#include <stdlib.h>

void dogSound(void *self) { printf("%s wangwang\n", ((Animal *)self)->name); }
void dogMove(void *self) {}
void dogEat(void *self) {}
static const VTable dogVTable = {.sound = dogSound, .move = dogMove, .eat = dogEat};

void catSound(void *self) { printf("%s mewmew\n", ((Animal *)self)->name); }
void catMove(void *self) {}
void catEat(void *self) {}
static const VTable catVTable = {.sound = catSound, .move = catMove, .eat = catEat};

int main(int argc, char **argv) {
  Animal dog = createAnimal("doggy", &dogVTable);
  Animal cat = createAnimal("catty", &catVTable);
  sound(&dog);
  sound(&cat);

  return 0;
}
