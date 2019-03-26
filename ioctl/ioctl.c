#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#define DUMMY_MAJOR 56
#define DUMMY_DEVICE "dummy"

long dummy_unlocked_ioctl(struct file* file,
	unsigned int cmd, unsigned long arg)
{
	switch(cmd)
	{
		case 0x01:
			printk("command 1: arg = %lu\n", arg);
			break;
		default:
			printk("command = 0x%x\n, arg = %lu",cmd,arg);

	}

	return 0;
}


struct file_operations dummy_ops = {
	owner: THIS_MODULE,
	unlocked_ioctl: dummy_unlocked_ioctl,
};


int init_module(void)
{
	if(register_chrdev(DUMMY_MAJOR,DUMMY_DEVICE,&dummy_ops) < 0)
		printk("major number %d not available \n",DUMMY_MAJOR);
	
	return 0;
}


void cleanup_module(void)
{
	unregister_chrdev(DUMMY_MAJOR,DUMMY_DEVICE);
}


MODULE_LICENSE("GPL");






