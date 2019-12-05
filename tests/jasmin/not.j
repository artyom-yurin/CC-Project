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

       iconst_1 ; 0-true 1-false
       istore 1

       iload 1
       ifne L2
       iconst_1
       goto L3
  L2:
       iconst_0
  L3:
       istore 2
       getstatic java/lang/System/out Ljava/io/PrintStream;
       iload 2
       invokevirtual java/io/PrintStream/println(I)V
       return
.end method