INTERPRETER=bfi
COMPILER=bfc
OUT=$(INTERPRETER) $(COMPILER)
INTERPRETER_IN=interpreter.c
COMPILER_IN=compiler.c

.PHONY: clean test compiler interpreter all

all: compiler interpreter

$(INTERPRETER): $(INTERPRETER_IN)
	gcc -o $(INTERPRETER) $(INTERPRETER_IN)

$(COMPILER): $(COMPILER_IN)
	gcc -o $(COMPILER) $(COMPILER_IN)

compiler: $(COMPILER)
interpreter: $(INTERPRETER)

clean:
	rm -f $(OUT)

test: $(OUT)
	./test.sh
