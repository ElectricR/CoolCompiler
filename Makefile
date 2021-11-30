all: build
	./codegen nao.cl
	cat a.s

test: codegen_test

lexer_test: build
	./lexer_smoke_test
	python3 -m unittest tests/lexer_tests/lexer_end_to_end_test.py

parser_test: build
	./parser_smoke_test
	python3 -m unittest tests/parser_tests/parser_end_to_end.py

semant_test: build
	./semant_smoke_test
	python3 -m unittest tests/semant_tests/semant_end_to_end.py

codegen_test: build
	./prototype_smoke_test
	./disptable_smoke_test
	./constants_smoke_test

build: googletest compile_commands.json
	cmake -Bbuild -DCMAKE_EXPORT_COMPILE_COMMANDS=1 
	cd build/; make -j4

googletest:
	git clone https://github.com/google/googletest

compile_commands.json:
	-ln -s build/compile_commands.json compile_commands.json

format:
	find include -iname *.h -o -iname *.cpp | xargs clang-format -i
	find src -iname *.h -o -iname *.cpp | xargs clang-format -i
	find tests -iname *.h -o -iname *.cpp | xargs clang-format -i

stack: examples/stack_machine.s
	spim examples/stack_machine.s

stack_test: examples/stack_machine.s
	python3 -m unittest tests/stack_tests/stack_test.py

examples/stack_machine.s:
	coolc examples/stack_machine.cl examples/atoi.cl

clean:
	-rm examples/stack_machine.s
	-rm lexer
	-rm parser
	-rm semant
	-rm codegen
	-rm lexer_smoke_test
	-rm parser_smoke_test
	-rm semant_smoke_test
	-rm datagen_smoke_test
	-rm build -r

.PHONY: build clean lexer_test parser_test format
