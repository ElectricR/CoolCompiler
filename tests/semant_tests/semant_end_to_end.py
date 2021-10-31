import unittest
import subprocess

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

    def test_lubtest(self):
        self.check(f"tests/semant_tests/examples/lubtest.test")

    def test_baddispatch(self):
        self.check(f"tests/semant_tests/examples/baddispatch.test")

    def test_returntypenoexist(self):
        self.check(f"tests/semant_tests/examples/returntypenoexist.test")

    def test_overridingmethod2(self):
        self.check(f"tests/semant_tests/examples/overridingmethod2.test")

    def test_list(self):
        self.check(f"tests/semant_tests/examples/list.test")

    def test_overridingmethod(self):
        self.check(f"tests/semant_tests/examples/overridingmethod.test")

    def test_classes(self):
        self.check(f"tests/semant_tests/examples/classes.test")

    def test_selftyperedeclared(self):
        self.check(f"tests/semant_tests/examples/selftyperedeclared.test")

    def test_hairyscary(self):
        self.check(f"tests/semant_tests/examples/hairyscary.tets")

    def test_basic(self):
        self.check(f"tests/semant_tests/examples/basic.test")

    def test_attroverride(self):
        self.check(f"tests/semant_tests/examples/attroverride.test")

    def test_methodnameclash(self):
        self.check(f"tests/semant_tests/examples/methodnameclash.test")

    def test_letselftype(self):
        self.check(f"tests/semant_tests/examples/letselftype.test")

    def test_badequalitytest(self):
        self.check(f"tests/semant_tests/examples/badequalitytest.test")

    def test_isvoid(self):
        self.check(f"tests/semant_tests/examples/isvoid.test")

    def test_scopes(self):
        self.check(f"tests/semant_tests/examples/scopes.test")

    def test_redefinedclass(self):
        self.check(f"tests/semant_tests/examples/redefinedclass.test")

    def test_selftype_method1(self):
        self.check(f"tests/semant_tests/examples/selftype_method1.test")

    def test_inheritsObject(self):
        self.check(f"tests/semant_tests/examples/inheritsObject.test")

    def test_letbadinit(self):
        self.check(f"tests/semant_tests/examples/letbadinit.test")

    def test_badargs1(self):
        self.check(f"tests/semant_tests/examples/badargs1.test")

    def test_cycleinmethods(self):
        self.check(f"tests/semant_tests/examples/cycleinmethods.test")

    def test_badmethodcallsitself(self):
        self.check(f"tests/semant_tests/examples/badmethodcallsitself.test")

    def test_overridingmethod4(self):
        self.check(f"tests/semant_tests/examples/overridingmethod4.test")

    def test_simple_dispatch_self_type(self):
        self.check(f"tests/semant_tests/examples/simple_dispatch_self_type.test")

    def test_assignment(self):
        self.check(f"tests/semant_tests/examples/assignment.test")

    def test_outofscope(self):
        self.check(f"tests/semant_tests/examples/outofscope.test")

    def test_objectdispatchabort(self):
        self.check(f"tests/semant_tests/examples/objectdispatchabort.test")

    def test_subtypemethodreturn(self):
        self.check(f"tests/semant_tests/examples/subtypemethodreturn.test")

    def test_dupformals(self):
        self.check(f"tests/semant_tests/examples/dupformals.test")

    def test_cells(self):
        self.check(f"tests/semant_tests/examples/cells.test")

    def test_staticdispatch(self):
        self.check(f"tests/semant_tests/examples/staticdispatch.test")

    def test_newselftype(self):
        self.check(f"tests/semant_tests/examples/newselftype.test")

    def test_function(self):
        self.check(f"tests/semant_tests/examples/function.cl")

    def test_dispatch(self):
        self.check(f"tests/semant_tests/examples/dispatch.test")

    def test_overriderenamearg(self):
        self.check(f"tests/semant_tests/examples/overriderenamearg.test")

    def test_attrbadinit(self):
        self.check(f"tests/semant_tests/examples/attrbadinit.test")

    def test_overridingmethod3(self):
        self.check(f"tests/semant_tests/examples/overridingmethod3.test")

    def test_simplecase(self):
        self.check(f"tests/semant_tests/examples/simplecase.test")

    def test_trickyatdispatch2(self):
        self.check(f"tests/semant_tests/examples/trickyatdispatch2.test")

    def test_assignnoconform(self):
        self.check(f"tests/semant_tests/examples/assignnoconform.test")

    def test_dot_expression_multiple(self):
        self.check(f"tests/semant_tests/examples/dot_expression_multiple.cl")

    def test_simplearith(self):
        self.check(f"tests/semant_tests/examples/simplearith.test")

    def test_simple_int_main(self):
        self.check(f"tests/semant_tests/examples/simple_int_main.cl")

    def test_stringtest(self):
        self.check(f"tests/semant_tests/examples/stringtest.test")

    def test_trickyatdispatch(self):
        self.check(f"tests/semant_tests/examples/trickyatdispatch.test")

    def test_selfinformalparameter(self):
        self.check(f"tests/semant_tests/examples/selfinformalparameter.test")

    def test_compare(self):
        self.check(f"tests/semant_tests/examples/compare.test")

    def test_self_assignment(self):
        self.check(f"tests/semant_tests/examples/self_assignment.test")

    def test_badarith(self):
        self.check(f"tests/semant_tests/examples/badarith.test")

    def test_nomain(self):
        self.check(f"tests/semant_tests/examples/nomain.test")

    def test_initwithself(self):
        self.check(f"tests/semant_tests/examples/initwithself.test")

    def test_if(self):
        self.check(f"tests/semant_tests/examples/if.test")

    def test_neg(self):
        self.check(f"tests/semant_tests/examples/neg.test")

    def test_selftype_method2(self):
        self.check(f"tests/semant_tests/examples/selftype_method2.test")

    def test_inheritsbool(self):
        self.check(f"tests/semant_tests/examples/inheritsbool.test")

    def test_field_redefinition(self):
        self.check(f"tests/semant_tests/examples/field_redefinition.test")

    def test_signaturechange(self):
        self.check(f"tests/semant_tests/examples/signaturechange.test")

    def test_badredefineint(self):
        self.check(f"tests/semant_tests/examples/badredefineint.test")

    def test_selftypeparameterposition(self):
        self.check(f"tests/semant_tests/examples/selftypeparameterposition.test")

    def test_missingclass(self):
        self.check(f"tests/semant_tests/examples/missingclass.test")

    def test_badwhilebody(self):
        self.check(f"tests/semant_tests/examples/badwhilebody.test")

    def test_caseidenticalbranch(self):
        self.check(f"tests/semant_tests/examples/caseidenticalbranch.test")

    def test_inheritsstring(self):
        self.check(f"tests/semant_tests/examples/inheritsstring.test")

    def test_redefinedobject(self):
        self.check(f"tests/semant_tests/examples/redefinedobject.test")

    def test_badwhilecond(self):
        self.check(f"tests/semant_tests/examples/badwhilecond.test")

    def test_expressionblock(self):
        self.check(f"tests/semant_tests/examples/expressionblock.test")

    def test_letinit(self):
        self.check(f"tests/semant_tests/examples/letinit.test")

    def test_methodcallsitself(self):
        self.check(f"tests/semant_tests/examples/methodcallsitself.test")

    def test_anattributenamedself(self):
        self.check(f"tests/semant_tests/examples/anattributenamedself.test")

    def test_letnoinit(self):
        self.check(f"tests/semant_tests/examples/letnoinit.test")

    def test_letself(self):
        self.check(f"tests/semant_tests/examples/letself.test")

    def test_io(self):
        self.check(f"tests/semant_tests/examples/io.test")

    def test_selftypebadreturn(self):
        self.check(f"tests/semant_tests/examples/selftypebadreturn.test")

    def test_inheritsselftype(self):
        self.check(f"tests/semant_tests/examples/inheritsselftype.test")

    def test_basicclassestree(self):
        self.check(f"tests/semant_tests/examples/basicclassestree.test")

    def test_comparisons(self):
        self.check(f"tests/semant_tests/examples/comparisons.test")

    def test_badstaticdispatch(self):
        self.check(f"tests/semant_tests/examples/badstaticdispatch.test")

    def test_badequalitytest2(self):
        self.check(f"tests/semant_tests/examples/badequalitytest2.test")

    def test_letshadows(self):
        self.check(f"tests/semant_tests/examples/letshadows.test")

    def test_forwardinherits(self):
        self.check(f"tests/semant_tests/examples/forwardinherits.test")

