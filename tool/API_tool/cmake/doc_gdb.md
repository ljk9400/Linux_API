   
## GDB   
   
### GCC   
GCC 使用的命令语法如下：   
4.1 输入选项   
gcc [选项] 输入的文件名   
常用选项：   
   
-o：   
小写字母“o”，指定生成的可执行文件的名字，不指定的话生成的可执行文件名为 a.out。   
-E：   
只进行预处理，既不编译，也不汇编。   
-S：   
只编译，不汇编。   
-c：   
编译并汇编，但不进行链接。    
-g：   
生成的可执行文件带调试信息，方便使用 gdb 进行调试。   
-Ox：   
 大写字母“O”加数字，设置程序的优化等级，如“-O0”“-O1”“-O2”“-O3”，数字越大代码的优化等级越高，编译出来的程序一般会越小，但有可能会导致程序不正常运行。    
4.2  gcc 符号解释   
$@   
 --代表目标文件(target)      
$^   
--代表所有的依赖文件(components)   
$<   
 --代表第一个依赖文件(components中最左边的那个)。   
$?   
--代表当前目标所依赖的文件列表中比当前目标文件还要新的文件。      
$*   
--不包括后缀名的当前依赖文件的名字   
4.3 编译路径指定   
-I ( i 的大写)：   
指定头文件路径（相对路径或绝对路径，建议相对路径）   
-i ：   
指定头文件名字 (一般不使用，而是直接放在**.c 文件中通过#include<***.h> 添加)   
-L ：   
指定连接的动态库或者静态库路径（相对路径或绝对路径，建议相对路径）   
-l (L的小写)：   
指定需要链接的库的名字（链接 libc.a : -lc 链接动态库：libc.so : -lc）。 注意：-l后面可以直接添加库名省去“lib”和“.so”或“.a”。 -l(L的小写)链接的到底是动态库还是静态库，如果链接路径下同时有 .so 和 .a 那优先链接 .so 。   
4.4 动态库、静态库编译   
编译动态库   
gcc -shared -o libexample.so example.c   
编译静态库   
gcc -c example.c   
ar rcs libexample.a example.o   
使用动态库编译    
libabc.so(下面的编译命令不带lib和so,只添加了-l)   
gcc main.c -o main  -L （动态库目录）  -labc    
4.5 运行设置库路径   
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/the/path/you/want/set    
4.6 查看编译文件库依赖   
lld xxx.so 查看库依赖   
   
### GDB工具   
5.1 命令   
B (break) 断点当前位置, B+line 断点需要的line行   
R (run)开始执行   
C 执行到下个断点   
N (next)单步执行   
S 跳入执行   
P（print） 查看变量   
    
1. 查看某线称崩溃   
info threads   
    
2. 切换到线程6：   
thread 6   
    
3. 查看当前线程的堆栈跟踪信息：   
bt   
   
5.2 GDB多线程调试   
shell的命令：   
  (1)查看当前运行的进程：ps aux | grep book   
  (2)查看当前运行的轻量级进程：ps -aL | grep book   
  (3)查看主线程和子线程的关系：pstree -p 主线程id   
    
 gdb的命令：   
  (1)查看可切换调试的线程：info threads   
  (2)切换调试的线程：thread 线程id   
  (3)只运行当前线程：set scheduler-locking on   
  (4)运行全部的线程：set scheduler-locking off    
  (5)指定某线程执行某gdb命令：thread apply 线程id gdb_cmd   
  Eg:thread apply 3 n  只让线程3执行next   
      (6)全部的线程执行某gdb命令：thread apply all gdb_cmd   
    
5.3 GDB内存查询   
1、内存查看   
x/3uh 0x54320:从地址0x54320开始，读取3个双字节（h），以十六进制方式显示（x）   
3可以替换成任意正整数   
u可以替换成：   
d 按十进制格式显示变量 x 按十六进制格式显示变量 a 按十六进制格式显示变量 u 按十六进制格式显示无符号整型 o 按八进制格式显示变量 t 按二进制格式显示变量 c 按字符格式显示变量 f 按浮点数格式显示变量   
h可以替换成：   
b表示单字节，h表示双字节，w表示四字 节，g表示八字节   
    
### valgrind 工具   
在树莓派上用 valgrind  这个工具可以查查有没有没释放的内存   
    
### core dump工具   
1.  查看限制   
ulimit -c   
 如果显示 1024 那么说明 coredump 已经被开启。   
1024 限制产生的 core 文件的大小不能超过 1024kb，可以使用参数unlimited，取消该限制   
ulimit -c unlimited   
    
2. 查询core dump文件路径：   
方法1：   
cat /proc/sys/kernel/core_pattern   
方法2：   
/sbin/sysctl kernel.core_pattern   
   
3. 修改core dump文件路径：   
方法1：临时修改：修改/proc/sys/kernel/core_pattern文件，但/proc目录本身是动态加载的，每次系统重启都会重新加载，因此这种方法只能作为临时修改。   
/proc/sys/kernel/core_pattern   
例：echo ‘/var/log/%e.core.%p’ > /proc/sys/kernel/core_pattern   
方法2：永久修改：使用sysctl -w name=value命令。   
例：/sbin/sysctl -w kernel.core_pattern=/var/log/%e.core.%   
    
 gdb+core文件   
