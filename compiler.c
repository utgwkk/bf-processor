#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define TO_C

static int loop_depth = 0;

void make_label (char *ret) {
  static int label_number = 0;
  sprintf(ret, "L%d", label_number);
  label_number++;
}

#ifdef TO_MIPS
void compile (FILE* in) {
  char ch;
  char loop_start_label[32], loop_end_label[32];
  while ((ch = fgetc(in)) != EOF) {
    switch (ch) {
      case '+':
        printf(
          "\tlw\t$t1, 0($t0)\n"
          "\taddu\t$t1, $t1, 1\n"
          "\tsw\t$t1, 0($t0)\n"
        );
        break;
      case '-':
        printf(
          "\tlw\t$t1, 0($t0)\n"
          "\tadd\t$t1, $t1, -1\n"
          "\tsw\t$t1, 0($t0)\n"
        );
        break;
      case '>':
        printf("\taddiu\t$t0, $t0, 4\n");
        break;
      case '<':
        printf("\taddi\t$t0, $t0, -4\n");
        break;
      case '.':
        printf(
          "\tli\t$v0, 11\n"
          "\tlw\t$a0, 0($t0)\n"
          "\tsyscall\n"
        );
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
        break;
      case ']':
        if (loop_depth > 0) return;
        else break;
      default:
        break;
    }
  }
}

void compile_main (FILE *in) {
  printf(
    "\t.text\n"
    "\t.align\t2\n"
    "main:\n"
    "\tsw\t$ra, -8($sp)\n"
    "\tsw\t$fp, -4($sp)\n"
    "\taddi\t$fp, $sp, -4\n"
    "\tli\t$v0, 9\n"
    "\tli\t$a0, 12000\n"
    "\tsyscall\n"
    "\tmove\t$t0, $v0\n"
  );
  compile(in);
  printf(
    "ret:\n"
    "\taddi\t$sp, $fp, 4\n"
    "\tlw\t$ra, -4($fp)\n"
    "\tlw\t$fp, 0($fp)\n"
    "\tjr\t$ra\n"
  );
}
#endif

#ifdef TO_C
void compile (FILE* in) {
  char ch;
  while ((ch = fgetc(in)) != EOF) {
    switch (ch) {
      case '+':
        printf("\t++(*ptr);\n");
        break;
      case '-':
        printf("\t--(*ptr);\n");
        break;
      case '>':
        printf("\t++ptr;\n");
        break;
      case '<':
        printf("\t--ptr;\n");
        break;
      case '.':
        printf("\tputchar(*ptr);\n");
        break;
      case ',':
        printf("\t*ptr = getchar();\n");
        break;
      case '[':
        printf("\twhile (*ptr) {\n");
        break;
      case ']':
        printf("\t}\n");
        break;
      default:
        break;
    }
  }
}

void compile_main (FILE *in) {
  printf(
    "#include <stdio.h>\n"
    "#include <string.h>\n"
    "\n"
    "char v[30000];\n"
    "\n"
    "int main (void) {\n"
    "\tchar *ptr = v;\n"
    "\tmemset(v, 0, sizeof v);\n"
  );
  compile(in);
  printf(
    "\treturn 0;\n"
    "}\n"
  );
}
#endif

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
