import unittest
import subprocess
import os

class ParserTest(unittest.TestCase):
    def check(self, *file_name):
        print(f"Checking {file_name}...")
        reference_lexer_proc = subprocess.Popen(["./reference/bin/lexer", *file_name], stdout=subprocess.PIPE)
        reference_lexer_out = reference_lexer_proc.communicate()[0]
        reference_parser_proc = subprocess.Popen(["./reference/bin/parser"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        reference_out = reference_parser_proc.communicate(reference_lexer_out)[0]

        my_proc = subprocess.Popen(["./parser", *file_name], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        my_out = my_proc.communicate()[0]
        if reference_parser_proc.returncode == 0:
            self.assertEqual(reference_out.decode("UTF-8"), my_out.decode("UTF-8"))
            self.assertEqual(0, my_proc.returncode)
        else:
            self.assertEqual(1, my_proc.returncode)


    def test_hello_world(self):
        self.check("./reference/examples/hello_world.cl")

    def test_stack(self):
        self.check("./examples/stack_machine.cl")

    def test_atoi(self):
        self.check("./examples/atoi.cl")

    def test_cool(self):
        self.check("./reference/examples/cool.cl")

    def test_io(self):
        self.check("./reference/examples/io.cl")

    def test_new_complex(self):
        self.check("./reference/examples/new_complex.cl")

    def test_multiple_files(self):
        self.check("./reference/examples/new_complex.cl", "./reference/examples/io.cl", "./reference/examples/hello_world.cl")

    def test_with_reference_tests(self):
        for file_name in os.listdir("tests/parser_tests/examples"):
            self.check(f"tests/parser_tests/examples/{file_name}")
