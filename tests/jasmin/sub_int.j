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

       sipush 2
       istore 1
       sipush 3
       istore 2
       iload 1
       iload 2
       isub
       istore 3
       getstatic java/lang/System/out Ljava/io/PrintStream;
       iload 3
       invokevirtual java/io/PrintStream/println(I)V
       return
.end method