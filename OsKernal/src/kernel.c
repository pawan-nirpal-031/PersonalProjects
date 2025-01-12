#include "kernel.h"
#include "idt/idt.h"
#include <stddef.h> // size_t
#include <stdint.h>

uint16_t *videoMem = 0;
uint16_t TermRow = 0;
uint16_t TermCol = 0;

uint16_t terminalMakeChar(char c, char col) { return col << 8 | c; }

void terminalPutChar(char c, char col, int x, int y) {
  videoMem[(y * VGA_WIDTH) + x] = terminalMakeChar(c, col);
}

void terminalWriteChar(char c, char col) {
  if (c == '\n') {
    TermCol = 0;
    TermRow += 1;
    return;
  }
  terminalPutChar(c, col, TermCol, TermRow);
  TermCol += 1;
  if (TermCol >= VGA_WIDTH) {
    TermCol = 0;
    TermRow += 1;
  }
}

// Initialize the terminal with blank spaces
void terminalInitalize() {
  videoMem = (uint16_t *)0xB8000;
  TermCol = 0;
  TermRow = 0;
  for (int y = 0; y < VGA_HEIGHT; y++)
    for (int x = 0; x < VGA_WIDTH; x++)
      terminalPutChar(' ', 0, x, y);
}

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len])
    len++;
  return len;
}

void print(const char *str) {
  size_t len = strlen(str);
  for (size_t i = 0; i < len; i++)
    terminalWriteChar(str[i], 15);
}

void kernelMain() {
  terminalInitalize();
  print("Hello, World!\nTest");

  idtInit(); // Initialize the IDT
}