import unittest
import subprocess
import os

class SemantTest(unittest.TestCase):
    def check(self, *file_name):
        print(f"Checking {file_name}...")
        reference_lexer_proc = subprocess.Popen(["./reference/bin/lexer", *file_name], stdout=subprocess.PIPE)
        reference_lexer_out = reference_lexer_proc.communicate()[0]

        reference_parser_proc = subprocess.Popen(["./reference/bin/parser"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        reference_parser_out = reference_parser_proc.communicate(reference_lexer_out)[0]

        reference_semant_proc = subprocess.Popen(["./reference/bin/semant"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        reference_out = reference_semant_proc.communicate(reference_parser_out)[0]


        my_proc = subprocess.Popen(["./semant", *file_name], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        my_out = my_proc.communicate()[0]
        if reference_semant_proc.returncode == 0:
            self.assertEqual(reference_out.decode("UTF-8"), my_out.decode("UTF-8"))
            self.assertEqual(0, my_proc.returncode)
        else:
            self.assertEqual(1, my_proc.returncode)


    def test_with_reference_tests(self):
        for file_name in os.listdir("tests/semant_tests/examples"):
            self.check(f"tests/semant_tests/examples/{file_name}")
