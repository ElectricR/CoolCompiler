all: stack_machine.s
	spim src/stack_machine.s

stack_machine.s:
	coolc src/stack_machine.cl src/atoi.cl

test: stack_machine.s
	python3 -m unittest tests/test.py
