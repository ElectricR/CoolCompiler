.data
_int_tag:
       .word 1

_bool_tag:
       .word 2

_string_tag:
       .word 3

_MemMgr_INITIALIZER:
       .word _NoGC_Init
_MemMgr_COLLECTOR:
       .word _NoGC_Collect
_MemMgr_TEST:
       .word 0

       .word -1
Object_protObj:
       .word 0
       .word 3
       .word Object_dispTab

       .word -1
Int_protObj:
       .word 1
       .word 4
       .word Int_dispTab
       .word 0

       .word -1
Bool_protObj:
       .word 2
       .word 4
       .word Bool_dispTab
       .word 0

       .word -1
String_protObj:
       .word 3
       .word 5
       .word String_dispTab
       .word int_const0
       .word 0

       .word -1
IO_protObj:
       .word 4
       .word 3
       .word IO_dispTab

       .word -1
Main_protObj:
       .word 5
       .word 4
       .word Main_dispTab
       .word int_const0

Object_dispTab:
       .word Object.abort
       .word Object.type_name
       .word Object.copy

Int_dispTab:
       .word Object.abort
       .word Object.type_name
       .word Object.copy

Bool_dispTab:
       .word Object.abort
       .word Object.type_name
       .word Object.copy

String_dispTab:
       .word Object.abort
       .word Object.type_name
       .word Object.copy
       .word String.length
       .word String.concat
       .word String.substr

IO_dispTab:
       .word Object.abort
       .word Object.type_name
       .word Object.copy
       .word IO.out_string
       .word IO.out_int
       .word IO.in_string
       .word IO.in_int

Main_dispTab:
       .word Object.abort
       .word Object.type_name
       .word Object.copy
       .word IO.out_string
       .word IO.out_int
       .word IO.in_string
       .word IO.in_int
       .word Main.main

class_objTab:
       .word Object_protObj
       .word Object_init
       .word Int_protObj
       .word Int_init
       .word Bool_protObj
       .word Bool_init
       .word String_protObj
       .word String_init
       .word IO_protObj
       .word IO_init
       .word Main_protObj
       .word Main_init

class_nameTab:
       .word str_constObject
       .word str_constInt
       .word str_constBool
       .word str_constString
       .word str_constIO
       .word str_constMain

       .word -1
int_const0:
       .word 1
       .word 4
       .word Int_dispTab
       .word 0

       .word -1
int_const1:
       .word 1
       .word 4
       .word Int_dispTab
       .word 1

       .word -1
int_const2:
       .word 1
       .word 4
       .word Int_dispTab
       .word 2

       .word -1
int_const3:
       .word 1
       .word 4
       .word Int_dispTab
       .word 3

       .word -1
int_const4:
       .word 1
       .word 4
       .word Int_dispTab
       .word 4

       .word -1
int_const5:
       .word 1
       .word 4
       .word Int_dispTab
       .word 5

       .word -1
int_const6:
       .word 1
       .word 4
       .word Int_dispTab
       .word 6

       .word -1
int_const9:
       .word 1
       .word 4
       .word Int_dispTab
       .word 9

       .word -1
int_const15:
       .word 1
       .word 4
       .word Int_dispTab
       .word 15

       .word -1
int_const100:
       .word 1
       .word 4
       .word Int_dispTab
       .word 100

       .word -1
bool_const0:
       .word 2
       .word 4
       .word Bool_dispTab
       .word 0

       .word -1
bool_const1:
       .word 2
       .word 4
       .word Bool_dispTab
       .word 1

       .word -1
str_const_path:
       .word 3
       .word 5
       .word String_dispTab
       .word int_const0
       .byte 0

       .word -1
str_constObject:
       .word 3
       .word 6
       .word String_dispTab
       .word int_const6
      .ascii "Object"
       .byte 0

       .word -1
str_constInt:
       .word 3
       .word 5
       .word String_dispTab
       .word int_const3
      .ascii "Int"
       .byte 0

       .word -1
str_constBool:
       .word 3
       .word 6
       .word String_dispTab
       .word int_const4
      .ascii "Bool"
       .byte 0

       .word -1
str_constString:
       .word 3
       .word 6
       .word String_dispTab
       .word int_const6
      .ascii "String"
       .byte 0

       .word -1
str_constIO:
       .word 3
       .word 5
       .word String_dispTab
       .word int_const2
      .ascii "IO"
       .byte 0

       .word -1
str_constMain:
       .word 3
       .word 6
       .word String_dispTab
       .word int_const4
      .ascii "Main"
       .byte 0

       .word -1
str_const0:
       .word 3
       .word 5
       .word String_dispTab
       .word int_const0
       .byte 0

       .word -1
str_const1:
       .word 3
       .word 7
       .word String_dispTab
       .word int_const9
      .ascii "FizzBuzz\n"
       .byte 0

       .word -1
str_const2:
       .word 3
       .word 6
       .word String_dispTab
       .word int_const5
      .ascii "Fizz\n"
       .byte 0

       .word -1
str_const3:
       .word 3
       .word 6
       .word String_dispTab
       .word int_const5
      .ascii "Buzz\n"
       .byte 0

       .word -1
str_const4:
       .word 3
       .word 5
       .word String_dispTab
       .word int_const1
      .ascii "\n"
       .byte 0

heap_start:
       .word 0

.globl class_nameTab
.globl Main_protObj
.globl Int_protObj
.globl String_protObj
.globl bool_const0
.globl bool_const1
.globl _int_tag
.globl _bool_tag
.globl _string_tag
.globl _MemMgr_INITIALIZER
.globl _MemMgr_COLLECTOR
.globl _MemMgr_TEST
.globl heap_start
.globl Main_init
.globl Int_init
.globl String_init
.globl Bool_init
.globl Main.main

.text
Object_init:
          jr $ra

Int_init:
          jr $ra

Bool_init:
          jr $ra

String_init:
          jr $ra

IO_init:
          jr $ra

Main_init:
       addiu $sp $sp -12
          sw $fp 12($sp)
          sw $s0 8($sp)
          sw $ra 4($sp)
       addiu $fp $sp 4
        move $s0 $a0

         jal IO_init
          la $a0 int_const0
          sw $a0 12($s0)

        move $a0 $s0

          lw $fp 12($sp)
          lw $s0 8($sp)
          lw $ra 4($sp)
       addiu $sp $sp 12
          jr $ra

Main.main:
       addiu $sp $sp -12
          sw $fp 12($sp)
          sw $s0 8($sp)
          sw $ra 4($sp)
       addiu $fp $sp 4
        move $s0 $a0

label0:
          lw $a0 12($s0)

          sw $a0 0($sp)
       addiu $sp $sp -4
          la $a0 int_const100
        move $t2 $a0
          lw $t1 4($sp)
       addiu $sp $sp 4
          la $a0 bool_const1
         beq $t1 $t2 label1
          la $a1 bool_const0
         jal equality_test
label1:
          lw $t1 12($a0)
          la $a0 bool_const1
        beqz $t1 label2
          la $a0 bool_const0
label2:
          lw $t1 12($a0)
         beq $t1 $zero label3
          lw $a0 12($s0)

          sw $a0 0($sp)
       addiu $sp $sp -4
          la $a0 int_const15
        move $t2 $a0
          lw $t1 4($sp)
       addiu $sp $sp 4
          la $a0 bool_const1
         beq $t1 $t2 label4
          la $a1 bool_const0
         jal equality_test
label4:
          lw $t1 12($a0)
        beqz $t1 label5

          la $a0 str_const1
          sw $a0 0($sp)
       addiu $sp $sp -4
        move $a0 $s0

         bne $a0 $zero label7
          la $a0 str_const_path
          li $t1 7
         jal _dispatch_abort
label7:
          lw $t1 8($a0)
          lw $t1 12($t1)
        jalr $t1

           b label6
label5:
          lw $a0 12($s0)

          sw $a0 0($sp)
       addiu $sp $sp -4
          la $a0 int_const3
        move $t2 $a0
          lw $t1 4($sp)
       addiu $sp $sp 4
          la $a0 bool_const1
         beq $t1 $t2 label8
          la $a1 bool_const0
         jal equality_test
label8:
          lw $t1 12($a0)
        beqz $t1 label9

          la $a0 str_const2
          sw $a0 0($sp)
       addiu $sp $sp -4
        move $a0 $s0

         bne $a0 $zero label11
          la $a0 str_const_path
          li $t1 10
         jal _dispatch_abort
label11:
          lw $t1 8($a0)
          lw $t1 12($t1)
        jalr $t1

           b label10
label9:
          lw $a0 12($s0)

          sw $a0 0($sp)
       addiu $sp $sp -4
          la $a0 int_const5
        move $t2 $a0
          lw $t1 4($sp)
       addiu $sp $sp 4
          la $a0 bool_const1
         beq $t1 $t2 label12
          la $a1 bool_const0
         jal equality_test
label12:
          lw $t1 12($a0)
        beqz $t1 label13

          la $a0 str_const3
          sw $a0 0($sp)
       addiu $sp $sp -4
        move $a0 $s0

         bne $a0 $zero label15
          la $a0 str_const_path
          li $t1 13
         jal _dispatch_abort
label15:
          lw $t1 8($a0)
          lw $t1 12($t1)
        jalr $t1

           b label14
label13:
          la $a0 str_const4
          sw $a0 0($sp)
       addiu $sp $sp -4
          lw $a0 12($s0)

          sw $a0 0($sp)
       addiu $sp $sp -4
        move $a0 $s0

         bne $a0 $zero label16
          la $a0 str_const_path
          li $t1 15
         jal _dispatch_abort
label16:
          lw $t1 8($a0)
          lw $t1 16($t1)
        jalr $t1

         bne $a0 $zero label17
          la $a0 str_const_path
          li $t1 15
         jal _dispatch_abort
label17:
          lw $t1 8($a0)
          lw $t1 12($t1)
        jalr $t1

label14:
label10:
label6:
          lw $a0 12($s0)

          sw $a0 0($sp)
       addiu $sp $sp -4
          la $a0 int_const1
         jal Object.copy
          lw $t0 4($sp)
       addiu $sp $sp 4
          lw $t1 12($t0)
          lw $t2 12($a0)
         add $t1 $t1 $t2
          sw $t1 12($a0)

          sw $a0 12($s0)

           b label0
label3:
        move $a0 $zero
          lw $fp 12($sp)
          lw $s0 8($sp)
          lw $ra 4($sp)
       addiu $sp $sp 12
          jr $ra

