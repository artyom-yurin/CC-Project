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

       ldc 5.2  ; float
       fstore 1

       ldc 5.2  ; float
       fstore 2

       fload 1
       fload 2
       fcmpl
       ifeq L3
       iconst_1  ; 1 - true
       goto L4
   L3:
       iconst_0  ; 0 - false
   L4:
       istore 3

       getstatic java/lang/System/out Ljava/io/PrintStream;
       iload 3
       invokevirtual java/io/PrintStream/println(I)V
       return
.end method