.class public examples/All
.super java/lang/Object

.method public <init>()V
   aload_0
   invokenonvirtual java/lang/Object/<init>()V
   return
.end method

.method public static main([Ljava/lang/String;)V
       .limit locals 4
       .limit stack 8

       ldc 4.2  ; 4.2 - float
       fstore 1
       ldc 3.0  ; 3.0 - float
       fstore 2
       fload 1
       fload 2
       fdiv
       fstore 3
       getstatic java/lang/System/out Ljava/io/PrintStream;
       fload 3
       invokevirtual java/io/PrintStream/println(F)V
       return
.end method