import unittest
import subprocess

class LexerTest(unittest.TestCase):
    def check(self, *file_name):
        reference_proc = subprocess.Popen(["./reference/bin/lexer", *file_name], stdout=subprocess.PIPE)
        reference_out = reference_proc.communicate()[0]

        my_proc = subprocess.Popen(["./lexer", *file_name], stdout=subprocess.PIPE)
        my_out = my_proc.communicate()[0]
        self.assertEqual(reference_out.decode("UTF-8"), my_out.decode("UTF-8"))
        self.assertEqual(reference_proc.returncode, my_proc.returncode)


    def test_stack(self):
        self.check("./examples/stack_machine.cl")

    def test_invalid(self):
        self.check("./tests/lexer_tests/examples/invalid_program.cl")

    def test_atoi(self):
        self.check("./examples/atoi.cl")

    def test_hello_world(self):
        self.check("./reference/examples/hello_world.cl")

    def test_cool(self):
        self.check("./reference/examples/cool.cl")

    def test_io(self):
        self.check("./reference/examples/io.cl")

    def test_new_complex(self):
        self.check("./reference/examples/new_complex.cl")

    def test_multiple_files(self):
        self.check("./reference/examples/new_complex.cl", "./reference/examples/io.cl", "./reference/examples/hello_world.cl")

    def test_oneliner(self):
        self.check("./tests/lexer_tests/examples/stack_machine_oneliner.cl")

