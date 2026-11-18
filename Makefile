CC ?= cc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Isrc -Itests
BUILD = build
TESTES = $(BUILD)/test_estados $(BUILD)/test_transicoes $(BUILD)/test_caminhos

.PHONY: test cobertura clean

test: $(TESTES)
	@for t in $(TESTES); do ./$$t || exit 1; done

$(BUILD)/%: tests/%.c src/maquina.c src/maquina.h tests/teste.h | $(BUILD)
	$(CC) $(CFLAGS) $< src/maquina.c -o $@

# Cobertura de linhas e ramos de src/maquina.c (requer gcc e gcov)
cobertura: clean
	mkdir -p $(BUILD)/cov
	$(CC) $(CFLAGS) --coverage -c src/maquina.c -o $(BUILD)/cov/maquina.o
	@for t in estados transicoes caminhos; do \
		$(CC) $(CFLAGS) --coverage tests/test_$$t.c $(BUILD)/cov/maquina.o -o $(BUILD)/cov/test_$$t || exit 1; \
		./$(BUILD)/cov/test_$$t > /dev/null || exit 1; \
	done
	gcov -b $(BUILD)/cov/maquina.o
	@mv *.gcov $(BUILD)/cov/

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)
