#include <stdio.h>
#include <string.h>

static char *p, *ptr;
static char vars[30000];

void beyond_loop_end () {
  while (*p != ']') {
    p++;
    if (*p == '[') {
      beyond_loop_end();
    }
  }
}

void eval () {
  char *oldptr;
  while (*p != '\0') {
    switch (*p) {
      case '+':
        (*ptr)++;
        break;
      case '-':
        (*ptr)--;
        break;
      case '>':
        ptr++;
        break;
      case '<':
        ptr--;
        break;
      case '.':
        putchar(*ptr);
        break;
      case ',':
        *ptr = getc(stdin);
        break;
      case '[':
        if (!(*ptr)) {
          beyond_loop_end();
          break;
        }
        oldptr = p++;
        eval();
        // if nonzero
        if (*ptr)
          p = oldptr - 1;
        break;
      case ']':
        // end of loop
        return;
      default:
        break;
    }
    p++;
  }
}

int main (int argc, char **argv) {
  // initialize variables
  memset(vars, 0, sizeof vars);
  if (argc < 2) {
    printf("Usage: ./bf [program]\n");
    return 1;
  }
  p = argv[1];
  ptr = vars;
  eval();
  return 0;
}
