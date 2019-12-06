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
   FOR1:
       iload 1
       sipush 3 ; i < X
       if_icmpge EXIT1

       sipush 0
       istore 2
     FOR2:
       iload 2
       sipush 2 ; j < Y
       if_icmpge EXIT2

       sipush 0
       istore 3
         FOR3:
           iload 3
           sipush 1
           if_icmpge EXIT3


               getstatic java/lang/System/out Ljava/io/PrintStream;
               iload 2
               iload 3
               iadd
               invokevirtual java/io/PrintStream/println(I)V

            iinc 3 1
            goto FOR3
          EXIT3:
       iinc 2 1
       goto FOR2
     EXIT2:

       iinc 1 1
       goto FOR1
    EXIT1:

       return

.end method