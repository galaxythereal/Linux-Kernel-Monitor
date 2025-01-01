#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kernel_stat.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/sysinfo.h>

#define PROC_NAME "kernel_monitor"

static int proc_show(struct seq_file *m, void *v) {
  struct task_struct *task;
  struct mm_struct *mm;
  struct sysinfo mem_info;
  u64 user_time;

  // Get CPU usage
  seq_printf(m, "Kernel Monitor Data:\n");
  user_time = kcpustat_cpu(0).cpustat[CPUTIME_USER];
  seq_printf(m, "CPU Usage: %llu\n", user_time);

  // Get memory usage
  si_meminfo(&mem_info);
  seq_printf(m, "Memory Usage: %lu\n", mem_info.freeram);

  // Iterate through tasks and print their details
  for_each_process(task) {
    mm = get_task_mm(task);
    if (mm) {
      seq_printf(m, "Task: %s, PID: %d, Memory: %lu\n", task->comm, task->pid,
                 mm->total_vm);
      mmput(mm);
    }
  }

  return 0;
}

static int proc_open(struct inode *inode, struct file *file) {
  return single_open(file, proc_show, NULL);
}

static const struct proc_ops proc_fops = {
    .proc_open = proc_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int __init kernel_monitor_init(void) {
  // Create /proc entry
  if (!proc_create(PROC_NAME, 0, NULL, &proc_fops)) {
    printk(KERN_ERR "Kernel Monitor: Failed to create /proc entry\n");
    return -ENOMEM;
  }

  printk(KERN_INFO "Kernel Monitor: Initialized\n");
  return 0;
}

static void __exit kernel_monitor_exit(void) {
  remove_proc_entry(PROC_NAME, NULL);
  printk(KERN_INFO "Kernel Monitor: Exited\n");
}

module_init(kernel_monitor_init);
module_exit(kernel_monitor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Kernel Monitor Module");
