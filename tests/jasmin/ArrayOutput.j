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
   
   getstatic java/lang/System.out Ljava/io/PrintStream;
   ldc "["
   invokevirtual java/io/PrintStream.print(Ljava/lang/String;)V
   getstatic java/lang/System.out Ljava/io/PrintStream;
   aload 0 ; массив который загружаем
   iconst_0
   aaload
   invokevirtual java/io/PrintStream.print(Ljava/lang/String;)V ; ИЗМЕНИТЬ ТИП АРГУМЕНТА ПРИ ВЫВОДЕ МАССИВА ДРУГОГО ТИПА
   iconst_1
   istore 1
  L3:
   iload 1
   aload 0 ; массив который загружаем
   arraylength
   if_icmpge L4
   getstatic java/lang/System.out Ljava/io/PrintStream;
   ldc ", "
   invokevirtual java/io/PrintStream.print(Ljava/lang/String;)V
   getstatic java/lang/System.out Ljava/io/PrintStream;
   aload 0 ; массив который загружаем
   iload 1
   aaload
   invokevirtual java/io/PrintStream.print(Ljava/lang/String;)V ; ИЗМЕНИТЬ ТИП АРГУМЕНТА ПРИ ВЫВОДЕ МАССИВА ДРУГОГО ТИПА
   iinc 1 1
   goto L3
  L4:
   getstatic java/lang/System.out Ljava/io/PrintStream;
   ldc "]"
   invokevirtual java/io/PrintStream.println(Ljava/lang/String;)V
   return
   
   return
.end method
