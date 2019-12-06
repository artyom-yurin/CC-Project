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

       sipush 5
       istore 1
       ldc 3.2
       fstore 2

       iload 1
       fload 2
       dadd
       dstore 3
       getstatic java/lang/System/out Ljava/io/PrintStream;
       fload 3
       invokevirtual java/io/PrintStream/println(F)V
       return
.end method