#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static int label_number = 0;
static int loop_depth = 0;

void make_label (char *ret) {
  sprintf(ret, "L%d", label_number);
  label_number++;
}

void compile (FILE* in) {
  char ch;
  char loop_start_label[32], loop_end_label[32];
  while ((ch = fgetc(in)) != EOF) {
    switch (ch) {
      case '+':
        printf("\tlw\t$t1, 0($t0)\n");
        printf("\taddu\t$t1, $t1, 1\n");
        printf("\tsw\t$t1, 0($t0)\n");
        break;
      case '-':
        printf("\tlw\t$t1, 0($t0)\n");
        printf("\tadd\t$t1, $t1, -1\n");
        printf("\tsw\t$t1, 0($t0)\n");
        break;
      case '>':
        printf("\taddiu\t$t0, $t0, 4\n");
        break;
      case '<':
        printf("\taddi\t$t0, $t0, -4\n");
        break;
      case '.':
        printf("\tli\t$v0, 11\n");
        printf("\tlw\t$a0, 0($t0)\n");
        printf("\tsyscall\n");
        break;
      case ',':
        printf("\tli\t$v0, 12\n");
        printf("\tsyscall\n");
        printf("\tsw\t$v0, 0($t0)\n");
        break;
      case '[':
        make_label(loop_start_label);
        make_label(loop_end_label);
        printf("%s:\n", loop_start_label);
        printf("\tlw\t$t1, 0($t0)\n");
        printf("\tbeq\t$t1, $zero, %s\n", loop_end_label);
        loop_depth++;
        compile(in);
        loop_depth--;
        printf("\tj\t%s\n", loop_start_label);
        printf("%s:\n", loop_end_label);
      case ']':
        if (loop_depth > 0) return;
        else break;
      default:
        break;
    }
  }
  printf(
    "ret:\n"
		"\taddi\t$sp, $fp, 4\n"
		"\tlw\t$ra, -4($fp)\n"
		"\tlw\t$fp, 0($fp)\n"
		"\tjr\t$ra\n"
	);
}

void compile_main (FILE *in) {
  printf("\t.text\n");
  printf("\t.align\t2\n");
  printf("main:\n");
  printf("\tsw\t$ra, -8($sp)\n");
  printf("\tsw\t$fp, -4($sp)\n");
  printf("\taddi\t$fp, $sp, -4\n");
  printf("\taddi\t$sp, $sp, -40000\n");
  printf("\taddi\t$t0, $fp, -8\n");
  compile(in);
}

int main (int argc, char **argv) {
  FILE *in;

  if (argc < 2) {
    compile_main(stdin);
  } else {
    if ((in = fopen(argv[1], "r")) == NULL) {
      fprintf(stderr, "Failed to open '%s'\n", argv[1]);
      exit(EXIT_FAILURE);
    }
    compile_main(in);
    fclose(in);
  }
  return 0;
}
