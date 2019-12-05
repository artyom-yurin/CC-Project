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

       sipush 0
       istore 1

  Loop:
       iload 1
       sipush 10
       if_icmpge Exit
       getstatic java/lang/System/out Ljava/io/PrintStream;
       iload 1
       invokevirtual java/io/PrintStream/println(I)V
       iinc 1 1
       goto Loop

  Exit:
       return
.end method