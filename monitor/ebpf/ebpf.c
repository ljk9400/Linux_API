// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
/* Copyright (c) 2022 Meta Platforms, Inc. */
//#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

//#include "profile.h"



char LICENSE[] SEC("license") = "Dual BSD/GPL";


/**
 * 这里定义了一个类型为 BPF_MAP_TYPE_RINGBUF 的 eBPF maps 。
 * Ring Buffer 是一种高性能的循环缓冲区，用于在内核和用户空间之间传输数据。max_entries 设置了 Ring Buffer 的最大大小。
*/
struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 256 * 1024);
} events SEC(".maps");

/**
 * 这里定义了一个名为 profile 的 eBPF 程序，它将在 perf 事件触发时执行。
*/
SEC("perf_event")
int profile(void *ctx)
{
    int pid = bpf_get_current_pid_tgid() >> 32;
    int cpu_id = bpf_get_smp_processor_id();
    struct stacktrace_event *event;
    int cp;

    // 预留 Ring Buffer 空间
    event = bpf_ringbuf_reserve(&events, sizeof(*event), 0);
    if (!event)
        return 1;

    event->pid = pid;
    event->cpu_id = cpu_id;

    if (bpf_get_current_comm(event->comm, sizeof(event->comm)))
        event->comm[0] = 0;

    // 获取内核栈信息
    event->kstack_sz = bpf_get_stack(ctx, event->kstack, sizeof(event->kstack), 0);

    // 获取用户空间栈信息
    event->ustack_sz = bpf_get_stack(ctx, event->ustack, sizeof(event->ustack), BPF_F_USER_STACK);

    // 将事件提交到 Ring Buffe，以便用户空间程序可以读取和处理
    bpf_ringbuf_submit(event, 0);

    return 0;
}


// 用户态
static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                int cpu, int group_fd, unsigned long flags)
{
    int ret;

    ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
    return ret;
}

static void show_stack_trace(__u64 *stack, int stack_sz, pid_t pid)
{
    const struct blazesym_result *result;
    const struct blazesym_csym *sym;
    sym_src_cfg src;
    int i, j;

    if (pid) {
        src.src_type = SRC_T_PROCESS;
        src.params.process.pid = pid;
    } else {
        src.src_type = SRC_T_KERNEL;
        src.params.kernel.kallsyms = NULL;
        src.params.kernel.kernel_image = NULL;
    }

    result = blazesym_symbolize(symbolizer, &src, 1, (const uint64_t *)stack, stack_sz);

    for (i = 0; i < stack_sz; i++) {
        if (!result || result->size <= i || !result->entries[i].size) {
            printf("  %d [<%016llx>]\n", i, stack[i]);
            continue;
        }

        if (result->entries[i].size == 1) {
            sym = &result->entries[i].syms[0];
            if (sym->path && sym->path[0]) {
                printf("  %d [<%016llx>] %s+0x%llx %s:%ld\n",
                       i, stack[i], sym->symbol,
                       stack[i] - sym->start_address,
                       sym->path, sym->line_no);
            } else {
                printf("  %d [<%016llx>] %s+0x%llx\n",
                       i, stack[i], sym->symbol,
                       stack[i] - sym->start_address);
            }
            continue;
        }

        printf("  %d [<%016llx>]\n", i, stack[i]);
        for (j = 0; j < result->entries[i].size; j++) {
            sym = &result->entries[i].syms[j];
            if (sym->path && sym->path[0]) {
                printf("        %s+0x%llx %s:%ld\n",
                       sym->symbol, stack[i] - sym->start_address,
                       sym->path, sym->line_no);
            } else {
                printf("        %s+0x%llx\n", sym->symbol,
                       stack[i] - sym->start_address);
            }
        }
    }

    blazesym_result_free(result);
}

/* Receive events from the ring buffer. */
static int event_handler(void *_ctx, void *data, size_t size)
{
    struct stacktrace_event *event = data;

    if (event->kstack_sz <= 0 && event->ustack_sz <= 0)
        return 1;

    printf("COMM: %s (pid=%d) @ CPU %d\n", event->comm, event->pid, event->cpu_id);

    if (event->kstack_sz > 0) {
        printf("Kernel:\n");
        show_stack_trace(event->kstack, event->kstack_sz / sizeof(__u64), 0);
    } else {
        printf("No Kernel Stack\n");
    }

    if (event->ustack_sz > 0) {
        printf("Userspace:\n");
        show_stack_trace(event->ustack, event->ustack_sz / sizeof(__u64), event->pid);
    } else {
        printf("No Userspace Stack\n");
    }

    printf("\n");
    return 0;
}


int main() 
{
    for (cpu = 0; cpu < num_cpus; cpu++) {
        /* skip offline/not present CPUs */
        if (cpu >= num_online_cpus || !online_mask[cpu])
            continue;

        /* Set up performance monitoring on a CPU/Core */
        pefd = perf_event_open(&attr, pid, cpu, -1, PERF_FLAG_FD_CLOEXEC);
        if (pefd < 0) {
            fprintf(stderr, "Fail to set up performance monitor on a CPU/Core\n");
            err = -1;
            goto cleanup;
        }
        pefds[cpu] = pefd;

        /* Attach a BPF program on a CPU */
        links[cpu] = bpf_program__attach_perf_event(skel->progs.profile, pefd);
        if (!links[cpu]) {
            err = -1;
            goto cleanup;
        }
    }
}
