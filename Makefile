test: lexer_test

lexer_test:
	./lexer_smoke_test
	python3 -m unittest tests/lexer_tests/lexer_end_to_end_test.py

build: googletest compile_commands.json
	cmake -Bbuild -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_CXX_COMPILER=clang++
	cd build/; make -j4

googletest:
	git clone https://github.com/google/googletest

compile_commands.json:
	-ln -s build/compile_commands.json compile_commands.json

format:
	find include -iname *.h -o -iname *.cpp | xargs clang-format -i
	find src -iname *.h -o -iname *.cpp | xargs clang-format -i

stack: examples/stack_machine.s
	spim examples/stack_machine.s

stack_test: examples/stack_machine.s
	python3 -m unittest tests/stack_tests/stack_test.py

examples/stack_machine.s:
	coolc examples/stack_machine.cl examples/atoi.cl

clean:
	-rm examples/stack_machine.s
	-rm lexer
	-rm lexer_smoke_test

.PHONY: build clean lexer_test format
