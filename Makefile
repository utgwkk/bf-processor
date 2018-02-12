OUT=bfi
IN=interpreter.c

$(OUT): $(IN)
	gcc -o $(OUT) $(IN)

.PHONY: clean test

clean:
	rm -f $(OUT)

test: $(OUT)
	./test.sh
