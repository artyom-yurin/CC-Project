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

       sipush 4
       istore 2

       iload 1
       iload 2
       if_icmpge Go

       iload 1
       iconst_1
       iadd
       istore 1
       getstatic java/lang/System/out Ljava/io/PrintStream;
       iload 1
       invokevirtual java/io/PrintStream/println(I)V
   Go:
       getstatic java/lang/System/out Ljava/io/PrintStream;
       iload 1
       invokevirtual java/io/PrintStream/println(I)V
       return
.end method