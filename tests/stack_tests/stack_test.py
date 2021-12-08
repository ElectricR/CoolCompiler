import unittest
import subprocess

class StackTest(unittest.TestCase):

    pattern = "SPIM Version 6.5 of January 4, 2003\nCopyright 1990-2003 by James R. Larus (larus@cs.wisc.edu).\nAll Rights Reserved.\nSee the file README for a full copyright notice.\nLoaded: ../lib/trap.handler\n{0}COOL program successfully executed\n"

    def check(self, in_value, answer):
        p = subprocess.Popen(["reference/bin/spim", "examples/stack_machine.s"], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
        out = p.communicate(input=in_value)[0]
        self.assertEqual(out.decode("UTF-8"), self.pattern.format(answer))


    def test_empty(self):
        in_value = b"x\n"
        answer = ">"
        self.check(in_value, answer)


    def test_1(self):
        in_value = b"4\nd\nx\n"
        answer = ">>4\n>"
        self.check(in_value, answer)


    def test_2(self):
        in_value = b"2\n5\n+\ne\nd\nx\n"
        answer = ">>>>>7\n>"
        self.check(in_value, answer)


    def test_3(self):
        in_value = b"5\n2\n7\n11\n+\ne\ns\ne\n+\ne\n+\ne\nd\nx\n"
        answer = ">>>>>>>>>>>>>25\n>"
        self.check(in_value, answer)


    def test_4(self):
        in_value = b"1\n2\n3\n4\n5\n6\nd\nx\n"
        answer = ">>>>>>>6\n5\n4\n3\n2\n1\n>"
        self.check(in_value, answer)


    def test_5(self):
        in_value = b"1\nd\nd\nd\nd\nd\nd\nx\n"
        answer = ">>1\n>1\n>1\n>1\n>1\n>1\n>"
        self.check(in_value, answer)


    def test_6(self):
        in_value = b"4\n5\ns\ne\ns\ne\nd\nx\n"
        answer = ">>>>>>>5\n4\n>"
        self.check(in_value, answer)

