all: stack_machine.s
	spim src/stack_machine.s

stack_machine.s:
	coolc src/stack_machine.cl src/atoi.cl

test: 
	./lexer_smoke_test
	python3 -m unittest tests/lexer_tests/lexer_end_to_end_test.py

install:
	git clone https://github.com/google/googletest
	ln -s build/compile_commands.json compile_commands.json

build:
	cmake -Bbuild -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	cd build/; make

.PHONY: build
