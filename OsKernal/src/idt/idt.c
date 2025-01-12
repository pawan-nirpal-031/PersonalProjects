#include "idt.h"
#include "config.h"
#include "io/io.h"
#include "kernel.h"
#include "memory/memory.h"

struct idtDesc idtDescriptors[TOTOAL_INTERRUPTS];
struct idtrDesc idtrDescriptor;

extern void noInterrupts();
extern void int21h();
extern void idtLoad(struct idtrDesc *ptr);

void idtZero() { print("Divide by zero error"); }

void int21hHandler() {
  print("Interrupt for keyboard press called\n");
  outb(0x20, 0x20);
}

void noInterruptsHandler() { outb(0x20, 0x20); }

void idtSet(int intrNum, void *address) {
  struct idtDesc *decs = &idtDescriptors[intrNum];
  decs->offset1 = (uint32_t)address & 0x0000FFFF;
  decs->selector = KERNEL_CODE_SELECTOR;
  decs->zero = 0;
  decs->type_attr = 0xEE;
  decs->offset2 = (uint32_t)address >> 16;
}

void idtInit() {
  memset(idtDescriptors, 0, sizeof(idtDescriptors));
  idtrDescriptor.limit = sizeof(idtDescriptors) - 1;
  idtrDescriptor.base = (uint32_t)idtDescriptors;

  for (int i = 0; i < TOTOAL_INTERRUPTS; i++) {
    idtSet(i, noInterrupts);
  }

  idtSet(0, idtZero);
  idtSet(0x21, int21h);

  // Load the idt
  idtLoad(&idtrDescriptor);
}