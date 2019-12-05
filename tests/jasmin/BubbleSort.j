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

       bipush 10
       newarray int

       dup
       bipush 0
       bipush 10
       iastore

       dup
       bipush 1
       bipush 9
       iastore

       dup
       bipush 2
       bipush 8
       iastore

       dup
       bipush 3
       bipush 7
       iastore

       dup
       bipush 4
       bipush 6
       iastore

       dup
       bipush 5
       bipush 5
       iastore

       dup
       bipush 6
       bipush 4
       iastore

       dup
       bipush 7
       bipush 3
       iastore

       dup
       bipush 8
       bipush 2
       iastore

       dup
       bipush 9
       bipush 1
       iastore

       astore 1

       aload 1
       invokestatic examples.All.bubbleSort([I)[I
       astore 1


       getstatic java/lang/System/out Ljava/io/PrintStream;
       ldc "["
       invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V

       getstatic java/lang/System/out Ljava/io/PrintStream;
       aload 1
       bipush 0
       iaload
       invokevirtual java/io/PrintStream/print(I)V

       bipush 1
       istore 2

   L4:
       iload 2
       aload 1
       arraylength
       if_icmpge L5

       getstatic java/lang/System/out Ljava/io/PrintStream;
       ldc ","
       invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V

       getstatic java/lang/System/out Ljava/io/PrintStream;
       aload 1
       iload 2
       iaload
       invokevirtual java/io/PrintStream/print(I)V

       iinc 2 1
       goto L4


    L5:
       getstatic java/lang/System/out Ljava/io/PrintStream;
       ldc "]"
       invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V

       return
.end method

.method public static bubbleSort([I)[I
       .limit locals 4
       .limit stack 8

       sipush 0
       istore 1
   L1:
       iload 1
       aload 0
       arraylength
       sipush 1
       isub
       if_icmpge L2

       sipush 0
       istore 2

    L4:
       iload 2
       aload 0
       arraylength
       iload 1
       isub
       sipush 1
       isub
       if_icmpge L5

       aload 0
       iload 2
       iaload
       aload 0
       iload 2
       sipush 1
       iadd
       iaload
       if_icmple L7

       aload 0
       iload 2
       iaload
       istore 3

       aload 0
       iload 2
       aload 0
       iload 2
       sipush 1
       iadd
       iaload
       iastore

       aload 0
       iload 2
       sipush 1
       iadd
       iload 3
       iastore

    L7:
       iinc 2 1
       goto L4

    L5:
       iinc 1 1
       goto L1
    L2:
       aload 0
       areturn
.end method