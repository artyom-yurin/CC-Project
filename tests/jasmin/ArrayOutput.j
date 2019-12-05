.class public temp/Test ; не трогать
.super java/lang/Object ; не трогать

; standard initializer
.method public <init>()V
   aload_0
   invokenonvirtual java/lang/Object/<init>()V
   return
.end method

.method public static main([Ljava/lang/String;)V
   .limit locals 2
   .limit stack 4

   getstatic java/lang/System/out Ljava/io/PrintStream;
   ldc "["
   invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V

   getstatic java/lang/System/out Ljava/io/PrintStream;
   aload 0
   iconst_0
   aaload
   invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V

   iconst_1
   istore 1

next_iteration:
   iload 1
   aload 0
   arraylength
   if_icmpge for_end

   getstatic java/lang/System/out Ljava/io/PrintStream;
   new java/lang/StringBuilder
   dup
   invokespecial java/lang/StringBuilder.<init>()V
   ldc ", "
   invokevirtual java/lang/StringBuilder.append(Ljava/lang/String;)Ljava/lang/StringBuilder;
   aload 0
   iload 1
   aaload
   invokevirtual java/lang/StringBuilder.append(Ljava/lang/String;)Ljava/lang/StringBuilder;
   invokevirtual java/lang/StringBuilder.toString()Ljava/lang/String;
   invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V

   iinc 1 1
   goto next_iteration

for_end:
   getstatic java/lang/System/out Ljava/io/PrintStream;
   ldc "]"
   invokevirtual java/io/PrintStream.println(Ljava/lang/String;)V

   return
.end method
