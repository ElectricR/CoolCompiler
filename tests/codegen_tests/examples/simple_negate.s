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
int_const6:
       .word 1
       .word 4
       .word Int_dispTab
       .word 6

       .word -1
int_const42:
       .word 1
       .word 4
       .word Int_dispTab
       .word 42

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
          la $a0 int_const42
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

          la $a0 str_const1
          sw $a0 0($sp)
       addiu $sp $sp -4
          lw $a0 12($s0)

         jal Object.copy
          lw $t0 12($a0)
         neg $t0 $t0
          sw $t0 12($a0)
         jal Object.copy
          lw $t0 12($a0)
         neg $t0 $t0
          sw $t0 12($a0)
         jal Object.copy
          lw $t0 12($a0)
         neg $t0 $t0
          sw $t0 12($a0)
          sw $a0 0($sp)
       addiu $sp $sp -4
        move $a0 $s0

         bne $a0 $zero label0
          la $a0 str_const_path
          li $t1 5
         jal _dispatch_abort
label0:
          lw $t1 8($a0)
          lw $t1 16($t1)
        jalr $t1

         bne $a0 $zero label1
          la $a0 str_const_path
          li $t1 5
         jal _dispatch_abort
label1:
          lw $t1 8($a0)
          lw $t1 12($t1)
        jalr $t1

          lw $fp 12($sp)
          lw $s0 8($sp)
          lw $ra 4($sp)
       addiu $sp $sp 12
          jr $ra

