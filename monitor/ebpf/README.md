### 性能分析工具
- 学习url

https://gitcode.com/eunomia-bpf/bpf-developer-tutorial/tree/main/src

- 工具安装步骤

```console
$ wget https://aka.pw/bpf-ecli -O ecli && chmod +x ./ecli
$ ./ecli -h
Usage: ecli [--help] [--version] [--json] [--no-cache] url-and-args
```

```console
$ wget https://github.com/eunomia-bpf/eunomia-bpf/releases/latest/download/ecc && chmod +x ./ecc
$ ./ecc -h
eunomia-bpf compiler
Usage: ecc [OPTIONS] <SOURCE_PATH> [EXPORT_EVENT_HEADER]
....

```shell
sudo apt install clang llvm
sudo apt install fuse3
```

使用 ecc 编译程序：

```console
$ ./ecc minimal.bpf.c
Compiling bpf object...
Packing ebpf object and config into package.json...
```

然后使用 ecli 运行编译后的程序：

```console
$ sudo ./ecli run package.json
Runing eBPF program...
```

- fentry-link.bpf.c 使用demo说明

代码

```c
#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

char LICENSE[] SEC("license") = "Dual BSD/GPL";

SEC("fentry/do_unlinkat")
int BPF_PROG(do_unlinkat, int dfd, struct filename *name)
{
    pid_t pid;

    pid = bpf_get_current_pid_tgid() >> 32;
    bpf_printk("fentry: pid = %d, filename = %s\n", pid, name->name);
    return 0;
}

SEC("fexit/do_unlinkat")
int BPF_PROG(do_unlinkat_exit, int dfd, struct filename *name, long ret)
{
    pid_t pid;

    pid = bpf_get_current_pid_tgid() >> 32;
    bpf_printk("fexit: pid = %d, filename = %s, ret = %ld\n", pid, name->name, ret);
    return 0;
}
```

1、do_unlinkat 是 Linux 内核中执行文件删除的底层函数。

2、SEC("fexit/do_unlinkat") 说明程序应挂载在内核的 do_unlinkat 函数返回时 eBPF 程序的部分和挂载点。

3、BPF_PROG 是一个宏，用于声明一个 eBPF 程序，do_unlinkat_exit 是此 eBPF 程序的名字。此程序将在 do_unlinkat 返回时被调用，
    并且提供了通过 do_unlinkat 的参数：目录文件描述符 dfd，文件名结构体指针 name，以及系统调用的返回值 ret。
    
4、在另外一个窗口中：

```shell
touch test_file
rm test_file
touch test_file2
rm test_file2
```

运行这段程序后，可以通过查看 `/sys/kernel/debug/tracing/trace_pipe` 文件来查看 eBPF 程序的输出：

```console
$ sudo cat /sys/kernel/debug/tracing/trace_pipe
              rm-9290    [004] d..2  4637.798698: bpf_trace_printk: fentry: pid = 9290, filename = test_file
              rm-9290    [004] d..2  4637.798843: bpf_trace_printk: fexit: pid = 9290, filename = test_file, ret = 0
              rm-9290    [004] d..2  4637.798698: bpf_trace_printk: fentry: pid = 9290, filename = test_file2
              rm-9290    [004] d..2  4637.798843: bpf_trace_printk: fexit: pid = 9290, filename = test_file2, ret = 0
```