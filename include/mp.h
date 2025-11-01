#ifndef MP_H
#define MP_H

// Check NULL everywhere
typedef struct {
  void (*sound)(void *self);
  void (*move)(void *self);
  void (*eat)(void *self);
} VTable; /* interface in go */

typedef struct {
  char name[20];
  const VTable *vtable;
} Animal; /* virtual class, user implement virtual functions defined in VTable */

Animal createAnimal(char name[], const VTable *vt);
void sound(Animal *animal);
void move(Animal *animal);
void eat(Animal *animal);

#endif // MP_H