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
Alice_protObj:
       .word 5
       .word 3
       .word Alice_dispTab

       .word -1
Bob_protObj:
       .word 6
       .word 3
       .word Bob_dispTab

       .word -1
John_protObj:
       .word 7
       .word 3
       .word John_dispTab

       .word -1
Main_protObj:
       .word 8
       .word 3
       .word Main_dispTab

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

Alice_dispTab:
       .word Object.abort
       .word Object.type_name
       .word Object.copy
       .word IO.out_string
       .word IO.out_int
       .word IO.in_string
       .word IO.in_int
       .word Alice.responce

Bob_dispTab:
       .word Object.abort
       .word Object.type_name
       .word Object.copy
       .word IO.out_string
       .word IO.out_int
       .word IO.in_string
       .word IO.in_int
       .word Bob.responce

John_dispTab:
       .word Object.abort
       .word Object.type_name
       .word Object.copy
       .word IO.out_string
       .word IO.out_int
       .word IO.in_string
       .word IO.in_int
       .word John.greet

Main_dispTab:
       .word Object.abort
       .word Object.type_name
       .word Object.copy
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
       .word Alice_protObj
       .word Alice_init
       .word Bob_protObj
       .word Bob_init
       .word John_protObj
       .word John_init
       .word Main_protObj
       .word Main_init

class_nameTab:
       .word str_constObject
       .word str_constInt
       .word str_constBool
       .word str_constString
       .word str_constIO
       .word str_constAlice
       .word str_constBob
       .word str_constJohn
       .word str_constMain

       .word -1
int_const0:
       .word 1
       .word 4
       .word Int_dispTab
       .word 0

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
int_const8:
       .word 1
       .word 4
       .word Int_dispTab
       .word 8

       .word -1
int_const14:
       .word 1
       .word 4
       .word Int_dispTab
       .word 14

       .word -1
int_const33:
       .word 1
       .word 4
       .word Int_dispTab
       .word 33

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
str_constAlice:
       .word 3
       .word 6
       .word String_dispTab
       .word int_const5
      .ascii "Alice"
       .byte 0

       .word -1
str_constBob:
       .word 3
       .word 5
       .word String_dispTab
       .word int_const3
      .ascii "Bob"
       .byte 0

       .word -1
str_constJohn:
       .word 3
       .word 6
       .word String_dispTab
       .word int_const4
      .ascii "John"
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
       .word 13
       .word String_dispTab
       .word int_const33
      .ascii "Don't want to talk to you, John.\n"
       .byte 0

       .word -1
str_const2:
       .word 3
       .word 8
       .word String_dispTab
       .word int_const14
      .ascii "Oh, hi, John!\n"
       .byte 0

       .word -1
str_const3:
       .word 3
       .word 7
       .word String_dispTab
       .word int_const8
      .ascii "Hi all!\n"
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

Alice_init:
          jr $ra

Bob_init:
          jr $ra

John_init:
          jr $ra

Main_init:
          jr $ra

Alice.responce:
       addiu $sp $sp -12
          sw $fp 12($sp)
          sw $s0 8($sp)
          sw $ra 4($sp)
       addiu $fp $sp 4
        move $s0 $a0

          la $a0 str_const1
          sw $a0 0($sp)
       addiu $sp $sp -4
        move $a0 $s0

         bne $a0 $zero label0
          la $a0 str_const_path
          li $t1 3
         jal _dispatch_abort
label0:
          lw $t1 8($a0)
          lw $t1 12($t1)
        jalr $t1

          lw $fp 12($sp)
          lw $s0 8($sp)
          lw $ra 4($sp)
       addiu $sp $sp 12
          jr $ra

Bob.responce:
       addiu $sp $sp -12
          sw $fp 12($sp)
          sw $s0 8($sp)
          sw $ra 4($sp)
       addiu $fp $sp 4
        move $s0 $a0

          la $a0 str_const2
          sw $a0 0($sp)
       addiu $sp $sp -4
        move $a0 $s0

         bne $a0 $zero label1
          la $a0 str_const_path
          li $t1 9
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

John.greet:
       addiu $sp $sp -12
          sw $fp 12($sp)
          sw $s0 8($sp)
          sw $ra 4($sp)
       addiu $fp $sp 4
        move $s0 $a0

          la $a0 str_const3
          sw $a0 0($sp)
       addiu $sp $sp -4
        move $a0 $s0

         bne $a0 $zero label2
          la $a0 str_const_path
          li $t1 16
         jal _dispatch_abort
label2:
          lw $t1 8($a0)
          lw $t1 12($t1)
        jalr $t1

          la $a0 Alice_protObj
         jal Object.copy
         jal Alice_init

         bne $a0 $zero label3
          la $a0 str_const_path
          li $t1 17
         jal _dispatch_abort
label3:
          lw $t1 8($a0)
          lw $t1 28($t1)
        jalr $t1

          la $a0 Bob_protObj
         jal Object.copy
         jal Bob_init

         bne $a0 $zero label4
          la $a0 str_const_path
          li $t1 18
         jal _dispatch_abort
label4:
          lw $t1 8($a0)
          lw $t1 28($t1)
        jalr $t1

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

          la $a0 John_protObj
         jal Object.copy
         jal John_init

         bne $a0 $zero label5
          la $a0 str_const_path
          li $t1 25
         jal _dispatch_abort
label5:
          lw $t1 8($a0)
          lw $t1 28($t1)
        jalr $t1

          lw $fp 12($sp)
          lw $s0 8($sp)
          lw $ra 4($sp)
       addiu $sp $sp 12
          jr $ra

