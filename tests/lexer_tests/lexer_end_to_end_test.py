import unittest
import subprocess

class LexerTest(unittest.TestCase):
    def check(self, file_name):
        reference_proc = subprocess.Popen(["./reference/bin/lexer", file_name], stdout=subprocess.PIPE)
        reference_out = reference_proc.communicate()[0]

        my_proc = subprocess.Popen(["./lexer", file_name], stdout=subprocess.PIPE)
        my_out = my_proc.communicate()[0]
        self.assertEqual(reference_out.decode("UTF-8"), my_out.decode("UTF-8"))


    def test_stack(self):
        self.check("./examples/stack_machine.cl")

    def test_invalid(self):
        self.check("./tests/lexer_tests/examples/invalid_program.cl")

    @unittest.skip
    def test_atoi(self):
        self.check("./examples/atoi.cl")

    def test_hello_world(self):
        self.check("./reference/examples/hello_world.cl")

